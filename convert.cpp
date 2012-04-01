
#include "convert.h"
#include <sstream>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <cstdlib>
#include <cctype>
#include <iterator>

namespace {
	// std::tolower は int (*)(int) なので都合が悪い
	struct ToLower {
		char operator()(const char c) {
			return std::tolower(c);
		}
	};
}

namespace YPT {

std::string ConvertIntToStr(int src) {
	std::string temp("");
	ConvertIntToStr(src, &temp);
	return temp;
}
bool ConvertIntToStr(int src, std::string *dest) {
	std::ostringstream oss;
	oss << src;
	if (dest != NULL)
		*dest = oss.str();
	return true;
}

int ConvertStrToInt(const std::string &src) {
	int temp = 0;
	ConvertStrToInt(src, &temp);
	return temp;
}
bool ConvertStrToInt(const std::string &src, int *dest) {
	if (src.empty())
		return false;

	int res = 0;
	int i = 0;
	const int length = src.length();
	bool minus = false;

	// 最初が'-'だったらminusフラグを立てておく
	if (src[i] == '-') {
		minus = true;
		++i;
	}

	// 数字部分
	// INT_MINからINT_MAXまで正しく変換するための分岐
	// 正のまま計算してあとから-1をかける場合、INT_MIN=-INT_MAX-1なら
	// INT_MINの場合の動作が未定義になる
	if (minus) {
		for (; i < length; ++i) {
			if (src[i] < '0' || src[i] > '9')
				return false;
			res *= 10;
			res -= src[i] - '0';
		}
	} else {
		for (; i < length; ++i) {
			if (src[i] < '0' || src[i] > '9')
				return false;
			res *= 10;
			res += src[i] - '0';
		}
	}

	if (dest != NULL)
		*dest = res;
	
	return true;
}

float ConvertStrToFloat(const std::string &src) {
	float temp = 0.0f;
	ConvertStrToFloat(src, &temp);
	return temp;
}
bool ConvertStrToFloat(const std::string &src, float *dest) {
	if (src.empty())
		return false;
	
	float res = 0.0f;
	int i = 0;
	const int length = src.length();
	bool minus = false;

	// 最初が'-'だったらminusフラグを立てておく
	if (src[i] == '-') {
		minus = true;
		++i;
	}

	// 小数部フラグ
	// 小数点'.'が見つかったら立てる
	bool decimal = false;

	// 整数部
	if (i == length)
		return false;
	for (; i < length; ++i) {
		if (src[i] == '.') {
			decimal = true;
			++i;
			break;
		}
		if (src[i] < '0' || src[i] > '9')
			return false;
		res *= 1e1;
		res += src[i] - '0';
	}

	if (decimal) {
		// 小数部
		if (i == length)
			return false;
		float digit = 1e-1;
		for (; i < length; ++i) {
			if (src[i] < '0' || src[i] > '9')
				return false;
			res += (src[i] - '0') * digit;
			digit /= 1e1;
		}
	}

	// minusフラグが立っていたら負にする
	if (minus)
		res *= -1;

	if (dest != NULL)
		*dest = res;

	return true;
}

b2Vec2 ConvertStrToVec2(const std::string &src) {
	b2Vec2 temp(0.0f, 0.0f);
	ConvertStrToVec2(src, &temp);
	return temp;
}
bool ConvertStrToVec2(const std::string &src, b2Vec2 *dest) {
	// ','で分割
	std::string::size_type found = src.find(",");
	if (found == std::string::npos)
		return false;
	
	// xとyをそれぞれ変換
	float x, y;
	if (!ConvertStrToFloat(src.substr(0, found), &x))
		return false;
	if (!ConvertStrToFloat(src.substr(found + 1), &y))
		return false;

	if (dest != NULL)
		dest->Set(x, y);

	return true;
}

bool ConvertStrToBool(const std::string &src) {
	bool temp = false;
	ConvertStrToBool(src, &temp);
	return temp;
}

bool ConvertStrToBool(const std::string &src, bool *dest) {
	// すべて小文字にする
	std::string lowerSrc;
	lowerSrc.reserve(src.length());
	std::transform(src.begin(), src.end(), std::back_inserter(lowerSrc),
		ToLower());
	
	if (lowerSrc == "true"
		|| lowerSrc == "1")
	{
		*dest = true;
		return true;
	}
	if (lowerSrc == "false"
		|| lowerSrc == "0")
	{
		*dest = false;
		return true;
	}

	return false;
}

std::string ConvertFloatToStr(float src) {
	std::string temp("");
	ConvertFloatToStr(src, &temp);
	return temp;
}

bool ConvertFloatToStr(float src, std::string *dest) {
	std::ostringstream oss;
	oss << std::setiosflags(std::ios::fixed) << src;
	if (dest != NULL)
		*dest = oss.str();
	return true;
}

std::string ConvertVec2ToStr(const b2Vec2 &src) {
	std::string temp("");
	ConvertVec2ToStr(src, &temp);
	return temp;
}
bool ConvertVec2ToStr(const b2Vec2 &src, std::string *dest) {
	std::string x, y;
	if (!ConvertFloatToStr(src.x, &x))
		return false;
	if (!ConvertFloatToStr(src.y, &y))
		return false;
	if (dest != NULL)
		*dest = x + "," + y;
	return true;
}

std::string ConvertBoolToStr(bool src) {
	std::string temp;
	ConvertBoolToStr(src, &temp);
	return temp;
}

bool ConvertBoolToStr(bool src, std::string *dest) {
	if (src)
		*dest = "true";
	else
		*dest = "false";
	return true;
}

} // YPT

