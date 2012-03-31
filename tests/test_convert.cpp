
#include "../convert.h"
#include <iostream>
#include <Box2D/Box2D.h>
#include <string>
#include <vector>
#include <climits>
#include <boost/function.hpp>
#include <boost/call_traits.hpp>
#include <functional>

std::ostream &operator <<(std::ostream &os, const b2Vec2 &rhs) {
	os << "(" << rhs.x << "," << rhs.y << ")";
	return os;
}

template <class SrcType, class DestType, class C>
void Test(const boost::function<bool (SrcType, DestType *)> &func,
	C value)
{
	std::cout << value << "\t-> ";
	DestType temp;
	if (func(value, &temp))
		std::cout << temp;
	else
		std::cout << "failed.";
	std::cout << std::endl;
}

int main() {
	std::vector< int > intCases;
	intCases.push_back(123);
	intCases.push_back(0);
	intCases.push_back(-456);
	intCases.push_back(INT_MAX);
	intCases.push_back(INT_MIN);

	std::vector< float > floatCases;
	floatCases.push_back(12.34f);
	floatCases.push_back(0.0f);
	floatCases.push_back(-3.1415926575);
	floatCases.push_back(1e20);
	floatCases.push_back(1e-20);

	std::vector< std::string > strIntCases;
	strIntCases.push_back("123");
	strIntCases.push_back("0");
	strIntCases.push_back("-456");

	std::vector< std::string > strFloatCases;
	strFloatCases.push_back("12.34");
	strFloatCases.push_back("0.0");
	strFloatCases.push_back("-3.1415926535");
	strFloatCases.push_back("123");
	strFloatCases.push_back("-456");
	strFloatCases.push_back("0");

	std::vector< std::string > strVec2Cases;
	strVec2Cases.push_back("1.2,-9.8");
	strVec2Cases.push_back("0.0,0.0");
	strVec2Cases.push_back("-1.0,2.0");
	strVec2Cases.push_back("1,2.3");
	strVec2Cases.push_back("3.141592,6");

	std::vector< std::string > strBoolCases;
	strBoolCases.push_back("true");
	strBoolCases.push_back("false");
	strBoolCases.push_back("1");
	strBoolCases.push_back("0");
	strBoolCases.push_back("TRUE");
	strBoolCases.push_back("FaLSe");

	std::vector< b2Vec2 > vec2Cases;
	vec2Cases.push_back(b2Vec2(5.5f, 10.1f));
	vec2Cases.push_back(b2Vec2(0.0f, 0.0f));
	vec2Cases.push_back(b2Vec2(-3.4f, -2.8f));
	vec2Cases.push_back(b2Vec2(1e10, 1e-10));

	std::vector< bool > boolCases;
	boolCases.push_back(false);
	boolCases.push_back(true);

	typedef std::vector< int >::const_iterator IntIterator;
	typedef std::vector< float >::const_iterator FloatIterator;
	typedef std::vector< std::string >::const_iterator StrIterator;
	typedef std::vector< b2Vec2 >::const_iterator Vec2Iterator;
	typedef std::vector< bool >::const_iterator BoolIterator;

	bool (*pIntToStr)(int, std::string *) = &YPT::ConvertIntToStr;
	bool (*pStrToInt)(const std::string &, int *) = &YPT::ConvertStrToInt;
	bool (*pStrToFloat)(const std::string &, float *) = &YPT::ConvertStrToFloat;
	bool (*pStrToVec2)(const std::string &, b2Vec2 *) = &YPT::ConvertStrToVec2;
	bool (*pStrToBool)(const std::string &, bool *) = &YPT::ConvertStrToBool;
	bool (*pFloatToStr)(float, std::string *) = &YPT::ConvertFloatToStr;
	bool (*pVec2ToStr)(const b2Vec2 &, std::string *) = &YPT::ConvertVec2ToStr;
	bool (*pBoolToStr)(bool, std::string *) = &YPT::ConvertBoolToStr;

	boost::function<bool (int, std::string *)> IntToStr(pIntToStr);
	boost::function<bool (const std::string &, int *)> StrToInt(pStrToInt);
	boost::function<bool (const std::string &, float *)> StrToFloat(pStrToFloat);
	boost::function<bool (const std::string &, b2Vec2 *)> StrToVec2(pStrToVec2);
	boost::function<bool (const std::string &, bool *)> StrToBool(pStrToBool);
	boost::function<bool (float, std::string *)> FloatToStr(pFloatToStr);
	boost::function<bool (const b2Vec2 &, std::string *)> Vec2ToStr(pVec2ToStr);
	boost::function<bool (bool, std::string *)> BoolToStr(pBoolToStr);

	// test - ConvertIntToStr
	std::cout << std::endl << "---YPT::ConvertIntToStr" << std::endl;
	for (IntIterator it = intCases.begin(); it != intCases.end(); ++it) {
		Test(IntToStr, *it);
	}

	// test - ConvertStrToInt
	std::cout << std::endl << "---YPT::ConvertStrToInt" << std::endl;
	for (StrIterator it = strIntCases.begin(); it != strIntCases.end(); ++it) {
		Test(StrToInt, *it);
	}

	// test - ConvertStrToFloat
	std::cout << std::endl << "---YPT::ConvertStrToFloat" << std::endl;
	for (StrIterator it = strFloatCases.begin(); it != strFloatCases.end(); ++it) {
		Test(StrToFloat, *it);
	}

	// test - ConvertStrToVec2
	std::cout << std::endl << "---YPT::ConvertStrToVec2" << std::endl;
	for (StrIterator it = strVec2Cases.begin(); it != strVec2Cases.end(); ++it) {
		Test(StrToVec2, *it);
	}

	// test - ConvertStrToBool
	std::cout << std::endl << "---YPT::ConvertStrToBool" << std::endl;
	for (StrIterator it = strBoolCases.begin(); it != strBoolCases.end(); ++it) {
		Test(StrToBool, *it);
	}

	// test - ConvertFloatToStr
	std::cout << std::endl << "---YPT::ConvertFloatToStr" << std::endl;
	for (FloatIterator it = floatCases.begin(); it != floatCases.end(); ++it) {
		Test(FloatToStr, *it);
	}

	// test - ConvertVec2ToStr
	std::cout << std::endl << "---YPT::ConvertVec2ToStr" << std::endl;
	for (Vec2Iterator it = vec2Cases.begin(); it != vec2Cases.end(); ++it) {
		Test(Vec2ToStr, *it);
	}

	// test - ConvertBoolToStr
	std::cout << std::endl << "---YPT::ConvertBoolToStr" << std::endl;
	for (BoolIterator it = boolCases.begin(); it != boolCases.end(); ++it) {
		Test(BoolToStr, *it);
	}

	return 0;
}

