// TODO: Port to dh-engine

#include "dce/dce.h"

int main(void) {
    dce_Initialize();
    getch();

    while (dce_IsRunning()) {
        Screen_Clear(DCE_BLACK);

        Vec2 a = V2.make(-50, 0);
        Vec2 b = V2.make(-50, 50);
        Vec2 c = V2.make(50, 25);
        Vec2 d = V2.make(100, -50);

        float t0 = 0.0f;
        for (int i = 0; i <= 30; ++i) {
            float t1 = (float)i / 30.0f;
            Vec2  p0 = V2.bezierCubic(a, b, c, d, t0);
            Vec2  p1 = V2.bezierCubic(a, b, c, d, t1);
            Screen_DrawLine(p0, p1, DCE_WHITE);
            t0 = t1;
        }

        float pt        = (Cos(Time_Now()) + 1.0f) * 0.5f;
        Vec2  boxCenter = V2.bezierCubic(a, b, c, d, pt);
        Screen_DrawBox(
            MakeAABB(
                V2.sub(boxCenter, V2.make(2.5f, 2.5f)),
                V2.add(boxCenter, V2.make(2.5f, 2.5f))
            ),
            DCE_WHITE
        );

        if (Input_GetKey(DCE_ESC).isPressed) {
            dce_Exit();
        }
    }

    return 0;
}
