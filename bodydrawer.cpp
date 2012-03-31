
#include "bodydrawer.h"

namespace YPT {

BodyDrawer::BodyDrawer(SDL_Surface *dest)
	:dest(dest),
	scale(1.0f),
	sc(dest->w / 2, dest->h / 2),
	reverse(true),
	shapeDrawer(dest)
{}

BodyDrawer::~BodyDrawer() {
}

void BodyDrawer::SetScale(float scale) {
	this->scale = scale;
	shapeDrawer.SetScale(scale);
}

float BodyDrawer::GetScale() const {
	return this->scale;
}

void BodyDrawer::SetSurfaceCenter(const Position< Sint32 > &c) {
	this->sc = c;
}

const Position< Sint32 > &BodyDrawer::GetSurfaceCenter() const {
	return this->sc;
}

void BodyDrawer::SetWorldCenter(const b2Vec2 &c) {
	//this->wc = c;
	shapeDrawer.SetWorldCenter(c);
}

const b2Vec2 &BodyDrawer::GetWorldCenter() const {
	//return this->wc;
	return shapeDrawer.GetWorldCenter();
}

void BodyDrawer::SetReverse(bool r) {
	this->reverse = r;
	shapeDrawer.SetReverse(r);
}

bool BodyDrawer::IsReverse() const {
	return this->reverse;
}

void BodyDrawer::Draw(const b2Body *body, Uint32 col) {
	Sint32 ox, oy;	// 表示オフセット
	{
		const b2Vec2 &bodyPos = body->GetPosition();
		ox = bodyPos.x * scale + sc.x;
		oy = reverse
			? dest->h - (bodyPos.y * scale + sc.y)
			: bodyPos.y * scale + sc.y;
	}

	shapeDrawer.SetAngle(body->GetAngle());

	for (const b2Fixture *fixture = body->GetFixtureList();
		fixture != NULL; fixture = fixture->GetNext())
	{
		const b2Shape *shape = fixture->GetShape();
		shapeDrawer.Draw(shape, col, ox, oy);
	}
}

const Position< Sint32 > &BodyDrawer::WorldPosToViewPos(const b2Vec2 &pos) {
	temp2.x = pos.x * scale + sc.x;
	temp2.y = reverse
		? -pos.y * scale + sc.y
		: pos.y * scale + sc.y;
	return temp2;
}

const b2Vec2 &BodyDrawer::ViewPosToWorldPos(const Position< Sint32 > &pos) {
	return ViewPosToWorldPos(pos.x, pos.y);
}

const b2Vec2 &BodyDrawer::ViewPosToWorldPos(Sint32 x, Sint32 y) {
	// x = res.x * scale + sc.x
	// y = -res.y * scale + sc.y (if reverse == true)
	//     res.y * scale + sc.y  (otherwise)
	temp.Set((x - sc.x) / scale, reverse
		? -(y - sc.y) / scale
		: (y - sc.y) / scale);
	return temp;
}

/*
void BodyDrawer::Draw(const b2Shape *shape, Uint32 col,
	const b2Vec2 &offset, float32 angle)
{
	switch (shape->GetType()) {
	case b2Shape::e_circle:
		this->Draw(reinterpret_cast< const b2CircleShape *>(shape),
			col, offset);
		break;
	case b2Shape::e_edge:
		this->Draw(reinterpret_cast< const b2EdgeShape *>(shape),
			col, offset);
		break;
	case b2Shape::e_polygon:
		this->Draw(reinterpret_cast< const b2PolygonShape *>(shape),
			col, offset);
		break;
	case b2Shape::e_chain:
		this->Draw(reinterpret_cast< const b2ChainShape *>(shape),
			col, offset);
		break;
	default:
		break;
	}
}

void BodyDrawer::Draw(const b2CircleShape *shape, Uint32 col, const b2Vec2 &offset) {
	Sint16 ox = offset.x * scale + sc.x,
		oy = reverse
			? dest->h - (offset.y * scale + sc.y)
			: offset.y * scale + sc.y;
	Sint16 x = ox + shape->m_p.x * scale,
		y = reverse
			? -(shape->m_p.y) * scale
			: shape->m_p.y * scale;
	Uint16 r = shape->m_radius * scale;
	Sint16 lx = x + (shape->m_radius - 0.1f) * std::cos(angle) * scale;
}

void BodyDrawer::Draw(const b2EdgeShape *shape, Uint32 col), const b2Vec2 &offset {
}

void BodyDrawer::Draw(const b2PolygonShape *shape, Uint32 col), const b2Vec2 &offset {
}

void BodyDrawer::Draw(const b2ChainShape *shape, Uint32 col), const b2Vec2 &offset {
}
*/

} // YPT

