
/*
 * Note: Surfaceから一部でもはみ出すものは描画されません。
 */

#pragma once

#include <SDL/SDL.h>
#include <SDL/SDL_draw.h>
#include <Box2D/Box2D.h>

namespace YPT {

class ShapeDrawer {
public:
	ShapeDrawer(SDL_Surface *dest);
	virtual ~ShapeDrawer();

	void SetScale(float scale);
	float GetScale() const;

	void SetAngle(float32 angle);
	float32 GetAngle() const;

	void SetCenterOfMass(const b2Vec2 &c);
	const b2Vec2 &GetCenterOfMass() const;	// 返り値はコピーして保存すること

	void SetWorldCenter(const b2Vec2 &c);
	const b2Vec2 &GetWorldCenter() const;	// 返り値はコピーして保存すること

	void SetReverse(bool r);
	bool IsReverse() const;

	void Draw(const b2Shape *shape, Uint32 col, Sint32 offx = 0, Sint32 offy = 0);

	void Draw(const b2CircleShape *shape, Uint32 col, Sint32 offx = 0, Sint32 offy = 0);
	void Draw(const b2EdgeShape *shape, Uint32 col, Sint32 offx = 0, Sint32 offy = 0);
	void Draw(const b2PolygonShape *shape, Uint32 col, Sint32 offx = 0, Sint32 offy = 0);
	void Draw(const b2ChainShape *shape, Uint32 col, Sint32 offx = 0, Sint32 offy = 0);

private:
	ShapeDrawer(const ShapeDrawer &rhs);
	const ShapeDrawer &operator =(const ShapeDrawer &rhs);

	SDL_Surface *dest;
	float scale;
	float32 angle;
	b2Vec2 center;
	b2Vec2 wc;
	bool reverse;
};

} // YPT

