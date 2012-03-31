
#pragma once

#include <Box2D/Box2D.h>
#include <SDL/SDL.h>

namespace YPT {

extern char const * const APP_NAME;

extern int const WIDTH;
extern int const HEIGHT;
extern int const BPP;
extern int const BUF_WIDTH;
extern int const BUF_HEIGHT;

extern int const FPS;
extern float32 const TIMESTEP;

extern int32 const VELOCITY_ITERATIONS;
extern int32 const POSITION_ITERATIONS;

extern b2Vec2 const DEFAULT_GRAVITY;
extern float const DEFAULT_VIEW_SCALE;

} // YPT

