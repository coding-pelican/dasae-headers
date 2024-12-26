#ifndef GAME_SCREEN_INCLUDED
#define GAME_SCREEN_INCLUDED (1)

#include "../../engine/include/engine/canvas.h"
#include "State.h"

void game_Screen_renderFirstPersonView(Ptr_engine_Canvas canvas, PtrConst_game_State state);
void game_Screen_renderUi(Ptr_engine_Canvas canvas, PtrConst_game_State state);
void game_Screen_renderMinimap(Ptr_engine_Canvas canvas, PtrConst_game_State state);

#endif // GAME_SCREEN_INCLUDED
