#include <xalanc/Include/PlatformDefinitions.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xalanc/XalanTransformer/XalanTransformer.hpp>
#include <xalanc/XSLT/XSLTInputSource.hpp>

#include "src/read_file_command.h"

int main() {
	xercesc::XMLPlatformUtils::Initialize();
	xalanc_1_11::XalanTransformer::initialize();

	const xalanc_1_11::XalanDOMString customNamespace(
		"http://ExternalFunction.xalan-c++.xml.apache.org"
	);

	xalanc_1_11::XalanTransformer transformer;

	transformer.installExternalFunction(
		customNamespace,
		xalanc_1_11::XalanDOMString("read-file"),
		FunctionFileRead()
	);

	xalanc_1_11::XSLTInputSource  input("dummy/in.xml");
	xalanc_1_11::XSLTInputSource  tranformation("dummy/transform.xsl");
	xalanc_1_11::XSLTResultTarget output("dummy/out.xml");

	int result(transformer.transform(
		input,
		tranformation,
		output
	));

	xalanc_1_11::XalanTransformer::terminate();

	return result;
}
