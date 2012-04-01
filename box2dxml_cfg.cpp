
#include "box2dxml_cfg.h"
#include <Box2D/Box2D.h>
#include <SDL/SDL.h>

namespace YPT {

// ソフトウェア名
char const * const APP_NAME = "Box2Dxml";

// 画面の大きさと色深度
int const WIDTH = 640;
int const HEIGHT = 480;
int const BPP = 32;
int const BUF_WIDTH = WIDTH * 2;
int const BUF_HEIGHT = HEIGHT * 2;

// フレームレート、1フレームあたりの秒数
int const FPS = 60;
float32 const TIMESTEP = 1.0f / FPS;

// Box2Dのイテレーション数
int32 const VELOCITY_ITERATIONS = 10;
int32 const POSITION_ITERATIONS = 10;

// デフォルト設定
b2Vec2 const DEFAULT_GRAVITY(0.0f, -9.8f);
float const DEFAULT_VIEW_SCALE = 48.0f;
std::string const DEFAULT_WORLD_FILE("default.xml");

// ディレクトリ
std::string const WORLDS_DIR("./worlds");

} // YPT

