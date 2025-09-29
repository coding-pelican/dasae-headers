#include "dh/prl.h"
#include "dh/math/common.h"
#include "dh/math/vec.h"
#include "dh/mem/Allocator.h"
#include "dh/heap/Page.h"
#include "dh/ArrList.h"

#if bti_plat_windows
#include "dh/os/windows.h"
#endif

#include "hex-sphere/stream.h"


func((normalize(m_V3f64 v))(m_V3f64)) {
    return m_V3f64_norm(v);
}
func((midpoint(m_V3f64 a, m_V3f64 b))(m_V3f64)) {
    return m_V3f64_add(a, m_V3f64_scale(m_V3f64_sub(b, a), 0.5));
}

typedef union m_M4x4 {
    f64 s[4][4];
    struct {
        f64 m00, m01, m02, m03;
        f64 m10, m11, m12, m13;
        f64 m20, m21, m22, m23;
        f64 m30, m31, m32, m33;
    };
} m_M4x4;
m_M4x4 m_M4x4_mul(m_M4x4 lhs, m_M4x4 rhs) {
    var result = make$(m_M4x4);
    for (i32 i = 0; i < 4; ++i) {
        for (i32 j = 0; j < 4; ++j) {
            result.s[i][j] = lhs.s[i][0] * rhs.s[0][j]
                           + lhs.s[i][1] * rhs.s[1][j]
                           + lhs.s[i][2] * rhs.s[2][j]
                           + lhs.s[i][3] * rhs.s[3][j];
        }
    }
    return result;
}
m_M4x4 m_M4x4_rotation(f64 yaw, f64 pitch, f64 roll) {
    let cy = math_cos(yaw);
    let sy = math_sin(yaw);
    let cp = math_cos(pitch);
    let sp = math_sin(pitch);
    let cr = math_cos(roll);
    let sr = math_sin(roll);
    return (m_M4x4){ {
        { cy * cr + sy * sp * sr, -cy * sr + sy * sp * cr, sy * cp, 0 },
        { cp * sr, cp * cr, -sp, 0 },
        { -sy * cr + cy * sp * sr, sy * sr + cy * sp * cr, cy * cp, 0 },
        { 0, 0, 0, 1 },
    } };
}
m_M4x4 m_M4x4_translation(f64 x, f64 y, f64 z) {
    return (m_M4x4){ {
        { 1, 0, 0, x },
        { 0, 1, 0, y },
        { 0, 0, 1, z },
        { 0, 0, 0, 1 },
    } };
}
m_M4x4 m_M4x4_perspective(f64 fov, f64 aspect, f64 near_plane, f64 far_plane) {
    let f  = 1.0 / math_tan(fov * 0.5);
    let nf = 1.0 / (near_plane - far_plane);
    return (m_M4x4){ {
        { f / aspect, 0, 0, 0 },
        { 0, f, 0, 0 },
        { 0, 0, (far_plane + near_plane) * nf, 2 * far_plane * near_plane * nf },
        { 0, 0, -1, 0 },
    } };
}


m_V3f64 m_V3f64_transform(m_V3f64 v, m_M4x4 m) {
    let w = m.s[3][0] * v.x + m.s[3][1] * v.y + m.s[3][2] * v.z + m.s[3][3];
    return m_V3f64_from(
        (m.s[0][0] * v.x + m.s[0][1] * v.y + m.s[0][2] * v.z + m.s[0][3]) / w,
        (m.s[1][0] * v.x + m.s[1][1] * v.y + m.s[1][2] * v.z + m.s[1][3]) / w,
        (m.s[2][0] * v.x + m.s[2][1] * v.y + m.s[2][2] * v.z + m.s[2][3]) / w
    );
}


typedef struct RGB {
    u8 r, g, b;
} RGB;
use_Sli$(RGB);
typedef struct Screen {
    m_V2i32 dimension;
    Sli$RGB color_buffer;
    Sli$RGB prev_color_buffer;  // Double buffering for change detection
    Sli$f64 depth_buffer;
    Sli$bool dirty_pixels;         // Track which pixels changed
} Screen;
use_Err$(Screen);
func((Screen_init(m_V2i32 dimension, mem_Allocator allocator))(Err$Screen)$guard) {
    var screen = make$(Screen);

    screen.dimension = dimension;
    let pixel_count = as$(usize, dimension.x) * dimension.y;
    screen.prev_color_buffer = meta_cast$(Sli$RGB, try_(mem_Allocator_alloc(allocator, typeInfo$(RGB), pixel_count)));
    errdefer_($ignore_capture, mem_Allocator_free(allocator, anySli(screen.prev_color_buffer)));
    screen.color_buffer = meta_cast$(Sli$RGB, try_(mem_Allocator_alloc(allocator, typeInfo$(RGB), pixel_count)));
    errdefer_($ignore_capture, mem_Allocator_free(allocator, anySli(screen.color_buffer)));
    screen.depth_buffer = meta_cast$(Sli$f64, try_(mem_Allocator_alloc(allocator, typeInfo$(f64), pixel_count)));
    errdefer_($ignore_capture, mem_Allocator_free(allocator, anySli(screen.depth_buffer)));
    screen.dirty_pixels = meta_cast$(Sli$bool, try_(mem_Allocator_alloc(allocator, typeInfo$(bool), pixel_count)));
    errdefer_($ignore_capture, mem_Allocator_free(allocator, anySli(screen.dirty_pixels)));

    return_ok(screen);
} $unguarded;
func((Screen_fini(Screen* self, mem_Allocator allocator))(void)) {
    mem_Allocator_free(allocator, anySli(self->color_buffer));
    mem_Allocator_free(allocator, anySli(self->prev_color_buffer));
    mem_Allocator_free(allocator, anySli(self->depth_buffer));
    mem_Allocator_free(allocator, anySli(self->dirty_pixels));
}
func((Screen_getRGB(const Screen* self, i32 x, i32 y))(RGB)) {
    if (0 <= x && x < self->dimension.x && 0 <= y && y < self->dimension.y) {
        return Sli_getAt(self->color_buffer, y * self->dimension.x + x);
    }
    return (RGB){ 0, 0, 0 };
}
func((Screen_setRGB(Screen* self, i32 x, i32 y, RGB color))(void)) {
    if (0 <= x && x < self->dimension.x && 0 <= y && y < self->dimension.y) {
        let idx = y * self->dimension.x + x;
        Sli_setAt(self->color_buffer, idx, color);
        Sli_setAt(self->dirty_pixels, idx, true);  // Mark pixel as dirty for change detection
    }
}

// 3D → 2D 스크린 좌표
m_V2f64 projectToScreen(m_V3f64 v, m_V2i32 dimension) {
    return m_V2f64_from(
        (v.x + 1.0) * dimension.x * 0.5,
        (1.0 - v.y) * dimension.y * 0.5 // invert y
    );
}
// Edge function for point-in-triangle test
f64 edge_testPointInTriangle(m_V2f64 a, m_V2f64 b, m_V2f64 c) {
    return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}

void Screen_drawLine(Screen* self, m_V3f64 v0, m_V3f64 v1, RGB color) {
    let p0 = projectToScreen(v0, self->dimension);
    let p1 = projectToScreen(v1, self->dimension);

    var x0 = as$(i32, p0.x);
    var y0 = as$(i32, p0.y);
    var x1 = as$(i32, p1.x);
    var y1 = as$(i32, p1.y);

    let dx  = abs(x1 - x0);
    let dy  = abs(y1 - y0);
    let sx  = (x0 < x1) ? 1 : -1;
    let sy  = (y0 < y1) ? 1 : -1;

    i32 err = dx - dy;
    while (true) {
        Screen_setRGB(self, x0, y0, color);
        if (x0 == x1 && y0 == y1) {
            break;
        }
        let err2 = 2 * err;
        if (err2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (err2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}
// 삼각형 래스터라이제이션 with Z-버퍼
void Screen_rasterizeTriangle(Screen* self, m_V3f64 v0, m_V3f64 v1, m_V3f64 v2, RGB color) {
    // 2D 투영
    let p0 = projectToScreen(v0, self->dimension);
    let p1 = projectToScreen(v1, self->dimension);
    let p2 = projectToScreen(v2, self->dimension);

    // Bounding box
    let min_x = fmax(0, fmin(p0.x, fmin(p1.x, p2.x)));
    let max_x = fmin(self->dimension.x - 1, fmax(p0.x, fmax(p1.x, p2.x)));
    let min_y = fmax(0, fmin(p0.y, fmin(p1.y, p2.y)));
    let max_y = fmin(self->dimension.y - 1, fmax(p0.y, fmax(p1.y, p2.y)));

    // 삼각형 면적
    let area = edge_testPointInTriangle(p0, p1, p2);
    if (area <= 0) {
        return; // Back-face culling
    }

    // 각 픽셀 테스트
    for (i32 y = as$(i32, min_y); y <= as$(i32, max_y); ++y) {
        for (i32 x = as$(i32, min_x); x <= as$(i32, max_x); ++x) {
            m_V2f64 p = m_V2f64_from(x + 0.5, y + 0.5);

            // Barycentric coordinates
            let w0 = edge_testPointInTriangle(p1, p2, p) / area;
            let w1 = edge_testPointInTriangle(p2, p0, p) / area;
            let w2 = edge_testPointInTriangle(p0, p1, p) / area;

            // 삼각형 내부 테스트
            if (0 <= w0 && 0 <= w1 && 0 <= w2) {
                // Z 보간
                let z   = w0 * v0.z + w1 * v1.z + w2 * v2.z;
                // Z-버퍼 테스트
                let idx = y * self->dimension.x + x;
                if (z < Sli_getAt(self->depth_buffer, idx)) {
                    Sli_setAt(self->depth_buffer, idx, z);
                    Screen_setRGB(self, x, y, color);
                }
            }
        }
    }
}


// Forward declarations
typedef struct Face Face;
typedef struct Vertex Vertex;
typedef struct Tile Tile;
typedef struct HexSphere HexSphere;

use_ArrList$(i32);
use_Sli$(m_V3f64);
use_ArrList$(m_V3f64);

// Face structure - represents a triangular face
typedef struct Face {
    i32 vertex_indices[3];
    m_V3f64 centroid;
    i32 id;
} Face;
use_Sli$(Face);
use_ArrList$(Face);

// Vertex structure - enhanced to track adjacent faces
typedef struct Vertex {
    m_V3f64 position;
    ArrList$i32 face_ids;  // IDs of faces that contain this vertex
} Vertex;
use_Sli$(Vertex);
use_ArrList$(Vertex);
use_Err$(Vertex);

// Tile structure - represents hexagonal/pentagonal tiles around vertices
typedef struct Tile {
    i32 center_vertex_id;
    ArrList$m_V3f64 boundary;  // Boundary points forming the tile
    i32 sides;                 // Number of sides (5 for pentagon, 6 for hexagon)
} Tile;
use_Sli$(Tile);
use_ArrList$(Tile);

// HexSphere structure - main geodesic sphere with hexagonal tiling
struct HexSphere {
    ArrList$Vertex vertices;
    ArrList$Face faces;
    ArrList$Tile tiles;
    f64 hex_size;  // Tile size factor (0.5 to 1.0)
};
use_Err$(HexSphere);

// Utility functions for vector operations
func((Face_calculateCentroid(const Face* face, const ArrList$Vertex* vertices))(m_V3f64)) {
    let v0 = Sli_getAt(vertices->items, face->vertex_indices[0]).position;
    let v1 = Sli_getAt(vertices->items, face->vertex_indices[1]).position;
    let v2 = Sli_getAt(vertices->items, face->vertex_indices[2]).position;
    return m_V3f64_scale(m_V3f64_add(m_V3f64_add(v0, v1), v2), 1.0/3.0);
}

func((Vertex_init(m_V3f64 position, mem_Allocator allocator))(Err$Vertex)$scope) {
    var vertex = make$(Vertex);
    vertex.position = position;
    vertex.face_ids.base[0] = ArrList_init(typeInfo$(i32), allocator);
    return_ok(vertex);
} $unscoped;

func((Vertex_addFace(Vertex* vertex, i32 face_id))(Err$void)$scope) {
    try_(ArrList_append(vertex->face_ids.base, meta_refPtr(&face_id)));
    return_ok({});
} $unscoped;

func((Vertex_fini(Vertex* vertex))(void)) {
    ArrList_fini(vertex->face_ids.base);
}

#define icosphere_vertex_count   (12)
#define icosphere_triangle_count (20)

func((HexSphere_init(f64 hex_size, mem_Allocator allocator))(Err$HexSphere)$guard) {
    var hex_sphere = make$(HexSphere);
    hex_sphere.hex_size = hex_size;

    // Initialize ArrLists
    hex_sphere.vertices.base[0] = ArrList_init(typeInfo$(Vertex), allocator);
    errdefer_($ignore_capture, ArrList_fini(hex_sphere.vertices.base));
    hex_sphere.faces.base[0] = ArrList_init(typeInfo$(Face), allocator);
    errdefer_($ignore_capture, ArrList_fini(hex_sphere.faces.base));
    hex_sphere.tiles.base[0] = ArrList_init(typeInfo$(Tile), allocator);
    errdefer_($ignore_capture, ArrList_fini(hex_sphere.tiles.base));

    let t = math_f64_golden_ratio;
    let s = math_sqrt(1 + t * t);
    let initial_positions = (Arr$$(icosphere_vertex_count, m_V3f64))Arr_init({
        [0]  = m_V3f64_from(-1, t, 0),
        [1]  = m_V3f64_from(1, t, 0),
        [2]  = m_V3f64_from(-1, -t, 0),
        [3]  = m_V3f64_from(1, -t, 0),
        [4]  = m_V3f64_from(0, -1, t),
        [5]  = m_V3f64_from(0, 1, t),
        [6]  = m_V3f64_from(0, -1, -t),
        [7]  = m_V3f64_from(0, 1, -t),
        [8]  = m_V3f64_from(t, 0, -1),
        [9]  = m_V3f64_from(t, 0, 1),
        [10] = m_V3f64_from(-t, 0, -1),
        [11] = m_V3f64_from(-t, 0, 1),
    });

    // Initialize vertices with normalized positions
    for (usize i = 0; i < icosphere_vertex_count; ++i) {
        let normalized_pos = m_V3f64_scale(*Arr_at(initial_positions, i), 1.0 / s);
        var vertex = try_(Vertex_init(normalized_pos, allocator));
        try_(ArrList_append(hex_sphere.vertices.base, meta_refPtr(&vertex)));
    }

    let face_indices = (Arr$$(icosphere_triangle_count, Face))Arr_init({
        [0]  = (Face){ {0, 11, 5}, .id = 0 },
        [1]  = (Face){ {0, 5, 1}, .id = 1 },
        [2]  = (Face){ {0, 1, 7}, .id = 2 },
        [3]  = (Face){ {0, 7, 10}, .id = 3 },
        [4]  = (Face){ {0, 10, 11}, .id = 4 },
        [5]  = (Face){ {1, 5, 9}, .id = 5 },
        [6]  = (Face){ {5, 11, 4}, .id = 6 },
        [7]  = (Face){ {11, 10, 2}, .id = 7 },
        [8]  = (Face){ {10, 7, 6}, .id = 8 },
        [9]  = (Face){ {7, 1, 8}, .id = 9 },
        [10] = (Face){ {3, 9, 4}, .id = 10 },
        [11] = (Face){ {3, 4, 2}, .id = 11 },
        [12] = (Face){ {3, 2, 6}, .id = 12 },
        [13] = (Face){ {3, 6, 8}, .id = 13 },
        [14] = (Face){ {3, 8, 9}, .id = 14 },
        [15] = (Face){ {4, 9, 5}, .id = 15 },
        [16] = (Face){ {2, 4, 11}, .id = 16 },
        [17] = (Face){ {6, 2, 10}, .id = 17 },
        [18] = (Face){ {8, 6, 7}, .id = 18 },
        [19] = (Face){ {9, 8, 1}, .id = 19 },
    });

    // Initialize faces and register them with vertices
    for (usize i = 0; i < icosphere_triangle_count; ++i) {
        var face = *Arr_at(face_indices, i);
        face.centroid = Face_calculateCentroid(&face, &hex_sphere.vertices);
        try_(ArrList_append(hex_sphere.faces.base, meta_refPtr(&face)));

        // Register this face with all its vertices
        for (i32 j = 0; j < 3; ++j) {
            var vertex = Sli_getAt(hex_sphere.vertices.items, face.vertex_indices[j]);
            try_(Vertex_addFace(&vertex, face.id));
        }
    }

    return_ok(hex_sphere);
} $unguarded;
func((HexSphere_fini(HexSphere* self))(void)) {
    // Free vertex face lists
    for_slice (self->vertices.items, vertex) {
        Vertex_fini(vertex);
    }
    ArrList_fini(self->vertices.base);

    // Free tile boundaries
    for_slice (self->tiles.items, tile) {
        ArrList_fini(tile->boundary.base);
    }
    ArrList_fini(self->tiles.base);

    ArrList_fini(self->faces.base);
}
// Utility function to sort face centroids around a vertex (simplified version)
func((sortCentroidsAroundVertex(const Vertex* vertex, const HexSphere* hex_sphere, ArrList$m_V3f64* sorted_centroids))(Err$void)$scope) {
    let face_count = vertex->face_ids.items.len;

    // Clear and populate centroids
    ArrList_clearRetainingCap(sorted_centroids->base);

    for (usize i = 0; i < face_count; ++i) {
        let face_id = Sli_getAt(vertex->face_ids.items, i);
        var face = Sli_getAt(hex_sphere->faces.items, face_id);
        try_(ArrList_append(sorted_centroids->base, meta_refPtr(&face.centroid)));
    }

    return_ok({});
} $unscoped;

// Generate hexagonal/pentagonal tiles around each vertex
func((HexSphere_generateTiles(HexSphere* self, mem_Allocator allocator))(Err$void)$guard) {
    // Clear existing tiles
    ArrList_clearRetainingCap(self->tiles.base);

    for (usize v_idx = 0; v_idx < self->vertices.items.len; ++v_idx) {
        let vertex = Sli_getAt(self->vertices.items, v_idx);

        if (vertex.face_ids.items.len == 0) {
            continue; // Skip vertices with no faces
        }

        var tile = make$(Tile);
        tile.center_vertex_id = as$(i32, v_idx);
        tile.sides = as$(i32, vertex.face_ids.items.len);

        // Initialize boundary ArrList
        tile.boundary.base[0] = ArrList_init(typeInfo$(m_V3f64), allocator);
        errdefer_($ignore_capture, ArrList_fini(tile.boundary.base));

        // Sort face centroids around vertex
        try_(sortCentroidsAroundVertex(&vertex, self, &tile.boundary));

        // Create boundary points by moving from centroid toward vertex by hex_size factor
        for (usize i = 0; i < tile.boundary.items.len; ++i) {
            let centroid = Sli_getAt(tile.boundary.items, i);
            let to_vertex = m_V3f64_sub(vertex.position, centroid);
            let boundary_point = m_V3f64_add(centroid, m_V3f64_scale(to_vertex, self->hex_size));
            let normalized_point = normalize(boundary_point); // Project back to sphere
            Sli_setAt(tile.boundary.items, i, normalized_point);
        }

        // Add tile to collection
        try_(ArrList_append(self->tiles.base, meta_refPtr(&tile)));
    }

    return_ok({});
} $unguarded;


// 간단한 Phong 조명 모델
RGB calcLighting(m_V3f64 normal, m_V3f64 light_dir, m_V3f64 view_dir, RGB base_color) {
    // Normalize vectors
    normal    = normalize(normal);
    light_dir = normalize(light_dir);
    view_dir  = normalize(view_dir);

    let ambient  = 0.2;
    let diffuse  = fmax(0, m_V3f64_dot(normal, light_dir)) * 0.6;
    let specular = pow(fmax(0, m_V3f64_dot(m_V3f64_reflect(light_dir, normal), view_dir)), 32) * 0.3;

    let intensity = ambient + diffuse + specular;
    return (RGB){
        prim_min(255, base_color.r * intensity),
        prim_min(255, base_color.g * intensity),
        prim_min(255, base_color.b * intensity)
    };
}
// 면 법선 계산
m_V3f64 calcNormal(m_V3f64 v0, m_V3f64 v1, m_V3f64 v2) {
    m_V3f64 edge1 = m_V3f64_sub(v1, v0);
    m_V3f64 edge2 = m_V3f64_sub(v2, v0);
    return normalize(m_V3f64_cross(edge1, edge2));
}

// Render a polygon (hexagon/pentagon) using triangulation
void Screen_renderPolygon(Screen* self, const ArrList$m_V3f64* boundary, RGB color, m_M4x4 mvp) {
    if (boundary->items.len < 3) {
        return;
    }

    // Transform all boundary points
    var transformed = eval_(Sli$m_V3f64 $scope) {
        static Arr$$(12, m_V3f64) temp_points = {}; // max sides
        let count = (boundary->items.len > Arr_len(temp_points)) ? Arr_len(temp_points) : boundary->items.len;
        for (usize i = 0; i < count; ++i) {
            Arr_setAt(temp_points, i, m_V3f64_transform(Sli_getAt(boundary->items, i), mvp));
        }
        eval_break_(Sli_slice(Arr_ref$(Sli$m_V3f64, temp_points), (0, count)));
    } $unscoped_eval;

    // Triangulate polygon with center point (fan triangulation)
    let center = eval_(m_V3f64 $scope) {
        var sum = m_V3f64_from(0, 0, 0);
        for_slice (transformed, point) {
            sum = m_V3f64_add(sum, *point);
        }
        eval_break_(m_V3f64_scale(sum, 1.0 / transformed.len));
    } $unscoped_eval;

    // Draw triangles from center to each edge
    for (usize i = 0; i < transformed.len; ++i) {
        let next_i = (i + 1) % transformed.len;
        Screen_rasterizeTriangle(
            self,
            center,
            Sli_getAt(transformed, i),
            Sli_getAt(transformed, next_i),
            color
        );
    }
}

void Screen_renderHexSphere(Screen* self, f64 rotation_x, f64 rotation_y, mem_Allocator allocator) {
    // 배경색 및 더블 버퍼 초기화
    let pixel_count = self->color_buffer.len;
    for (usize i = 0; i < pixel_count; ++i) {
        Sli_setAt(self->color_buffer, i, (RGB){ 20, 20, 40 });
        Sli_setAt(self->prev_color_buffer, i, (RGB){ 0, 0, 0 }); // Initialize prev buffer
        Sli_setAt(self->dirty_pixels, i, true); // Mark all pixels as dirty for first frame
    }

    // Z-버퍼 초기화
    for_slice (self->depth_buffer, depth) {
        *depth = 1.0;
    }

    // HexSphere 생성
    var hex_sphere = catch_((HexSphere_init(0.95, allocator))($ignore_capture, claim_unreachable));
    catch_((HexSphere_generateTiles(&hex_sphere, allocator))($ignore_capture, claim_unreachable));

    // 변환 행렬 설정
    let model = m_M4x4_rotation(rotation_y, rotation_x, 0);
    let view  = m_M4x4_translation(0.0, 0.0, -5.0); // 카메라 위치
    let proj  = m_M4x4_perspective(math_pi / 3.0, (f64)self->dimension.x / self->dimension.y, 0.1, 100.0);

    // 행렬 결합
    let mvp = m_M4x4_mul(proj, m_M4x4_mul(view, model));

    // 조명 설정
    let light_dir = m_V3f64_from(0.3, 0.5, 0.7);
    let view_dir  = m_V3f64_from(0.0, 0.0, 1.0);

    // 각 타일 렌더링
    for (usize t = 0; t < hex_sphere.tiles.items.len; ++t) {
        let tile = Sli_getAt(hex_sphere.tiles.items, t);

        // Pentagon vs Hexagon 색상 구분
        let base_color = eval_(RGB $scope) if (tile.sides == 5) {
            eval_break_({ 200, 150, 50 }); // Pentagon - 골드
        } else if (tile.sides == 6) {
            eval_break_({ 50, 150, 200 }); // Hexagon - 블루
        } else {
            eval_break_({ 100, 100, 100 }); // Other - 그레이
        } $unscoped_eval;

        // 법선 계산 (타일 중심점 사용)
        let center_vertex = Sli_getAt(hex_sphere.vertices.items, tile.center_vertex_id);
        let normal = normalize(center_vertex.position); // 구면에서는 중심점이 법선

        // 조명 적용
        let lit_color = calcLighting(normal, light_dir, view_dir, base_color);

        // 폴리곤 렌더링
        Screen_renderPolygon(self, &tile.boundary, lit_color, mvp);
    }

    HexSphere_fini(&hex_sphere);
}

#include "dh/heap/Fixed.h"

/// @brief Ultra-fast RGB buffer display with change detection and batched output
/// @param self Screen buffer containing RGB data
/// @details Maximum performance rendering using dirty pixel tracking, batched output,
///          and optimized ANSI sequences. Only updates changed pixels for speed.
/// @example
///   Screen_display(&screen);
/// @dependencies Windows API (Windows), ANSI terminal support
/// @limitations Requires terminal with ANSI escape sequence support
func((Screen_display(Screen* self))(void)) {
#if bti_plat_windows
    // Windows console setup for ANSI escape sequences (one-time only)
    static bool s_console_initialized = false;
    if (!s_console_initialized) {
        if_(let handle =GetStdHandle(STD_OUTPUT_HANDLE), handle != INVALID_HANDLE_VALUE) {
            if_(DWORD mode = 0, GetConsoleMode(handle, &mode)) {
                mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
                $ignore = SetConsoleMode(handle, mode);
                s_console_initialized = true;
            }
        }
    }
#endif
    // Output buffer setup (one-time only)
    use_ArrList$(u8);
    static ArrList$u8 output_buffer = {};
    static bool s_output_buffer_initialized = false;
    if (!s_output_buffer_initialized) {
        static Arr$$(lit_num$(usize, (320*180*40)+(320*180*40/2)), u8) memory = {};
        static heap_Fixed heap ={};
        heap = heap_Fixed_init(Arr_ref$(Sli$u8, memory));
        output_buffer = type$(ArrList$u8, ArrList_init(typeInfo$(u8), heap_Fixed_allocator(&heap)));
        s_output_buffer_initialized = true;
    }

    // Pre-allocate output buffer for maximum performance
    let buffer_size = as$(usize, self->dimension.x) * as$(usize, self->dimension.y) * 40; // Larger buffer
    if (output_buffer.items.len < buffer_size) {
        catch_((ArrList_resize(
            output_buffer.base,
            buffer_size
        ))($ignore_capture, claim_unreachable));
    }

    var buffer_ptr = output_buffer.items.ptr;
    usize buffer_used = 0;
    // Clear screen and move cursor to top-left
    buffer_used += sprintf(as$(char*, buffer_ptr) + buffer_used, "\033[2J\033[H");
    // Ultra-fast pixel rendering with change detection
    for (i32 y = 0; y < self->dimension.y; ++y) {
        // Check if line has any changes first
        let line_has_changes = eval_(bool $scope) for (usize x = 0; x < as$(usize, self->dimension.x); ++x) {
            let idx = as$(usize, y) * as$(usize, self->dimension.x) + x;
            if (Sli_getAt(self->dirty_pixels, idx)) { eval_break_(true); }
        } eval_break_(false) $unscoped_eval;
        if (line_has_changes) {
            // Only render lines with changes
            for (i32 x = 0; x < self->dimension.x; ++x) {
                let idx = y * self->dimension.x + x;
                let rgb = Screen_getRGB(self, x, y);
                // Ultra-optimized ANSI sequence
                buffer_used += sprintf(as$(char*, buffer_ptr) + buffer_used, "\033[48;2;%d;%d;%dm ", rgb.r, rgb.g, rgb.b);
                // Mark as clean
                Sli_setAt(self->dirty_pixels, idx, false);
            }
            // Reset colors and newline
            buffer_used += sprintf(as$(char*, buffer_ptr) + buffer_used, "\033[0m\n");
        } else {
            // Skip unchanged lines - just add newline
            buffer_used += sprintf(as$(char*, buffer_ptr) + buffer_used, "\n");
        }
        // Safety check
        if (buffer_used > output_buffer.items.len - 200) {
            break;
        }
    }
    // Position cursor at bottom
    buffer_used += sprintf(as$(char*, buffer_ptr) + buffer_used, "\033[0m\033[%d;1H", self->dimension.y + 1);

    // Ultra-fast platform-optimized output using stream_fastOut API
    stream_fastOut(Sli_slice(output_buffer.items, (0, buffer_used)).as_const);
}


#include "dh/main.h"
#include "dh/heap/Page.h"
#include "dh/time/Instant.h"


func((dh_main(Sli$Sli_const$u8 args))(Err$void)$guard) {
    $ignore = args;

    // 스크린 버퍼 할당
    var page = make$(heap_Page);
    let allocator = heap_Page_allocator(&page);
    const usize width = 160;
    const usize height = 100;
    var screen = try_(Screen_init(m_V2i32_from(width, height), allocator));
    defer_(Screen_fini(&screen, allocator));

    // 애니메이션 루프 - HexSphere 렌더링
    f64 rotation_x = 0;
    f64 rotation_y = 0;
    let start = time_Instant_now();
    let duration = time_Duration_fromSecs_f64(10.0);
    u32 frame_count = 0;

    while (time_Duration_lt(time_Instant_elapsed(start), duration)) {
        // HexSphere 렌더링
        Screen_renderHexSphere(&screen, rotation_x, rotation_y, allocator);

        // 프레임 스킵으로 더 빠른 애니메이션 (매 2프레임마다 디스플레이)
        if (frame_count % 2 == 0) {
            Screen_display(&screen);
        }

        // 회전 업데이트 (더 빠른 회전)
        rotation_x += 0.02;
        rotation_y += 0.03;
        frame_count++;
    }

    return_ok({});
} $unguarded;
