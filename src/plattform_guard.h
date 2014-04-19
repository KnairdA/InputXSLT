#ifndef INPUTXSLT_SRC_PLATTFORM_GUARD_H_
#define INPUTXSLT_SRC_PLATTFORM_GUARD_H_

#include <xalanc/Include/PlatformDefinitions.hpp>
#include <xercesc/util/PlatformUtils.hpp>

namespace xalan = xalanc_1_11;

namespace InputXSLT {

struct PlattformGuard {
	PlattformGuard() {
		xercesc::XMLPlatformUtils::Initialize();
		xalan::XalanTransformer::initialize();
	}

	~PlattformGuard() {
		xalan::XalanTransformer::terminate();
	}
};

}

#endif  // INPUTXSLT_SRC_PLATTFORM_GUARD_H_
