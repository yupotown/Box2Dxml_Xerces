
#pragma once

#include "xmlcomparestring.hpp"
#include <xercesc/util/XMLString.hpp>

namespace YPT {

class XmlString {
public:
	XmlString();
	XmlString(const char *str);
	XmlString(const XMLCh *str);
	XmlString(const XmlString &rhs);
	virtual ~XmlString();

	void Set(const XmlString &rhs);
	void Set(XmlString &&rhs);
	void Set(const char *str);
	void Set(const XMLCh *str);

	const char *ToChar() const;
	const XMLCh *ToXMLCh() const;

	bool Equals(const XmlString &str) const;
	bool Equals(const char *str) const;
	bool Equals(const XMLCh *str) const;

	std::size_t Length() const;
	XMLSize_t LengthXMLSize_t() const;

	const XmlString &operator =(const XmlString &rhs) {
		Set(rhs);
		return *this;
	}
	const XmlString &operator =(const XmlString &&rhs) {
		Set(rhs);
		return *this;
	}
	const XmlString &operator =(const char *rhs) {
		Set(rhs);
		return *this;
	}
	const XmlString &operator =(const XMLCh *rhs) {
		Set(rhs);
		return *this;
	}

	bool operator ==(const XmlString &rhs) const {
		return YPT::CompareString(strChar, rhs.strChar) == 0;
	}
	bool operator ==(const char *rhs) const {
		return YPT::CompareString(strChar, rhs) == 0;
	}
	bool operator ==(const XMLCh *rhs) const {
		return YPT::CompareString(strXMLCh, rhs) == 0;
	}

	bool operator <(const XmlString &rhs) const {
		return YPT::CompareString(strChar, rhs.strChar) < 0;
	}
	bool operator <(const char *rhs) const {
		return YPT::CompareString(strChar, rhs) < 0;
	}
	bool operator <(const XMLCh *rhs) const {
		return YPT::CompareString(strXMLCh, rhs) < 0;
	}
	bool operator >(const XmlString &rhs) const {
		return YPT::CompareString(strChar, rhs.strChar) > 0;
	}
	bool operator >(const char *rhs) const {
		return YPT::CompareString(strChar, rhs) > 0;
	}
	bool operator >(const XMLCh *rhs) const {
		return YPT::CompareString(strXMLCh, rhs) > 0;
	}
	bool operator >=(const XmlString &rhs) const {
		return !this->operator <(rhs);
	}
	bool operator >=(const char *rhs) const {
		return !this->operator <(rhs);
	}
	bool operator >=(const XMLCh *rhs) const {
		return !this->operator <(rhs);
	}
	bool operator <=(const XmlString &rhs) const {
		return !this->operator >(rhs);
	}
	bool operator <=(const char *rhs) const {
		return !this->operator >(rhs);
	}
	bool operator <=(const XMLCh *rhs) const {
		return !this->operator >(rhs);
	}

	operator const char *() const {
		return ToChar();
	}
	operator const XMLCh *() const {
		return ToXMLCh();
	}

private:
	char *strChar;
	XMLCh *strXMLCh;
	std::size_t length, capacity;
};

inline bool operator ==(const char *lhs, const YPT::XmlString &rhs) {
	return rhs == lhs;
}
inline bool operator ==(const XMLCh *lhs, const YPT::XmlString &rhs) {
	return rhs == lhs;
}

inline bool operator <(const char *lhs, const YPT::XmlString &rhs) {
	return rhs > lhs;
}
inline bool operator <(const XMLCh *lhs, const YPT::XmlString &rhs) {
	return rhs > lhs;
}
inline bool operator >(const char *lhs, const YPT::XmlString &rhs) {
	return rhs < lhs;
}
inline bool operator >(const XMLCh *lhs, const YPT::XmlString &rhs) {
	return rhs < lhs;
}
inline bool operator >=(const char *lhs, const YPT::XmlString &rhs) {
	return rhs <= lhs;
}
inline bool operator >=(const XMLCh *lhs, const YPT::XmlString &rhs) {
	return rhs <= lhs;
}
inline bool operator <=(const char *lhs, const YPT::XmlString &rhs) {
	return rhs >= lhs;
}
inline bool operator <=(const XMLCh *lhs, const YPT::XmlString &rhs) {
	return rhs >= lhs;
}

}

