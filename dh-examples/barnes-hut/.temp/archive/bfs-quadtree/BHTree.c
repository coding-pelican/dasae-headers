#include "BHTree.h"
#include <stdlib.h> // malloc, free, qsort
#include <string.h> // memset

// safe min function
static f32 fminf2(f32 a, f32 b) { return (a < b) ? a : b; }
// safe max function
static f32 fmaxf2(f32 a, f32 b) { return (a > b) ? a : b; }

// Possibly inline
$inline_always f32   lenSq(f32 dx, f32 dy) { return dx * dx + dy * dy; }
// We will store children for node i at index base=4*i+1.. base+3
// Return (4*i + 1).
$inline_always usize BHTree_childIndex(usize i) { return 4 * i + 1; }

// Partition bodies into quadrants, used for building
// returns the count that go to quadrant 'q'
static usize partitionBodies(Body* arr, usize n, f32 cx, f32 cy, f32 half, u8 q) {
    unused(cx), unused(half);
    // quadrant bits: q & 1 => x>cx? ; q & 2 => y>cy?
    f32   midx  = cx;
    f32   midy  = cy;
    // We'll do a stable partition or an in-place partition
    usize write = 0;
    for (usize i = 0; i < n; ++i) {
        f32 x     = arr[i].x;
        f32 y     = arr[i].y;
        u8  thisQ = 0;
        if (x > midx) {
            thisQ |= 1;
        }
        if (y > midy) {
            thisQ |= 2;
        }
        if (thisQ == q) {
            // swap arr[i], arr[write]
            if (write != i) {
                Body tmp   = arr[i];
                arr[i]     = arr[write];
                arr[write] = tmp;
            }
            write++;
        }
    }
    return write;
}

bool BHTree_init(BHTree* tree, usize max_nodes, f32 theta, f32 eps) {
    if (!tree) {
        return false;
    }
    tree->nodes = (BHNode*)calloc(max_nodes, sizeof(BHNode));
    if (!tree->nodes) {
        return false;
    }
    tree->capacity = max_nodes;
    tree->length   = 0;
    tree->theta    = theta;
    tree->epsilon  = eps;
    return true;
}

void BHTree_fini(BHTree* tree) {
    if (tree && tree->nodes) {
        free(tree->nodes);
        tree->nodes = NULL;
    }
    if (tree) {
        tree->capacity = 0;
        tree->length   = 0;
    }
}

// BFS queue approach to building an implicit quadtree
// We store nodes at indices: i=0 => root; children => 4*i+1..+4
// If node 'i' has more than LEAF_CAP bodies, we subdivide once
// and push children. If children themselves have > LEAF_CAP, we subdivide *them*, etc.
bool BHTree_build(BHTree* tree, Body* bodies, usize n) { // NOLINT
    if (!tree || !tree->nodes) {
        return false;
    }
    tree->length = 0;
    if (n == 0) {
        return true;
    }

    // 1) find bounding box
    f32 minx = FLT_MAX;
    f32 miny = FLT_MAX;
    f32 maxx = -FLT_MAX;
    f32 maxy = -FLT_MAX;
    for (usize i = 0; i < n; ++i) {
        f32 x = bodies[i].x;
        f32 y = bodies[i].y;
        minx  = fminf2(minx, x);
        maxx  = fmaxf2(maxx, x);
        miny  = fminf2(miny, y);
        maxy  = fmaxf2(maxy, y);
    }
    f32 cx = 0.5f * (minx + maxx);
    f32 cy = 0.5f * (miny + maxy);
    f32 hw = 0.5f * ((maxx - minx) > (maxy - miny) ? (maxx - minx) : (maxy - miny));

    // 2) push root node
    if (tree->capacity < 1) {
        return false;
    }
    BHNode* root    = &tree->nodes[0];
    tree->length    = 1;
    // init root
    root->center_x  = cx;
    root->center_y  = cy;
    root->half_size = hw;
    root->count     = (u8)n;
    root->com_x     = 0.f;
    root->com_y     = 0.f;
    root->com_mass  = 0.f;
    // copy first n bodies as a local leaf array if n <= LEAF_CAP
    // else we store them in 'bodies' externally and subdiv later
    if (n <= LEAF_CAP) {
        for (usize i = 0; i < n; ++i) {
            root->leaf[i].x    = bodies[i].x;
            root->leaf[i].y    = bodies[i].y;
            root->leaf[i].mass = bodies[i].mass;
        }
    }

    // We'll do BFS with a queue of (nodeIndex, bodyStart, bodyCount)
    // but let's store it in a small local stack for clarity
    typedef struct {
        usize nodeIndex;
        Body* arr;
        usize count;
    } BuildTask;

    BuildTask tasks[256] = cleared(); // limit for BFS depth ~ not too large
    usize     head       = 0;
    usize     tail       = 0;

    // Root item
    tasks[tail++] = (BuildTask){ 0, bodies, n };

    while (head < tail) {
        BuildTask T   = tasks[head++];
        usize     i   = T.nodeIndex;
        Body*     arr = T.arr;
        usize     cnt = T.count;

        BHNode* nd = &tree->nodes[i];

        // If we exceed LEAF_CAP, subdivide
        if (cnt > LEAF_CAP) {
            // subdiv
            f32   half = nd->half_size * 0.5f;
            // we have children => 4*i+1..+4
            usize base = BHTree_childIndex(i);

            // ensure capacity for these children
            if (base + 3 >= tree->capacity) {
                return false;
            }

            // Subdivide 'arr' into 4 quadrants in-place
            // We'll do: quadrant 0 => bottom-left, 1 => bottom-right, 2 => top-left, 3 => top-right
            // (But you can do bit logic or your own quadrant order.)
            // We do partial partition for each quadrant in sequence
            // The sequence of partition calls is an easy approach:
            //   #0 => x <= center_x, y <= center_y
            //   #1 => x >  center_x, y <= center_y
            //   #2 => x <= center_x, y >  center_y
            //   #3 => x >  center_x, y >  center_y
            // We'll do it manually for clarity.

            // quadrant 0
            usize q0_count = 0;
            {
                f32   midx  = nd->center_x;
                f32   midy  = nd->center_y;
                usize write = 0;
                for (usize k = 0; k < cnt; ++k) {
                    f32 x = arr[k].x;
                    f32 y = arr[k].y;
                    if (x <= midx && y <= midy) {
                        // swap arr[k], arr[write]
                        if (write != k) {
                            Body tmp   = arr[k];
                            arr[k]     = arr[write];
                            arr[write] = tmp;
                        }
                        write++;
                    }
                }
                q0_count = write;
            }
            // quadrant 1 => x>midx, y<=midy
            usize q1_count = 0;
            {
                f32   midx  = nd->center_x;
                f32   midy  = nd->center_y;
                usize start = 0;
                usize write = 0;
                // start from q0_count
                start       = q0_count;
                write       = q0_count;
                for (usize k = start; k < cnt; ++k) {
                    f32 x = arr[k].x;
                    f32 y = arr[k].y;
                    if (x > midx && y <= midy) {
                        if (write != k) {
                            Body tmp   = arr[k];
                            arr[k]     = arr[write];
                            arr[write] = tmp;
                        }
                        write++;
                    }
                }
                q1_count = write - start;
            }
            // quadrant 2 => x<=midx, y>midy
            usize q2_count = 0;
            {
                f32   midx  = nd->center_x;
                f32   midy  = nd->center_y;
                usize start = q0_count + q1_count;
                usize write = start;
                for (usize k = start; k < cnt; ++k) {
                    f32 x = arr[k].x;
                    f32 y = arr[k].y;
                    if (x <= midx && y > midy) {
                        if (write != k) {
                            Body tmp   = arr[k];
                            arr[k]     = arr[write];
                            arr[write] = tmp;
                        }
                        write++;
                    }
                }
                q2_count = write - start;
            }
            // quadrant 3 => remainder
            usize q3_count = cnt - (q0_count + q1_count + q2_count);

            // Now allocate the 4 children
            for (usize c = 0; c < 4; ++c) {
                usize   child_i = base + c;
                BHNode* ch      = &tree->nodes[child_i];
                ch->center_x    = nd->center_x + ((c & 1) ? +half : -half) * 0.5f;
                ch->center_y    = nd->center_y + ((c & 2) ? +half : -half) * 0.5f;
                ch->half_size   = half;
                ch->count       = 0;
                ch->com_x       = 0.f;
                ch->com_y       = 0.f;
                ch->com_mass    = 0.f;
            }
            tree->length = (base + 4 > tree->length) ? (base + 4) : tree->length;

            // push BFS tasks for each quadrant
            // we also copy the child's bodies if <= LEAF_CAP
            {
                // quadrant 0 => arr[0.. q0_count-1]
                usize child0 = base + 0;
                usize c0n    = q0_count;
                if (c0n <= LEAF_CAP) {
                    BHNode* c0 = &tree->nodes[child0];
                    c0->count  = (u8)c0n;
                    for (usize x = 0; x < c0n; ++x) {
                        c0->leaf[x].x    = arr[x].x;
                        c0->leaf[x].y    = arr[x].y;
                        c0->leaf[x].mass = arr[x].mass;
                    }
                }
                if (c0n > 0) {
                    tasks[tail++] = (BuildTask){ child0, arr, c0n };
                }

                // quadrant 1 => arr[q0_count.. q0_count+q1_count-1]
                usize start1 = q0_count;
                usize c1n    = q1_count;
                usize child1 = base + 1;
                if (c1n <= LEAF_CAP) {
                    BHNode* c1 = &tree->nodes[child1];
                    c1->count  = (u8)c1n;
                    for (usize x = 0; x < c1n; ++x) {
                        c1->leaf[x].x    = arr[start1 + x].x;
                        c1->leaf[x].y    = arr[start1 + x].y;
                        c1->leaf[x].mass = arr[start1 + x].mass;
                    }
                }
                if (c1n > 0) {
                    tasks[tail++] = (BuildTask){ child1, &arr[start1], c1n };
                }

                // quadrant 2 => arr[q0_count+q1_count..]
                usize start2 = q0_count + q1_count;
                usize c2n    = q2_count;
                usize child2 = base + 2;
                if (c2n <= LEAF_CAP) {
                    BHNode* c2 = &tree->nodes[child2];
                    c2->count  = (u8)c2n;
                    for (usize x = 0; x < c2n; ++x) {
                        c2->leaf[x].x    = arr[start2 + x].x;
                        c2->leaf[x].y    = arr[start2 + x].y;
                        c2->leaf[x].mass = arr[start2 + x].mass;
                    }
                }
                if (c2n > 0) {
                    tasks[tail++] = (BuildTask){ child2, &arr[start2], c2n };
                }

                // quadrant 3 => arr[q0_count+q1_count+q2_count.. end]
                usize start3 = q0_count + q1_count + q2_count;
                usize c3n    = q3_count;
                usize child3 = base + 3;
                if (c3n <= LEAF_CAP) {
                    BHNode* c3 = &tree->nodes[child3];
                    c3->count  = (u8)c3n;
                    for (usize x = 0; x < c3n; ++x) {
                        c3->leaf[x].x    = arr[start3 + x].x;
                        c3->leaf[x].y    = arr[start3 + x].y;
                        c3->leaf[x].mass = arr[start3 + x].mass;
                    }
                }
                if (c3n > 0) {
                    tasks[tail++] = (BuildTask){ child3, &arr[start3], c3n };
                }
            }
        } else {
            // It's a leaf
            // compute center-of-mass right away
            f32 sx = 0.f;
            f32 sy = 0.f;
            f32 sm = 0.f;
            // either we have it in node->leaf[] if we copied earlier, or we copy if not
            if (cnt <= LEAF_CAP) {
                // we presumably already copied them above
                for (u16 j = 0; j < nd->count; ++j) {
                    f32 m = nd->leaf[j].mass;
                    sx += nd->leaf[j].x * m;
                    sy += nd->leaf[j].y * m;
                    sm += m;
                }
            } else {
                nd->count = (u8)cnt;
                // copy now
                for (usize j = 0; j < cnt; ++j) {
                    nd->leaf[j].x    = arr[j].x;
                    nd->leaf[j].y    = arr[j].y;
                    nd->leaf[j].mass = arr[j].mass;
                    f32 m            = arr[j].mass;
                    sx += arr[j].x * m;
                    sy += arr[j].y * m;
                    sm += m;
                }
            }
            nd->com_mass = sm;
            if (sm > 0.f) {
                nd->com_x = sx / sm;
                nd->com_y = sy / sm;
            } else {
                nd->com_x = nd->center_x;
                nd->com_y = nd->center_y;
            }
        }
    }

    // Next, do an upward pass to compute com for internal nodes
    // In BFS layout, we can do this from the last node down to 0
    // i.e. if node i is internal, we sum children
    // But we can't easily detect internal vs leaf by 'count>LEAF_CAP' because we forced subdiv.
    // Instead we detect if all children exist in range or if i+1> length => leaf
    // Actually, simpler is: if count == 0 => branch node => sum children
    for (usize i = tree->length; i > 0; i--) {
        usize   nd_i = i - 1;
        BHNode* nd   = &tree->nodes[nd_i];
        if (nd->count == 0) {
            // sum children => i.e. 4 children => 4*nd_i+1..+4
            usize base = BHTree_childIndex(nd_i);
            f32   sx   = 0.f;
            f32   sy   = 0.f;
            f32   sm   = 0.f;
            for (usize c = 0; c < 4; ++c) {
                usize ch_i = base + c;
                if (ch_i < tree->length) {
                    BHNode* ch = &tree->nodes[ch_i];
                    sx += ch->com_x * ch->com_mass;
                    sy += ch->com_y * ch->com_mass;
                    sm += ch->com_mass;
                }
            }
            nd->com_mass = sm;
            if (sm > 0.f) {
                nd->com_x = sx / sm;
                nd->com_y = sy / sm;
            } else {
                nd->com_x = nd->center_x;
                nd->com_y = nd->center_y;
            }
        }
    }

    return true;
}

// Single pass Barnes-Hut force for each body
void BHTree_computeForces(const BHTree* tree, Body* bodies, usize n) {
    if (!tree || !tree->nodes) { return; }
    // We'll do a naive approach: For each body, traverse the tree
    // If node is far enough or is a leaf, use node COM or bodies.
    // This is O(n log n) in typical distributions.

    f32 theta_sq = tree->theta * tree->theta;
    f32 eps_sq   = tree->epsilon * tree->epsilon;

    for (usize i = 0; i < n; ++i) {
        f32 ax = 0.0f;
        f32 ay = 0.0f;
        f32 bx = bodies[i].x;
        f32 by = bodies[i].y;

        // stack-based traversal
        usize stack[128] = cleared(); // adapt as needed
        usize top        = 0;
        stack[top++]     = 0; // root index

        while (top > 0) {
            usize nd_i = stack[--top];
            if (nd_i >= tree->length) {
                continue; // skip invalid node
            }
            const BHNode* nd = &tree->nodes[nd_i];

            f32 dx  = nd->com_x - bx;
            f32 dy  = nd->com_y - by;
            f32 dsq = dx * dx + dy * dy;

            // check leaf or far-enough
            f32 size = nd->half_size * 2.f; // full width
            // "far enough" => size^2 < dsq * theta^2
            if (nd->count == 0) {
                // it's internal => check if far enough
                if (size * size < dsq * theta_sq) {
                    // approximate
                    f32 denom = (dsq + eps_sq) * sqrtf(dsq + eps_sq);
                    if (denom > 1e-20f) {
                        f32 factor = nd->com_mass / denom;
                        ax += dx * factor;
                        ay += dy * factor;
                    }
                } else {
                    // not far => push children
                    usize base = BHTree_childIndex(nd_i);
                    for (usize c = 0; c < 4; ++c) {
                        usize child_i = base + c;
                        if (child_i < tree->length) {
                            stack[top++] = child_i;
                        }
                    }
                }
            } else {
                // leaf => if far, approximate, else sum each body
                // or simply sum each body for a leaf. But let's do the standard test.
                if (size * size < dsq * theta_sq) {
                    // approximate by leaf's com
                    f32 denom = (dsq + eps_sq) * sqrtf(dsq + eps_sq);
                    if (denom > 1e-20f) {
                        f32 factor = nd->com_mass / denom;
                        ax += dx * factor;
                        ay += dy * factor;
                    }
                } else {
                    // sum each body
                    for (u16 j = 0; j < nd->count; ++j) {
                        f32 px = nd->leaf[j].x;
                        f32 py = nd->leaf[j].y;
                        f32 pm = nd->leaf[j].mass;
                        f32 rx = px - bx;
                        f32 ry = py - by;
                        f32 rr = rx * rx + ry * ry + eps_sq;
                        f32 r3 = rr * sqrtf(rr);
                        if (r3 > 1e-20f) {
                            f32 f = pm / r3;
                            ax += rx * f;
                            ay += ry * f;
                        }
                    }
                }
            }
        }

        bodies[i].ax = ax;
        bodies[i].ay = ay;
    }
}
