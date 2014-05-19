#include "plattform_guard.h"

#include <xalanc/Include/PlatformDefinitions.hpp>
#include <xalanc/XalanTransformer/XalanTransformer.hpp>

#include <xercesc/util/PlatformUtils.hpp>

#include "common.h"
#include "function/read_file.h"
#include "function/read_xml_file.h"
#include "function/read_directory.h"
#include "function/transform.h"
#include "function/resolve_include.h"

namespace InputXSLT {

PlattformGuard::PlattformGuard(const std::vector<std::string>& path) {
	xercesc::XMLPlatformUtils::Initialize();
	xalan::XalanTransformer::initialize();

	const xalan::XalanDOMString customNamespace(
		"function.inputxslt.application"
	);

	xalan::XalanTransformer::installExternalFunctionGlobal(
		customNamespace,
		xalan::XalanDOMString("read-file"),
		InputXSLT::FunctionReadFile()
	);

	xalan::XalanTransformer::installExternalFunctionGlobal(
		customNamespace,
		xalan::XalanDOMString("read-xml-file"),
		InputXSLT::FunctionReadXmlFile()
	);

	xalan::XalanTransformer::installExternalFunctionGlobal(
		customNamespace,
		xalan::XalanDOMString("read-directory"),
		InputXSLT::FunctionReadDirectory()
	);

	xalan::XalanTransformer::installExternalFunctionGlobal(
		customNamespace,
		xalan::XalanDOMString("transform"),
		InputXSLT::FunctionTransform()
	);

	xalan::XalanTransformer::installExternalFunctionGlobal(
		customNamespace,
		xalan::XalanDOMString("resolve-include"),
		InputXSLT::FunctionResolveInclude(path)
	);
}

PlattformGuard::~PlattformGuard() {
	xalan::XalanTransformer::terminate();
}

}
