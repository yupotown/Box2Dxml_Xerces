
#include "xmlworldreader.h"
#include "convert.h"
#include "xmlworldpartreader.h"
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include "xmlstring.hpp"

namespace YPT {

using namespace xercesc;

XmlWorldReader::XmlWorldReader(b2World &world)
	:world(world),
	initFlag(true)
{
	initialize();
}

XmlWorldReader::~XmlWorldReader() {
}

bool XmlWorldReader::Read(const std::string &file) {
	// ワールドを初期化
	try {
		initialize();
		if (initFlag) {
			initializeWorld();
		}
	} catch (...) {
		return false;
	}

	// TODO: ファイルの有無を確認

	// XMLファイルをパース
	const XMLCh gLS[] = {chLatin_L, chLatin_S, chNull};
	DOMImplementationLS *impl = DOMImplementationRegistry::getDOMImplementation(gLS);
	DOMLSParser *parser = impl->createLSParser(
		DOMImplementationLS::MODE_SYNCHRONOUS, NULL
	);
	DOMDocument *doc = parser->parseURI(file.c_str());
	if (doc == nullptr) {
		return false;
	}

	// rootノードを取得
	DOMElement *worldElement = doc->getDocumentElement();
	if (worldElement == nullptr) {
		parser->release();
		return false;
	}
	{
		YPT::XmlString temp("world");
		bool res = XMLString::equals(worldElement->getNodeName(), temp);
		if (!res) {
			parser->release();
			return false;
		}
	}

	// ロード用クラス作成
	YPT::XmlWorldPartReader partReader(doc);

	// XPathコンテキスト作成
	DOMXPathNSResolver *resolver = doc->createNSResolver(worldElement);
	if (resolver == nullptr) {
		parser->release();
		return false;
	}

	YPT::XmlString str, str2;
	DOMXPathResult *result;

	// --------------------------------------------------
	// ワールド全体の設定
	// --------------------------------------------------

	// ワールド名
	str = worldElement->getAttribute(YPT::XmlString("name"));
	if (str != "") {
		name = str;
	}

	// 重力ベクトル
	result = doc->evaluate(
		YPT::XmlString("./gravity"), worldElement, resolver,
		DOMXPathResult::ORDERED_NODE_SNAPSHOT_TYPE,
		NULL
	);
	if (result != nullptr) {
		if (result->getSnapshotLength() >= 1) {
			str = result->getNodeValue()->getTextContent();
			b2Vec2 temp;
			if (!YPT::ConvertStrToVec2(str.ToChar(), &temp)) {
				world.SetGravity(temp);
			}
		}
		result->release();
	}

	// --------------------------------------------------
	// shapes
	// --------------------------------------------------

	result = doc->evaluate(
		YPT::XmlString("./shape"), worldElement, resolver,
		DOMXPathResult::ORDERED_NODE_SNAPSHOT_TYPE,
		NULL
	);
	if (result != nullptr) {
		const XMLSize_t len = result->getSnapshotLength();
		for (XMLSize_t i = 0; i < len; ++i) {
			result->snapshotItem(i);
			DOMNode *node = result->getNodeValue();
			if (node == nullptr) {
				continue;
			}
			DOMNamedNodeMap *nodeMap = node->getAttributes();
			if (nodeMap == nullptr) {
				continue;
			}
			DOMNode *typeNode = nodeMap->getNamedItem(YPT::XmlString("type"));
			if (typeNode == nullptr) {
				continue;
			}
			str = typeNode->getNodeValue();
			b2Shape::Type type;
			int index;
			if (str == "circle") {
				type = b2Shape::e_circle;
				b2CircleShape temp;
				if (partReader.ReadCircleShape(node, &temp)) {
					circleShapes.push_back(temp);
					index = circleShapes.size()-1;
				} else {
					// 読み込み失敗
					continue;
				}
			} else if (str == "edge") {
				type = b2Shape::e_edge;
				b2EdgeShape temp;
				if (partReader.ReadEdgeShape(node, &temp)) {
					edgeShapes.push_back(temp);
					index = edgeShapes.size()-1;
				} else {
					// 読み込み失敗
					continue;
				}
			} else if (str == "polygon") {
				type = b2Shape::e_polygon;
				b2PolygonShape temp;
				if (partReader.ReadPolygonShape(node, &temp)) {
					polygonShapes.push_back(temp);
					index = polygonShapes.size()-1;
				} else {
					// 読み込み失敗
					continue;
				}
			} else if (str == "chain") {
				type = b2Shape::e_chain;
				b2ChainShape temp;
				if (partReader.ReadChainShape(node, &temp)) {
					chainShapes.push_back(temp);
					index = chainShapes.size()-1;
				} else {
					// 読み込み失敗
					continue;
				}
			} else {
				// 未対応
				continue;
			}

			// nameプロパティがあれば保存
			DOMNode *name = nodeMap->getNamedItem(YPT::XmlString("name"));
			if (name != nullptr) {
				str = name->getNodeValue();
				shapes.insert(ShapesMap::value_type(
					std::string(str),
					std::make_pair(type, index)
				));
			}
		}
		result->release();
	}

	// --------------------------------------------------
	// fixtures
	// --------------------------------------------------

	result = doc->evaluate(
		YPT::XmlString("./fixture"), worldElement, resolver,
		DOMXPathResult::ORDERED_NODE_SNAPSHOT_TYPE,
		NULL
	);
	if (result != nullptr) {
		const XMLSize_t len = result->getSnapshotLength();
		for (XMLSize_t i = 0; i < len; ++i) {
			result->snapshotItem(i);
			DOMNode *node = result->getNodeValue();
			if (node == nullptr) {
				continue;
			}
			DOMXPathResult *result2 = doc->evaluate(
				YPT::XmlString("./shape"), node, resolver,
				DOMXPathResult::ORDERED_NODE_SNAPSHOT_TYPE,
				NULL
			);
			if (result2 == nullptr) {
				continue;
			}
			DOMNode *shapeNode = result2->getNodeValue();
			if (shapeNode == nullptr) {
				continue;
			}
			str = shapeNode->getTextContent();
			result2->release();
			ShapesMap::iterator found = shapes.find(std::string(str));
			if (found == shapes.end()) {
				continue;
			}

			// fixture読み込み
			b2FixtureDef fixtureDef;
			b2Shape *shape = NULL;
			int index = found->second.second;
			switch (found->second.first) {
			case b2Shape::e_circle:
				shape = &circleShapes[index];
				break;
			case b2Shape::e_edge:
				shape = &edgeShapes[index];
				break;
			case b2Shape::e_polygon:
				shape = &polygonShapes[index];
				break;
			case b2Shape::e_chain:
				shape = &chainShapes[index];
				break;
			default:
				// 未対応
				break;
			}
			if (shape == NULL) {
				continue;
			}
			if (partReader.ReadFixture(node, shape, &fixtureDef)) {
				// 読み込み成功
				// nameプロパティがあれば保存する
				DOMNamedNodeMap *nodeMap = node->getAttributes();
				if (nodeMap == nullptr) {
					continue;
				}
				DOMNode *nameNode = nodeMap->getNamedItem(YPT::XmlString("name"));
				if (nameNode == nullptr) {
					continue;
				}
				str = nameNode->getNodeValue();
				fixtures.insert(FixturesMap::value_type(
					std::string(str), fixtureDef
				));
			}
		}
		result->release();
	}

	// --------------------------------------------------
	// bodies
	// --------------------------------------------------

	result = doc->evaluate(
		YPT::XmlString("./body"), worldElement, resolver,
		DOMXPathResult::ORDERED_NODE_SNAPSHOT_TYPE,
		NULL
	);
	if (result != nullptr) {
		const XMLSize_t len = result->getSnapshotLength();
		for (XMLSize_t i = 0; i < len; ++i) {
			result->snapshotItem(i);
			DOMNode *node = result->getNodeValue();
			if (node == nullptr) {
				continue;
			}
			DOMXPathResult *result2 = doc->evaluate(
				YPT::XmlString("./fixtures/fixture"), node, resolver,
				DOMXPathResult::ORDERED_NODE_SNAPSHOT_TYPE,
				NULL
			);
			if (result2 == nullptr) {
				continue;
			}
			std::vector< b2FixtureDef *> fixtureDefs;
			const XMLSize_t fixturesLen = result2->getSnapshotLength();
			for (XMLSize_t j = 0; j < fixturesLen; ++j) {
				result2->snapshotItem(j);
				DOMNode *fixtureNode = result2->getNodeValue();
				if (fixtureNode == nullptr) {
					continue;
				}
				str = fixtureNode->getTextContent();
				FixturesMap::iterator found = fixtures.find(
					std::string(str)
				);
				if (found != fixtures.end()) {
					fixtureDefs.push_back(&found->second);
				}
			}
			result2->release();

			b2Body *body = partReader.ReadBody(world, node, fixtureDefs);
			if (body != nullptr) {
				// 読み込み成功
				// nameプロパティがあれば保存する
				DOMNamedNodeMap *nodeMap = node->getAttributes();
				if (nodeMap == nullptr) {
					continue;
				}
				DOMNode *nameNode = nodeMap->getNamedItem(YPT::XmlString("name"));
				if (nameNode == nullptr) {
					continue;
				}
				str = nameNode->getNodeValue();
				bodies.insert(BodiesMap::value_type(
					std::string(str), body
				));
			}
		}
		result->release();
	}

	// --------------------------------------------------
	// 読み込み完了
	// --------------------------------------------------

	resolver->release();
	parser->release();

	return true;
}

void XmlWorldReader::SetInitializeWithReading(bool flag) {
	initFlag = flag;
}

bool XmlWorldReader::GetInitializeWithReading() const {
	return initFlag;
}

void XmlWorldReader::initialize() {
	XMLPlatformUtils::Initialize();
	name = "world";
	gravity.Set(0.0f, -9.8f);
	shapes.clear();
	fixtures.clear();
	bodies.clear();
}

void XmlWorldReader::initializeWorld() {
	world.SetGravity(gravity);

	for (b2Body *body = world.GetBodyList(), *next;
		body != NULL; body = next)
	{
		next = body->GetNext();
		world.DestroyBody(body);
	}

	for (b2Joint *joint = world.GetJointList(), *next;
		joint != NULL; joint = next)
	{
		next = joint->GetNext();
		world.DestroyJoint(joint);
	}
}

} // YPT

