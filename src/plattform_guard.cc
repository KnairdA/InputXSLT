#include "plattform_guard.h"

#include <xalanc/Include/PlatformDefinitions.hpp>
#include <xalanc/XalanTransformer/XalanTransformer.hpp>

#include <xercesc/util/PlatformUtils.hpp>

#include "common.h"
#include "function/read_file.h"
#include "function/read_xml_file.h"
#include "function/read_directory.h"
#include "function/transform.h"

namespace InputXSLT {

PlattformGuard::PlattformGuard() {
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
}

PlattformGuard::~PlattformGuard() {
	xalan::XalanTransformer::terminate();
}

}
