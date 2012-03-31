
#pragma once

#include <SDL/SDL.h>
#include <Box2D/Box2D.h>
#include "shapedrawer.h"
#include "position.hpp"
/*
#include <Box2D/Collision/Shapes/b2Shape.h>
#include <Box2D/Collision/Shapes/b2CircleShape.h>
#include <Box2D/Collision/Shapes/b2EdgeShape.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Collision/Shapes/b2ChainShape.h>
*/

namespace YPT {

class BodyDrawer {
public:
	BodyDrawer(SDL_Surface *dest);
	virtual ~BodyDrawer();

	void SetScale(float scale);
	float GetScale() const;

	void SetSurfaceCenter(const Position< Sint32 > &c);
	const Position< Sint32 > &GetSurfaceCenter() const;
		// 返り値はコピーして保存すること

	void SetWorldCenter(const b2Vec2 &c);
	const b2Vec2 &GetWorldCenter() const;	// 返り値はコピーして保存すること

	// Y軸が上向きならtrueにする
	void SetReverse(bool r);
	bool IsReverse() const;

	void Draw(const b2Body *body, Uint32 col);

	// ワールド上の位置から実際に表示される位置を計算
	const Position< Sint32 > &WorldPosToViewPos(const b2Vec2 &pos);
		// 返り値はコピーして保存すること

	// 実際に表示される位置からワールド上の位置を逆算
	const b2Vec2 &ViewPosToWorldPos(const Position< Sint32 > &pos);
		// 返り値はコピーして保存すること
	const b2Vec2 &ViewPosToWorldPos(Sint32 x, Sint32 y);
		// 返り値はコピーして保存すること

private:
	BodyDrawer(const BodyDrawer &rhs);
	const BodyDrawer &operator =(const BodyDrawer &rhs);

	SDL_Surface *dest;
	float scale;
	Position< Sint32 > sc;
	//b2Vec2 wc;
	bool reverse;

	::YPT::ShapeDrawer shapeDrawer;

	b2Vec2 temp;
	Position< Sint32 > temp2;
};

} // YPT

