
#pragma once

#include <string>
#include <Box2D/Box2D.h>

// SrcをDestに変換する関数(1)：
//	Dest ConvertSrcToDest(Src またｈ const Src &);
//	(内部で(2)が呼び出される)

// SrcをDestに変換する関数(2):
//	bool ConvertSrcToDest(Src または const Src &, Dest *);
//	成功したらtrueを返す
//	失敗したらfalseを返し、destには影響を与えない

// Int		int
// Str		std::string
// Float	float
// Vec2		b2Vec2
// Bool		bool

namespace YPT {

std::string ConvertIntToStr(int src);
bool ConvertIntToStr(int src, std::string *dest);

int ConvertStrToInt(const std::string &src);
bool ConvertStrToInt(const std::string &src, int *dest);

float ConvertStrToFloat(const std::string &src);
bool ConvertStrToFloat(const std::string &src, float *dest);

b2Vec2 ConvertStrToVec2(const std::string &src);
bool ConvertStrToVec2(const std::string &src, b2Vec2 *dest);

bool ConvertStrToBool(const std::string &src);
bool ConvertStrToBool(const std::string &src, bool *dest);

std::string ConvertFloatToStr(float src);
bool ConvertFloatToStr(float src, std::string *dest);

std::string ConvertVec2ToStr(const b2Vec2 &src);
bool ConvertVec2ToStr(const b2Vec2 &src, std::string *dest);

std::string ConvertBoolToStr(bool src);
bool ConvertBoolToStr(bool src, std::string *dest);

} // YPT

