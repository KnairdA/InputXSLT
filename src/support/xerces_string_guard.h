#ifndef INPUTXSLT_SRC_SUPPORT_XERCES_STRING_GUARD_H_
#define INPUTXSLT_SRC_SUPPORT_XERCES_STRING_GUARD_H_

#include <xercesc/util/XMLString.hpp>

#include <string>

namespace InputXSLT {

class XercesStringGuard {
	public:
		XercesStringGuard(const std::string&);
		~XercesStringGuard();

		XMLCh* operator*();

	private:
		XMLCh* const string_;

};

}

#endif  // INPUTXSLT_SRC_SUPPORT_XERCES_STRING_GUARD_H_
