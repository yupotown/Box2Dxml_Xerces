
#pragma once

#include <Box2D/Box2D.h>
#include <string>
#include <map>
#include <vector>
#include <utility>

namespace YPT {

class XmlWorldReader {
public:
	XmlWorldReader(b2World &world);
	virtual ~XmlWorldReader();

	// ワールドを読み込み、成功したか否かを返す
	bool Read(const std::string &file);

	// ワールドの読み込み時にワールドを初期化するか
	void SetInitializeWithReading(bool flag);
	bool GetInitializeWithReading() const;

private:
	XmlWorldReader(const XmlWorldReader &rhs);
	const XmlWorldReader &operator =(const XmlWorldReader &rhs);

	void initialize();
	void initializeWorld();

	bool strToVec2(const std::string &str);

	b2World &world;

	bool initFlag;

	std::string name;
	b2Vec2 gravity;

	typedef std::map< std::string, b2Body *> BodiesMap;
	typedef std::map< std::string, std::pair< b2Shape::Type, int > > ShapesMap;
	typedef std::map< std::string, b2FixtureDef > FixturesMap;
	BodiesMap bodies;
	ShapesMap shapes;
	FixturesMap fixtures;

	typedef std::vector< b2CircleShape > CircleShapesVec;
	typedef std::vector< b2EdgeShape > EdgeShapesVec;
	typedef std::vector< b2PolygonShape > PolygonShapesVec;
	typedef std::vector< b2ChainShape > ChainShapesVec;
	CircleShapesVec circleShapes;
	EdgeShapesVec edgeShapes;
	PolygonShapesVec polygonShapes;
	ChainShapesVec chainShapes;

	b2Vec2 resVec2;
};

} // YPT

