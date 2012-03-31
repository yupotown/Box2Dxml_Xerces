
#include "fpsctrl.h"

namespace YPT {

FPSController::FPSController(int fps)
	:fps(fps),
	realFps(0),
	nextTicks(0),
	remainingFrames(0)
{
	if (this->fps <= 0) {
		if (this->fps == 0)
			this->fps = 60;
		else
			this->fps = 1;
	}
	// 大きすぎる場合の対処は特にしない

	this->Initialize();
}

FPSController::~FPSController() {
}

int FPSController::GetFPS() const {
	return realFps;
}

int FPSController::GetRequestedFPS() const {
	return fps;
}

void FPSController::Initialize() {
	realFps = 0;
	nextTicks = SDL_GetTicks() + 1000;
	remainingFrames = fps;
}

void FPSController::Update() {
	Uint32 nowTicks = SDL_GetTicks();
	if (nowTicks >= nextTicks) {
		realFps = fps - remainingFrames;
		remainingFrames = fps;
		nextTicks = nowTicks + 1000;
	}
	int waitTime = static_cast< float >(nextTicks - nowTicks + 4) / remainingFrames;
	if (waitTime > 0)
		SDL_Delay(waitTime);
	--remainingFrames;
}

} // YPT

