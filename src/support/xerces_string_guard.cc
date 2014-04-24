#include "xerces_string_guard.h"

namespace InputXSLT {

XercesStringGuard::XercesStringGuard(const std::string& src):
	string_(xercesc::XMLString::transcode(src.data())) { }

XercesStringGuard::~XercesStringGuard() {
	xercesc::XMLString::release(
		const_cast<XMLCh**>(&this->string_)
	);
}

XMLCh* XercesStringGuard::operator*() {
	return this->string_;
}

}
