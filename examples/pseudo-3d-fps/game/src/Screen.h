#ifndef GAME_SCREEN_INCLUDED
#define GAME_SCREEN_INCLUDED (1)

#include "../../engine/include/engine/canvas.h"
#include "State.h"

extern void game_Screen_renderFirstPersonView(engine_Canvas* canvas, const game_State* state);
extern void game_Screen_renderUi(engine_Canvas* canvas, const game_State* state);
extern void game_Screen_renderMinimap(engine_Canvas* canvas, const game_State* state);

#endif // GAME_SCREEN_INCLUDED
