#include "transformer_guard.h"

#include <xalanc/Include/PlatformDefinitions.hpp>
#include <xercesc/util/PlatformUtils.hpp>

#include <xalanc/XSLT/XSLTInputSource.hpp>
#include <xalanc/XalanTransformer/XalanTransformer.hpp>
#include <xalanc/XalanTransformer/XercesDOMWrapperParsedSource.hpp>
#include <xalanc/XercesParserLiaison/XercesDOMSupport.hpp>

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMImplementation.hpp>

#include "function/read_file.h"
#include "function/read_xml_file.h"

namespace InputXSLT {

TransformerGuard::TransformerGuard():
	plattform_(),
	parser_(),
	transformer_() {
	const xalan::XalanDOMString customNamespace(
		"http://ExternalFunction.xalan-c++.xml.apache.org"
	);

	this->transformer_.installExternalFunction(
		customNamespace,
		xalan::XalanDOMString("read-file"),
		InputXSLT::FunctionReadFile()
	);

	this->transformer_.installExternalFunction(
		customNamespace,
		xalan::XalanDOMString("read-xml-file"),
		InputXSLT::FunctionReadXmlFile()
	);

}

int TransformerGuard::execute(
	const std::string& transformation,
	const std::string& target
) {
	xercesc::DOMDocument* inputDom(
		xercesc::DOMImplementation::getImplementation()->createDocument()
	);
	xalan::XercesDOMSupport domSupport(this->parser_);

	xalan::XercesDOMWrapperParsedSource parsedInput(
		inputDom,
		this->parser_,
		domSupport,
		xalan::XalanDOMString("")
	);

	xalan::XSLTInputSource transform(transformation.data());
	xalan::XSLTResultTarget output(target.data());

	return this->transformer_.transform(
		parsedInput,
		transform,
		output
	);
}

}
