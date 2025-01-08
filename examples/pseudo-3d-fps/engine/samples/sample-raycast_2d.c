// TODO: Port to dh-engine

#include <float.h>

#define DCE_TEST
#include "dce/dce.h"


Circle circles[] = {
    { -20, 0, 4 },
    { -40, 10, 6 },
    { -30, 30, 14 },
    { 10, 40, 20 },
    { 40, -30, 14 },
    { 0, -20, 4 },
    { -30, -40, 20 }
};
int circleCount = sizeof(circles) / sizeof(*circles);

void Ray_cast(PhysRay this, int depth, int depthMax) {
    if (depth == depthMax) {
        Screen_DrawVector2D(this.position, V2.sub(Ray_v2EndPointOf(this), this.position), 1, DCE_WHITE);
    }
    TimeSecond minTime = FLT_MAX;
    RayHit     bestHit = NewRayHit();
    for (int i = 0; i < circleCount; ++i) {
        RayHit hit;
        bool   hits = Ray_RaycastsCircle(this, circles[i], &hit);
        if (hits && depth < depthMax) {
            if (hit.impactTime < minTime) {
                minTime = hit.impactTime;
                bestHit = hit;
            }
        }
    }
    if (minTime == FLT_MAX) {
        Screen_DrawVector2D(this.position, V2.sub(Ray_v2EndPointOf(this), this.position), 1, DCE_WHITE);
        return;
    }
    Vec2 hitPos = Ray_v2Impact(this, bestHit);
    Screen_DrawLine(this.position, hitPos, DCE_WHITE);
    this.direction = Ray_v2Reflect(this.direction, bestHit.surfaceNormal);
    this.position  = hitPos;
    this.travelTime -= bestHit.impactTime;
    Ray_cast(this, depth + 1, depthMax);
}

int main(void) {
    dce_Initialize();
    getch();

    TimeSecond heldTime = 0;

    while (dce_IsRunning()) {
        Screen_Clear(DCE_BLACK);

        for (int i = 0; i < circleCount; i += 2) {
            Screen_DrawCircle(circles[i], DCE_WHITE);
        }
        for (int i = 1; i < circleCount; i += 2) {
            Screen_FillCircle(circles[i], DCE_WHITE);
        }

        bool isHeldMouseLeft     = Input_GetMouse(DCE_BTN_LT).isHeld;
        bool isPressedMouseRight = Input_GetMouse(DCE_BTN_RT).isPressed;

        if (isHeldMouseLeft) {
            heldTime += Time_DeltaTime();
        }

        if (isPressedMouseRight) {
            heldTime = 0.0f;
        }

        if (heldTime > 0.0f) {
            Vec2    mousePos = Input_GetMousePosScreenToWorld();
            PhysRay ray      = MakeRay(
                V2.new(),
                V2.normSafe(mousePos),
                // 12.5f + (cosf(t * 2.0f) + 1.0f) * 0.5f * 250.0f
                heldTime * 0.5f * 250.0f
            );
            Ray_cast(ray, 0, 100);
            if (!isHeldMouseLeft) {
                heldTime -= Time_DeltaTime();
            }
        }

        if (Input_GetKey(DCE_ESC).isPressed) {
            dce_Exit();
        }
    }

    return 0;
}
