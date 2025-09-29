#ifndef planet_common_included
#define planet_common_included (1)

#include "dh/prl.h"
#include "dh/math/vec.h"

typedef struct planet_Flow planet_Flow;
struct planet_Flow {
    m_V2f64 velocity;
} static const planet_Flow_default = {
    .velocity = m_V2f64_zero,
};
extern fn_(planet_Flow_strength(planet_Flow self), f64);


typedef enum planet_TectonicPlateType planet_TectonicPlateType;
enum planet_TectonicPlateType {
    planet_TectonicPlateType_oceanic     = 0,
    planet_TectonicPlateType_continental = 1,
};
typedef struct planet_TectonicPlate planet_TectonicPlate;
struct planet_TectonicPlate {
    planet_Flow              flow;
    f64                      density; // Affects subduction
    planet_TectonicPlateType type;    // 0=oceanic, 1=continental
} static const planet_TectonicPlate_default = {
    .flow    = planet_Flow_default,
    .type    = planet_TectonicPlateType_oceanic,
    .density = 0.0,
};
use_Sli$(planet_TectonicPlate);

typedef struct planet_Tectonic planet_Tectonic;
struct planet_Tectonic {
    Sli$planet_TectonicPlate plates;
};


typedef struct planet_Atmosphere planet_Atmosphere;
struct planet_Atmosphere {
    f64 nitrogen;
    f64 oxygen;
    f64 co2;
    f64 methane;
    f64 water_vapor;
    f64 argon;
    f64 pressure;
} static const planet_Atmosphere_default = {
    .nitrogen    = 0.78,
    .oxygen      = 0.01,
    .co2         = 0.10,
    .methane     = 0.05,
    .water_vapor = 0.05,
    .argon       = 0.01,
    .pressure    = 1.0,
};
static let planet_Atmosphere_early_earth = planet_Atmosphere_default;

typedef struct planet_Climate planet_Climate;
struct planet_Climate {
    planet_Atmosphere atmosphere;
    f64               solar_constant;
    f64               avg_temp;
    f64               albedo;
};


typedef struct planet_World planet_World;
struct planet_World {
    planet_Tectonic tectonic;
    planet_Climate  climate;
};

#endif /* planet_common_included */

/*
지질계(Geosphere)
명칭
영어
설명
비고
화산활동
Volcano
화산활동의 정도
↑ : 육지 생물 멸종
풍화
Erosion
비나 바람으로 지형을 깎는 정도
↓ : 고저차 심화
유성충돌
Meteor
유성 충돌의 정도
↑ : 생물 멸종
지각이동
Continental Drift
지각의 이동 정도
마그마 이동의 영향을 받음
내부열
Core Heat
핵에서 나오는 열
↑ : 온실효과
↓ : 대동결
핵형성
Core Formation
핵의 크기 조절
↑ : 화산활동 저하
↓ : 화산활동 활발
자전축 기울기
Axial Tilt
자전축의 기울기
↑ : 생물 멸종
↓ : 계절변화 없어짐

대기계(Atmosphere)
태양열
Solar Impact
태양열이 들어오는 정도
↑ : 온실효과
↓ : 대동결
구름 반사율
Cloud Albedo
구름이 태양열을 반사하는 정도
↑ : 대동결
온실효과
Greenhouse Effect
온실효과의 정도
↑ : 사막화
↓ : 대동결
구름
Cloud
구름의 양
구름 반사율을 조절하기도 함
강수
Rainfall
평균 강수량
↑ : 온실효과
↓ : 사막화
표면 반사율
Surface Albedo
표면이 태양열을 반사하는 정도
↓ : 온실효과
온도 순환
Thermal Transfer
물의 순환에 따른 온도 순환의 양
*/
