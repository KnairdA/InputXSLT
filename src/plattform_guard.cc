#include "plattform_guard.h"

#include <xalanc/Include/PlatformDefinitions.hpp>
#include <xalanc/XalanTransformer/XalanTransformer.hpp>

#include <xercesc/util/PlatformUtils.hpp>

#include "common.h"
#include "function/read_file.h"
#include "function/write_file.h"
#include "function/read_directory.h"
#include "function/transform.h"
#include "function/external_text_formatter.h"

namespace InputXSLT {

PlattformGuard::PlattformGuard(const std::vector<std::string>& path):
	include_resolver_(path) {
	xercesc::XMLPlatformUtils::Initialize();
	xalan::XalanTransformer::initialize();

	const xalan::XalanDOMString customNamespace(
		"function.inputxslt.application"
	);

	xalan::XalanTransformer::installExternalFunctionGlobal(
		customNamespace,
		xalan::XalanDOMString("read-file"),
		InputXSLT::FunctionReadFile(&this->include_resolver_)
	);

	xalan::XalanTransformer::installExternalFunctionGlobal(
		customNamespace,
		xalan::XalanDOMString("write-file"),
		InputXSLT::FunctionWriteFile(&this->include_resolver_)
	);

	xalan::XalanTransformer::installExternalFunctionGlobal(
		customNamespace,
		xalan::XalanDOMString("read-directory"),
		InputXSLT::FunctionReadDirectory(&this->include_resolver_)
	);

	xalan::XalanTransformer::installExternalFunctionGlobal(
		customNamespace,
		xalan::XalanDOMString("transform"),
		InputXSLT::FunctionTransform(&this->include_resolver_)
	);

	xalan::XalanTransformer::installExternalFunctionGlobal(
		customNamespace,
		xalan::XalanDOMString("external-text-formatter"),
		InputXSLT::FunctionExternalTextFormatter(&this->include_resolver_)
	);
}

PlattformGuard::~PlattformGuard() {
	xalan::XalanTransformer::terminate();
}

IncludeEntityResolver* PlattformGuard::getEntityResolver() {
	return &this->include_resolver_;
}

}
