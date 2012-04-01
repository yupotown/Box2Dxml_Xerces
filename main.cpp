
#include <iostream>
#include <Box2D/Box2D.h>
#include <SDL/SDL.h>
#include <SDL/SDL_draw.h>

#include "bodydrawer.h"
#include "fpsctrl.h"
#include "box2dxml_cfg.h"
#include "xmlworldreader.h"

int main(int argc, char *argv[]) {

	// --------------------------------------------------
	// 初期化
	// --------------------------------------------------

	// SDL初期化
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "error in SDL_Init:"
			<< SDL_GetError() << std::endl;
		return 1;
	}

	// メイン画面作成
	SDL_Surface *screen = SDL_SetVideoMode(YPT::WIDTH, YPT::HEIGHT, YPT::BPP,
		SDL_HWSURFACE);
	if (screen == NULL) {
		std::cerr << "error in SDL_SetVideoMode:"
			<< SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	// メイン画面のタイトルを設定
	SDL_WM_SetCaption(YPT::APP_NAME, YPT::APP_NAME);

	// バッファ画面作成
	SDL_Surface *bufScreen = SDL_CreateRGBSurface(
		SDL_HWSURFACE, YPT::BUF_WIDTH, YPT::BUF_HEIGHT, YPT::BPP,
		0, 0, 0, 0);
	if (bufScreen == NULL) {
		std::cerr << "error in SDL_CreateRGBSurface:"
			<< SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}
	SDL_Rect bufScreenRect = {
		static_cast< Sint16 >((bufScreen->w - screen->w) / 2),
		static_cast< Sint16 >((bufScreen->h - screen->h) / 2),
		static_cast< Uint16 >(screen->w),
		static_cast< Uint16 >(screen->h)
	};
	
	// 背景色
	Uint32 bgCol = SDL_MapRGB(bufScreen->format, 0, 0, 0);

	// Box2Dのワールドを作成
	b2World world(YPT::DEFAULT_GRAVITY);

	// 描画用クラス設定
	YPT::BodyDrawer bodyDrawer(bufScreen);
	bodyDrawer.SetScale(YPT::DEFAULT_VIEW_SCALE);
	bodyDrawer.SetSurfaceCenter(
		YPT::Position< Sint32 >(bufScreen->w / 2, bufScreen->h / 2));
	bodyDrawer.SetWorldCenter(b2Vec2(0.0f, 0.0f));

	// フレームレート制御設定
	YPT::FPSController fpsctrl(YPT::FPS);

	// ワールド読み込み
	std::string worldFile;
	YPT::XmlWorldReader worldReader(world);
	bool worldLoaded = false;
	if (argc >= 2) {
		worldFile = argv[1];
		worldLoaded = worldReader.Read(YPT::WORLDS_DIR + "/" + worldFile);
		if (!worldLoaded) {
			worldFile += ".xml";
			worldLoaded = worldReader.Read(YPT::WORLDS_DIR + "/" + worldFile);
		}
	}
	if (!worldLoaded) {
		worldFile = YPT::DEFAULT_WORLD_FILE;
		worldLoaded = worldReader.Read(YPT::WORLDS_DIR + "/" + worldFile);
	}
	if (!worldLoaded) {
		std::cerr << "loading world failed." << std::endl;
	}

	// --------------------------------------------------
	// メインループ
	// --------------------------------------------------

	// quit = true でそのフレームの終わりに終了
	for (bool quit = false; !quit;) {

		// --------------------------------------------------
		// 処理
		// --------------------------------------------------

		// SDLのイベント処理
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				quit = true;
				break;
			default:
				break;
			}
		}

		// ワールドの時間を進める
		world.Step(YPT::TIMESTEP,
			YPT::VELOCITY_ITERATIONS, YPT::POSITION_ITERATIONS);

		// --------------------------------------------------
		// 描画
		// --------------------------------------------------

		// 画面を消去
		SDL_FillRect(bufScreen, &bufScreenRect, bgCol);

		// ボディを描画
		for (b2Body *body = world.GetBodyList(); body != NULL;
			body = body->GetNext())
		{
			Uint32 col;
			if (body->GetType() == b2_staticBody) {
				col = SDL_MapRGB(bufScreen->format, 0, 0, 255);
			} else {
				col = SDL_MapRGB(bufScreen->format, 255, 0, 0);
			}

			bodyDrawer.Draw(body, col);
		}

		// バッファ画面からメイン画面へコピー
		SDL_BlitSurface(bufScreen, &bufScreenRect, screen, NULL);

		// メイン画面を更新
		SDL_UpdateRect(screen, 0, 0, 0, 0);

		// --------------------------------------------------
		// フレームレート制御
		// --------------------------------------------------

		fpsctrl.Update();

	}

	// --------------------------------------------------
	// 終了
	// --------------------------------------------------

	// SDL終了
	SDL_Quit();

	return 0;
}

