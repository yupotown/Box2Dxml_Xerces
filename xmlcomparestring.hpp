
#pragma once

#include <xercesc/util/XMLString.hpp>
#include <cstring>

namespace YPT {

inline int CompareString(const char *lhs, const char *rhs) {
	return std::strcmp(lhs, rhs);
}

inline int CompareString(const char *lhs, const XMLCh *rhs) {
	char *temp = xercesc::XMLString::transcode(rhs);
	int res = xercesc::XMLString::compareString(lhs, temp);
	xercesc::XMLString::release(&temp);
	return res;
}

inline int CompareString(const XMLCh *lhs, const char *rhs) {
	char *temp = xercesc::XMLString::transcode(lhs);
	int res = xercesc::XMLString::compareString(temp, rhs);
	xercesc::XMLString::release(&temp);
	return res;
}

inline int CompareString(const XMLCh *lhs, const XMLCh *rhs) {
	return xercesc::XMLString::compareString(lhs, rhs);
}

} // YPT

