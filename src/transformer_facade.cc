#include "transformer_facade.h"

#include <xalanc/Include/PlatformDefinitions.hpp>
#include <xercesc/util/PlatformUtils.hpp>

#include <xalanc/XSLT/XSLTInputSource.hpp>
#include <xalanc/XalanTransformer/XalanTransformer.hpp>
#include <xalanc/XalanTransformer/XercesDOMWrapperParsedSource.hpp>
#include <xalanc/XercesParserLiaison/XercesDOMSupport.hpp>

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMImplementation.hpp>

#include <iostream>

#include "function/read_file.h"
#include "function/read_xml_file.h"
#include "function/read_directory.h"


namespace InputXSLT {

TransformerFacade::TransformerFacade(const std::string& path):
	fs_context_(path),
	parser_(),
	transformer_() {
	const xalan::XalanDOMString customNamespace(
		"http://ExternalFunction.xalan-c++.xml.apache.org"
	);

	this->transformer_.installExternalFunction(
		customNamespace,
		xalan::XalanDOMString("read-file"),
		InputXSLT::FunctionReadFile(this->fs_context_)
	);

	this->transformer_.installExternalFunction(
		customNamespace,
		xalan::XalanDOMString("read-xml-file"),
		InputXSLT::FunctionReadXmlFile(this->fs_context_)
	);

	this->transformer_.installExternalFunction(
		customNamespace,
		xalan::XalanDOMString("read-directory"),
		InputXSLT::FunctionReadDirectory(this->fs_context_)
	);
}

int TransformerFacade::execute(
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
