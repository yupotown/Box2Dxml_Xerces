
#include "shapedrawer.h"
#include <cmath>

namespace YPT {

ShapeDrawer::ShapeDrawer(SDL_Surface *dest)
	:dest(dest),
	scale(1.0f),
	angle(0.0f),
	center(0.0f, 0.0f),
	wc(0.0f, 0.0f),
	reverse(true)
{
}

ShapeDrawer::~ShapeDrawer() {
}

void ShapeDrawer::SetScale(float scale) {
	this->scale = scale;
}

float ShapeDrawer::GetScale() const {
	return this->scale;
}

void ShapeDrawer::SetAngle(float32 angle) {
	this->angle = angle;
}

float32 ShapeDrawer::GetAngle() const {
	return this->angle;
}

void ShapeDrawer::SetCenterOfMass(const b2Vec2 &c) {
	this->center = c;
}

const b2Vec2 &ShapeDrawer::GetCenterOfMass() const {
	return this->center;
}

void ShapeDrawer::SetWorldCenter(const b2Vec2 &c) {
	this->wc = c;
}

const b2Vec2 &ShapeDrawer::GetWorldCenter() const {
	return this->wc;
}

void ShapeDrawer::SetReverse(bool r) {
	this->reverse = r;
}

bool ShapeDrawer::IsReverse() const {
	return this->reverse;
}

void ShapeDrawer::Draw(const b2Shape *shape, Uint32 col,
	Sint32 offx, Sint32 offy)
{
	switch (shape->GetType()) {
	case b2Shape::e_circle:
		this->Draw(reinterpret_cast< const b2CircleShape *>(shape),
			col, offx, offy);
		break;
	case b2Shape::e_edge:
		this->Draw(reinterpret_cast< const b2EdgeShape *>(shape),
			col, offx, offy);
		break;
	case b2Shape::e_polygon:
		this->Draw(reinterpret_cast< const b2PolygonShape *>(shape),
			col, offx, offy);
		break;
	case b2Shape::e_chain:
		this->Draw(reinterpret_cast< const b2ChainShape *>(shape),
			col, offx, offy);
		break;
	default:
		break;
	}
}

void ShapeDrawer::Draw(const b2CircleShape *shape, Uint32 col,
	Sint32 offx, Sint32 offy)
{
	// 中心 (x, y)
	Sint32 x = offx + (shape->m_p.x - wc.x) * scale,
		y = reverse
			? offy - (shape->m_p.y - wc.y) * scale
			: offy + (shape->m_p.y - wc.y) * scale;
	// 半径 r
	// 後の演算のためsigned
	Sint32 r = shape->m_radius * scale;
	// 直線 (x, y) - (lx, ly)
	Sint32 lx = x + (shape->m_radius - 2.0f / scale) * std::cos(angle) * scale,
		ly = y + (shape->m_radius - 2.0f / scale) * (
			reverse
				? -std::sin(angle)
				: std::sin(angle)
			) * scale;
	if (x-r >= 0 && x+r < dest->w && y-r >= 0 && y+r < dest->h) {
		Draw_Circle(dest, x, y, r, col);
		Draw_Line(dest, x, y, lx, ly, col); // 暗黙の変換:signed -> unsigned
	}
}

void ShapeDrawer::Draw(const b2EdgeShape *shape, Uint32 col,
	Sint32 offx, Sint32 offy)
{
	const b2Vec2 &a = shape->m_vertex1, &b = shape->m_vertex2;
	float s = std::sin(angle), c = std::cos(angle);
	Sint32 x1 = offx + (a.x * c - a.y * s - wc.x) * scale,
		y1 = reverse
			? offy - (a.x * s + a.y * c - wc.y) * scale
			: offy + (a.x * s + a.y * c - wc.y) * scale,
		x2 = offx + (b.x * c - b.y * s - wc.x) * scale,
		y2 = reverse
			? offy - (b.x * s + b.y * c - wc.y) * scale
			: offy + (b.x * s + b.y * c - wc.y) * scale;
	if (x1 >= 0 && x1 < dest->w && y1 >= 0 && y1 < dest->h &&
		x2 >= 0 && x2 < dest->w && y2 >= 0 && y2 < dest->h)
	{
		Draw_Line(dest, x1, y1, x2, y2, col);
	}
}


void ShapeDrawer::Draw(const b2PolygonShape *shape, Uint32 col,
	Sint32 offx, Sint32 offy)
{
	// すべて範囲チェックしてから描画するため一旦配列に保存する
	const int32 vertexCount = shape->GetVertexCount();
	struct Pos {
		Sint32 x, y;
	};
	Pos *vertices = new Pos[vertexCount]; // shared_ptrで配列は扱えない
	float s = std::sin(angle), c = std::cos(angle);

	// 頂点の取得と範囲チェック
	for (int i = 0; i < vertexCount; ++i) {
		const b2Vec2 &vertex = shape->GetVertex(i);
		vertices[i].x = offx + (vertex.x * c - vertex.y * s- wc.x) * scale;
		vertices[i].y = reverse
			? offy - (vertex.x * s + vertex.y * c - wc.y) * scale
			: offy + (vertex.x * s + vertex.y * c - wc.y) * scale;
		if (vertices[i].x < 0 || vertices[i].x >= dest->w ||
			vertices[i].y < 0 || vertices[i].y >= dest->h)
		{
			delete[] vertices;
			return;
		}
	}

	// 描画
	for (int i = 0; i < vertexCount; ++i) {
		int temp = (i+1) % vertexCount;
		Sint32 x1 = vertices[i].x,
			y1 = vertices[i].y,
			x2 = vertices[temp].x,
			y2 = vertices[temp].y;
		Draw_Line(dest, x1, y1, x2, y2, col);
	}

	delete[] vertices;
}

void ShapeDrawer::Draw(const b2ChainShape *shape, Uint32 col,
	Sint32 offx, Sint32 offy)
{
	const int32 vertexCount = shape->m_count;
	struct Pos {
		Sint32 x, y;
	};
	Pos *vertices = new Pos[vertexCount];
	float s = std::sin(angle), c = std::cos(angle);

	// 頂点の取得と範囲チェック
	for (int i = 0; i < vertexCount; ++i) {
		float32 &x = shape->m_vertices[i].x,
			&y = shape->m_vertices[i].y;	// 名前が長いので参照に置き換え
		vertices[i].x = offx + (x * c - y * s - wc.x) * scale;
		vertices[i].y = reverse
			? offy - (x * s + y * c - wc.y) * scale
			: offy + (x * s + y * c - wc.y) * scale;
		if (vertices[i].x < 0 || vertices[i].x >= dest->w ||
			vertices[i].y < 0 || vertices[i].y >= dest->h)
		{
			delete[] vertices;
			return;
		}
	}

	// 描画
	for (int i = 1; i < vertexCount; ++i) {
		Draw_Line(dest, vertices[i-1].x, vertices[i-1].y,
			vertices[i].x, vertices[i].y, col);
	}

	delete[] vertices;
}

} // YPT

