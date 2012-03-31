
#pragma once

#include <SDL/SDL.h>

namespace YPT {

class FPSController {
public:
	FPSController(int fps);
	virtual ~FPSController();

	int GetFPS() const;
	int GetRequestedFPS() const;

	void Initialize();
	void Update();

private:
	FPSController(const FPSController &rhs);
	const FPSController &operator =(const FPSController &rhs);

	int fps;
	int realFps;
	Uint32 nextTicks;
	int remainingFrames;
};

} // YPT

