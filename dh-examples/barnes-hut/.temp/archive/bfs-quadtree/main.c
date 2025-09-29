#include <stdio.h>
#include <stdlib.h>
#include "BHTree.h"

int main(void) {
    const size_t N      = 100000;
    Body*        bodies = (Body*)malloc(N * sizeof(Body));
    if (!bodies) { return 1; }
    // Initialize bodies with some distribution (e.g. random)
    for (size_t i = 0; i < N; i++) {
        bodies[i].x    = /* random / uniform disc / etc. */;
        bodies[i].y    = /* ... */;
        bodies[i].vx   = 0.f;
        bodies[i].vy   = 0.f;
        bodies[i].mass = 1.0f; // or random
        bodies[i].ax   = 0.f;
        bodies[i].ay   = 0.f;
    }

    // Barnes-Hut config
    BHTree tree;
    // For 100k bodies, we might have up to ~some factor of n.
    // Hard to know exactly, but let's guess 4*n is safe for BFS array:
    bool   ok = bh_initTree(&tree, 4 * N, 1.0f /*theta*/, 0.01f /*epsilon*/);
    if (!ok) {
        fprintf(stderr, "Fail to alloc tree\n");
        free(bodies);
        return 1;
    }

    // Build
    if (!bh_buildTree(&tree, bodies, N)) {
        fprintf(stderr, "Fail to build tree\n");
        bh_freeTree(&tree);
        free(bodies);
        return 1;
    }

    // Compute forces
    bh_computeForces(&tree, bodies, N);

    // Now do an "integration step" or collision detection or ...
    // e.g.:
    for (size_t i = 0; i < N; i++) {
        bodies[i].vx += bodies[i].ax * 0.01f; // dt
        bodies[i].vy += bodies[i].ay * 0.01f;
        bodies[i].x += bodies[i].vx * 0.01f;
        bodies[i].y += bodies[i].vy * 0.01f;
    }

    // ...
    bh_freeTree(&tree);
    free(bodies);
    return 0;
}
