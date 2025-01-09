// TODO: Port to dh-engine

#include "dce/dce.h"

int main(void) {
    dce_Initialize();
    getch();

    while (dce_IsRunning()) {
        Screen_Clear(DCE_BLACK);

        Vec2     o = V2.new();
        Vec2     p = V2.make(50.0f, 0);
        Rotation r = Rot.sincos(Time_Now());
        p          = Rot.v2MulV2(r, p);
        Screen_DrawVector2D(o, p, 1.0f, DCE_WHITE);

        if (Input_GetKey(DCE_ESC).isPressed) {
            dce_Exit();
        }
    }

    return 0;
}
