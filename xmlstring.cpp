
#include "xmlstring.hpp"
#include <cstring>

namespace YPT {

using namespace xercesc;

XmlString::XmlString()
	:length(0),
	capacity(0)
{
	strChar = new char[capacity+1];
	strXMLCh = new XMLCh[capacity+1];
	strChar[0] = '\0';
	strXMLCh[0] = '\0';
}

XmlString::XmlString(const char *str)
	:length(std::strlen(str)),
	capacity(length)
{
	strChar = new char[capacity+1];
	strXMLCh = new XMLCh[capacity+1];
	std::strcpy(strChar, str);
	XMLCh *temp = XMLString::transcode(str);
	XMLString::copyString(strXMLCh, temp);
	XMLString::release(&temp);
}

XmlString::XmlString(const XMLCh *str)
	:length(XMLString::stringLen(str)),
	capacity(length)
{
	strChar = new char[capacity+1];
	strXMLCh = new XMLCh[capacity+1];
	char *temp = XMLString::transcode(str);
	std::strcpy(strChar, temp);
	XMLString::release(&temp);
	XMLString::copyString(strXMLCh, str);
}

XmlString::XmlString(const XmlString &rhs)
	:length(rhs.length),
	capacity(rhs.capacity)
{
	strChar = new char[capacity+1];
	strXMLCh = new XMLCh[capacity+1];
	std::strcpy(strChar, rhs.strChar);
	XMLString::copyString(strXMLCh, rhs.strXMLCh);
}

XmlString::~XmlString() {
	if (strChar != nullptr)
		delete[] strChar;
	if (strXMLCh != nullptr)
		delete[] strXMLCh;
}

void XmlString::Set(const XmlString &rhs) {
	if (rhs.length > capacity) {
		delete[] strChar;
		delete[] strXMLCh;
		capacity = rhs.length;
		strChar = new char[capacity+1];
		strXMLCh = new XMLCh[capacity+1];
	}
	length = rhs.length;
	std::strcpy(strChar, rhs.strChar);
	XMLString::copyString(strXMLCh, rhs.strXMLCh);
}

void XmlString::Set(XmlString &&rhs) {
	delete[] strChar;
	delete[] strXMLCh;
	length = rhs.length;
	capacity = rhs.capacity;
	strChar = rhs.strChar;
	strXMLCh = rhs.strXMLCh;
	rhs.strChar = nullptr;
	rhs.strXMLCh = nullptr;
}

void XmlString::Set(const char *str) {
	const std::size_t len = std::strlen(str);
	if (len > capacity) {
		delete[] strChar;
		delete[] strXMLCh;
		capacity = len;
		strChar = new char[capacity+1];
		strXMLCh = new XMLCh[capacity+1];
	}
	length = len;
	std::strcpy(strChar, str);
	XMLCh *temp = XMLString::transcode(str);
	XMLString::copyString(strXMLCh, temp);
	XMLString::release(&temp);
}

void XmlString::Set(const XMLCh *str) {
	const std::size_t len = static_cast< std::size_t >(
		XMLString::stringLen(str)
	);
	if (len > capacity) {
		delete[] strChar;
		delete[] strXMLCh;
		capacity = len;
		strChar = new char[capacity+1];
		strXMLCh = new XMLCh[capacity+1];
	}
	length = len;
	char *temp = XMLString::transcode(str);
	std::strcpy(strChar, temp);
	XMLString::release(&temp);
	XMLString::copyString(strXMLCh, str);
}

const char *XmlString::ToChar() const {
	return strChar;
}

const XMLCh *XmlString::ToXMLCh() const {
	return strXMLCh;
}

std::size_t XmlString::Length() const {
	return length;
}

XMLSize_t XmlString::LengthXMLSize_t() const {
	return static_cast< XMLSize_t >(length);
}

}

