
#include "xmlworldpartreader.h"
#include <string>
#include "convert.h"
#include "constant.h"
#include <xercesc/dom/DOM.hpp>
#include "xmlstring.hpp"

namespace YPT {

using namespace xercesc;

XmlWorldPartReader::XmlWorldPartReader()
	:doc(NULL)
{
}

XmlWorldPartReader::XmlWorldPartReader(DOMDocument *doc)
	:doc(doc)
{
}

XmlWorldPartReader::~XmlWorldPartReader() {
}

b2Body *XmlWorldPartReader::ReadBody(
	b2World &world, const DOMNode *bodyNode,
	const std::vector< b2FixtureDef *> &fixtureDefs
) {
	DOMXPathNSResolver *resolver = doc->createNSResolver(bodyNode);
	if (resolver == nullptr) {
		return nullptr;
	}

	b2BodyDef bodyDef;
	bodyDef.position.SetZero();
	bodyDef.angle = 0.0f;
	bodyDef.fixedRotation = false;
	bodyDef.type = b2_dynamicBody;
	bodyDef.gravityScale = 1.0f;

	DOMXPathResult *result;
	YPT::XmlString str;

	// position
	result = doc->evaluate(
		YPT::XmlString("./position"), bodyNode, resolver,
		DOMXPathResult::ORDERED_NODE_SNAPSHOT_TYPE,
		NULL
	);
	if (result != nullptr) {
		if (result->getSnapshotLength() >= 1) {
			str = result->getNodeValue()->getTextContent();
			YPT::ConvertStrToVec2(std::string(str), &bodyDef.position);
		}
		result->release();
	}

	// angle
	result = doc->evaluate(
		YPT::XmlString("./angle"), bodyNode, resolver,
		DOMXPathResult::ORDERED_NODE_SNAPSHOT_TYPE,
		NULL
	);
	if (result != nullptr) {
		if (result->getSnapshotLength() >= 1) {
			str = result->getNodeValue()->getTextContent();
			float temp;
			if (YPT::ConvertStrToFloat(std::string(str), &temp)) {
				// 度からラジアンへ変換
				bodyDef.angle = temp * YPT::PI / 180;
			}
		}
		result->release();
	}

	// fixedRotation
	result = doc->evaluate(
		YPT::XmlString("./fixedRotation"), bodyNode, resolver,
		DOMXPathResult::ORDERED_NODE_SNAPSHOT_TYPE,
		NULL
	);
	if (result != nullptr) {
		if (result->getSnapshotLength() >= 1) {
			str = result->getNodeValue()->getTextContent();
			YPT::ConvertStrToBool(std::string(str), &bodyDef.fixedRotation);
		}
		result->release();
	}

	// type
	result = doc->evaluate(
		YPT::XmlString("./type"), bodyNode, resolver,
		DOMXPathResult::ORDERED_NODE_SNAPSHOT_TYPE,
		NULL
	);
	if (result != nullptr) {
		if (result->getSnapshotLength() >= 1) {
			str = result->getNodeValue()->getTextContent();
			if (str == "dynamic") {
				bodyDef.type = b2_dynamicBody;
			} else if (str == "static") {
				bodyDef.type = b2_staticBody;
			} else if (str == "kinematic") {
				bodyDef.type = b2_kinematicBody;
			} else {
				// 未対応
			}
		}
		result->release();
	}

	// gravityScale
	result = doc->evaluate(
		YPT::XmlString("./gravityScale"), bodyNode, resolver,
		DOMXPathResult::ORDERED_NODE_SNAPSHOT_TYPE,
		NULL
	);
	if (result != nullptr) {
		if (result->getSnapshotLength() >= 1) {
			str = result->getNodeValue()->getTextContent();
			YPT::ConvertStrToFloat(std::string(str), &bodyDef.gravityScale);
		}
		result->release();
	}

	b2Body *body = world.CreateBody(&bodyDef);
	if (body == NULL) {
		return NULL;
	}

	for (std::vector< b2FixtureDef *>::const_iterator it
		= fixtureDefs.begin(); it != fixtureDefs.end(); ++it)
	{
		if (*it != NULL)
			body->CreateFixture(*it);
	}

	return body;
}

bool XmlWorldPartReader::ReadFixture(const DOMNode *fixtureNode,
	b2Shape *shape, b2FixtureDef *dest)
{
	DOMXPathNSResolver *resolver = doc->createNSResolver(fixtureNode);
	if (resolver == nullptr) {
		return nullptr;
	}

	DOMXPathResult *result;
	YPT::XmlString str;

	b2FixtureDef fixtureDef;
	fixtureDef.friction = 0.5f;
	fixtureDef.restitution = 0.5f;
	fixtureDef.density = 1.0f;

	// shape
	fixtureDef.shape = shape;

	// friction
	result = doc->evaluate(
		YPT::XmlString("./friction"), fixtureNode, resolver,
		DOMXPathResult::ORDERED_NODE_SNAPSHOT_TYPE,
		NULL
	);
	if (result != nullptr) {
		if (result->getSnapshotLength() >= 1) {
			str = result->getNodeValue()->getTextContent();
			YPT::ConvertStrToFloat(std::string(str), &fixtureDef.friction);
		}
		result->release();
	}

	// restitution
	result = doc->evaluate(
		YPT::XmlString("./restitution"), fixtureNode, resolver,
		DOMXPathResult::ORDERED_NODE_SNAPSHOT_TYPE,
		NULL
	);
	if (result != nullptr) {
		if (result->getSnapshotLength() >= 1) {
			str = result->getNodeValue()->getTextContent();
			YPT::ConvertStrToFloat(std::string(str), &fixtureDef.restitution);
		}
		result->release();
	}

	// density
	result = doc->evaluate(
		YPT::XmlString("./density"), fixtureNode, resolver,
		DOMXPathResult::ORDERED_NODE_SNAPSHOT_TYPE,
		NULL
	);
	if (result != nullptr) {
		if (result->getSnapshotLength() >= 1) {
			str = result->getNodeValue()->getTextContent();
			YPT::ConvertStrToFloat(std::string(str), &fixtureDef.density);
		}
		result->release();
	}

	*dest = fixtureDef;

	return true;
}

bool XmlWorldPartReader::ReadCircleShape(
	const DOMNode *shapeNode, b2CircleShape *dest
) {
	DOMXPathNSResolver *resolver = doc->createNSResolver(shapeNode);
	if (resolver == nullptr) {
		return nullptr;
	}

	DOMXPathResult *result;
	YPT::XmlString str;

	b2Vec2 center(0.0f, 0.0f);
	float32 radius;

	result = doc->evaluate(
		YPT::XmlString("./center"), shapeNode, resolver,
		DOMXPathResult::ORDERED_NODE_SNAPSHOT_TYPE,
		NULL
	);
	if (result != nullptr) {
		if (result->getSnapshotLength() >= 1) {
			str = result->getNodeValue()->getTextContent();
			YPT::ConvertStrToVec2(std::string(str), &center);
		}
		result->release();
	}

	result = doc->evaluate(
		YPT::XmlString("./radius"), shapeNode, resolver,
		DOMXPathResult::ORDERED_NODE_SNAPSHOT_TYPE,
		NULL
	);
	if (result == nullptr) {
		return false;
	}
	if (result->getSnapshotLength() == 0) {
		// 値が存在しない
		result->release();
		return false;
	}
	str = result->getNodeValue()->getTextContent();
	if (!YPT::ConvertStrToFloat(std::string(str), &radius)) {
		// 値が正しい形式で書かれていない
		return false;
	}
	result->release();

	dest->m_p = center;
	dest->m_radius = radius;

	return true;
}

b2CircleShape XmlWorldPartReader::ReadCircleShape(
	const DOMNode *shapeNode
) {
	b2CircleShape temp;
	ReadCircleShape(shapeNode, &temp);
	return temp;
}

bool XmlWorldPartReader::ReadEdgeShape(
	const DOMNode *shapeNode, b2EdgeShape *dest
) {
	DOMXPathNSResolver *resolver = doc->createNSResolver(shapeNode);
	if (resolver == nullptr) {
		return nullptr;
	}

	DOMXPathResult *result;
	YPT::XmlString str;

	b2Vec2 vec1, vec2;

	result = doc->evaluate(
		YPT::XmlString("./vertex1"), shapeNode, resolver,
		DOMXPathResult::ORDERED_NODE_SNAPSHOT_TYPE,
		NULL
	);
	if (result == nullptr) {
		return false;
	}
	if (result->getSnapshotLength() == 0) {
		// 値が存在しない
		result->release();
		return false;
	}
	str = result->getNodeValue()->getTextContent();
	result->release();
	if (!YPT::ConvertStrToVec2(std::string(str), &vec1)) {
		// 値が正しい形式で書かれていない
		return false;
	}

	result = doc->evaluate(
		YPT::XmlString("./vertex2"), shapeNode, resolver,
		DOMXPathResult::ORDERED_NODE_SNAPSHOT_TYPE,
		NULL
	);
	if (result == nullptr) {
		return false;
	}
	if (result->getSnapshotLength() == 0) {
		// 値が存在しない
		result->release();
		return false;
	}
	str = result->getNodeValue()->getTextContent();
	result->release();
	if (!YPT::ConvertStrToVec2(std::string(str), &vec2)) {
		// 値が正しい形式で書かれていない
		return false;
	}

	dest->m_vertex1 = vec1;
	dest->m_vertex2 = vec2;

	return true;
}

b2EdgeShape XmlWorldPartReader::ReadEdgeShape(
	const DOMNode *shapeNode
) {
	b2EdgeShape temp;
	ReadEdgeShape(shapeNode, &temp);
	return temp;
}

bool XmlWorldPartReader::ReadPolygonShape(
	const DOMNode *shapeNode, b2PolygonShape *dest
) {
	DOMXPathNSResolver *resolver = doc->createNSResolver(shapeNode);
	if (resolver == nullptr) {
		return nullptr;
	}

	DOMXPathResult *result;
	YPT::XmlString str;

	int vertexCount;
	b2Vec2 vertices[b2_maxPolygonVertices];

	result = doc->evaluate(
		YPT::XmlString("./vertexCount"), shapeNode, resolver,
		DOMXPathResult::ORDERED_NODE_SNAPSHOT_TYPE,
		NULL
	);
	if (result == nullptr) {
		return false;
	}
	if (result->getSnapshotLength() == 0) {
		// 値が存在しない
		result->release();
		return false;
	}
	str = result->getNodeValue()->getTextContent();
	result->release();
	if (!YPT::ConvertStrToInt(std::string(str), &vertexCount)
		|| vertexCount <= 0 || vertexCount > b2_maxPolygonVertices
	) {
		// 値が正しい形式で書かれていない
		return false;
	}

	for (int i = 0; i < vertexCount; ++i) {
		std::string iStr = YPT::ConvertIntToStr(i+1);
		result = doc->evaluate(
			YPT::XmlString((
				"./vertices/vertex" + iStr + "|./vertices/v" + iStr
			).c_str()), shapeNode, resolver,
			DOMXPathResult::ORDERED_NODE_SNAPSHOT_TYPE,
			NULL
		);
		if (result == nullptr) {
			return false;
		}
		const XMLSize_t len = result->getSnapshotLength();
		if (len == 0) {
			// 値が存在しない
			result->release();
			return false;
		}
		str = result->getNodeValue()->getTextContent();
		result->release();
		if (!YPT::ConvertStrToVec2(std::string(str), &vertices[i])) {
			 // 値が正しい形式で書かれていない
			 return false;
		}
	}
	dest->Set(vertices, vertexCount);

	return true;
}

b2PolygonShape XmlWorldPartReader::ReadPolygonShape(
	const DOMNode *shapeNode
) {
	b2PolygonShape temp;
	ReadPolygonShape(shapeNode, &temp);
	return temp;
}

bool XmlWorldPartReader::ReadChainShape(
	const DOMNode *shapeNode, b2ChainShape *dest
) {
	DOMXPathNSResolver *resolver = doc->createNSResolver(shapeNode);
	if (resolver == nullptr) {
		return nullptr;
	}

	DOMXPathResult *result;
	YPT::XmlString str;

	int vertexCount;
	b2Vec2 vertices[b2_maxPolygonVertices];

	result = doc->evaluate(
		YPT::XmlString("./vertexCount"), shapeNode, resolver,
		DOMXPathResult::ORDERED_NODE_SNAPSHOT_TYPE,
		NULL
	);
	if (result == nullptr) {
		return false;
	}
	if (result->getSnapshotLength() == 0) {
		// 値が存在しない
		result->release();
		return false;
	}
	str = result->getNodeValue()->getTextContent();
	result->release();
	if (!YPT::ConvertStrToInt(std::string(str), &vertexCount)
		|| vertexCount <= 0 || vertexCount > b2_maxPolygonVertices
	) {
		// 値が正しい形式で書かれていない
		return false;
	}

	for (int i = 0; i < vertexCount; ++i) {
		std::string iStr = YPT::ConvertIntToStr(i+1);
		result = doc->evaluate(
			YPT::XmlString((
				"./vertices/vertex" + iStr + "|./vertices/v" + iStr
			).c_str()), shapeNode, resolver,
			DOMXPathResult::ORDERED_NODE_SNAPSHOT_TYPE,
			NULL
		);
		if (result == nullptr) {
			return false;
		}
		const XMLSize_t len = result->getSnapshotLength();
		if (len == 0) {
			// 値が存在しない
			result->release();
			return false;
		}
		str = result->getNodeValue()->getTextContent();
		result->release();
		if (!YPT::ConvertStrToVec2(std::string(str), &vertices[i])) {
			 // 値が正しい形式で書かれていない
			 return false;
		}
	}
	dest->CreateChain(vertices, vertexCount);

	return true;
}

b2ChainShape XmlWorldPartReader::ReadChainShape(
	const DOMNode *shapeNode
) {
	b2ChainShape temp;
	ReadChainShape(shapeNode, &temp);
	return temp;
}

} // YPT

