#include <xalanc/Include/PlatformDefinitions.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xalanc/XalanTransformer/XalanTransformer.hpp>
#include <xalanc/XSLT/XSLTInputSource.hpp>

#include "src/read_file_command.h"
#include "src/read_xml_file_command.h"

int main() {
	xercesc::XMLPlatformUtils::Initialize();
	xalan::XalanTransformer::initialize();

	const xalan::XalanDOMString customNamespace(
		"http://ExternalFunction.xalan-c++.xml.apache.org"
	);

	xalan::XalanTransformer transformer;

	transformer.installExternalFunction(
		customNamespace,
		xalan::XalanDOMString("read-file"),
		FunctionReadFile()
	);

	transformer.installExternalFunction(
		customNamespace,
		xalan::XalanDOMString("read-xml-file"),
		FunctionReadXmlFile()
	);

	xalan::XSLTInputSource  input("../dummy/in.xml");
	xalan::XSLTInputSource  tranformation("../dummy/transform.xsl");
	xalan::XSLTResultTarget output("out.xml");

	int result(transformer.transform(
		input,
		tranformation,
		output
	));

	xalan::XalanTransformer::terminate();

	return result;
}
