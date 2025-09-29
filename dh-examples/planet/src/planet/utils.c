#include "planet/utils.h"

fn_(planet_utils_generatePerlinNoise$f64(f64 x, f64 y), f64) {
    let n0 = as$(isize, (x * 57.0 + y * 131.0));
    let n1 = (n0 << 13) ^ n0;
    return 1.0 - as$(f64, (n1 * (n1 * n1 * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0;
}

fn_(planet_utils_generatePerlinNoise$V2f64(m_V2f64 p), f64) {
    return planet_utils_generatePerlinNoise$f64(p.x, p.y);
}
