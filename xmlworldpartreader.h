
#pragma once

#include <xercesc/dom/DOMDocument.hpp>
#include <Box2D/Box2D.h>
#include <vector>

namespace YPT {

class XmlWorldPartReader {
public:
	XmlWorldPartReader();
	XmlWorldPartReader(xercesc::DOMDocument *doc);
	virtual ~XmlWorldPartReader();

	b2Body *ReadBody(b2World &world, const xercesc::DOMNode *bodyNode,
		const std::vector< b2FixtureDef *> &fixtureDefs);
	bool ReadFixture(const xercesc::DOMNode *fixtureNode,
		b2Shape *shape, b2FixtureDef *dest);

	bool ReadCircleShape(const xercesc::DOMNode *shapeNode, b2CircleShape *dest);
	b2CircleShape ReadCircleShape(const xercesc::DOMNode *shapeNode);
	bool ReadEdgeShape(const xercesc::DOMNode *shapeNode, b2EdgeShape *dest);
	b2EdgeShape ReadEdgeShape(const xercesc::DOMNode *shapeNode);
	bool ReadPolygonShape(const xercesc::DOMNode *shapeNode, b2PolygonShape *dest);
	b2PolygonShape ReadPolygonShape(const xercesc::DOMNode *shapeNode);
	bool ReadChainShape(const xercesc::DOMNode *shapeNode, b2ChainShape *dest);
	b2ChainShape ReadChainShape(const xercesc::DOMNode *shapeNode);

private:
	XmlWorldPartReader(const XmlWorldPartReader &rhs);
	const XmlWorldPartReader &operator =(const XmlWorldPartReader &rhs);

	xercesc::DOMDocument *doc;
};

} // YPT

