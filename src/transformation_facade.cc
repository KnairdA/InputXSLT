#include "transformation_facade.h"

#include <xalanc/XSLT/XSLTInputSource.hpp>
#include <xalanc/XalanTransformer/XalanCompiledStylesheet.hpp>

#include <sstream>

#include "function/read_file.h"
#include "function/read_xml_file.h"
#include "function/read_directory.h"

namespace InputXSLT {

TransformationFacade::TransformationFacade(const std::string& transformation):
	fs_context_(boost::filesystem::path(transformation).parent_path().string()),
	transformation_{},
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

	this->transformer_.compileStylesheet(
		xalan::XSLTInputSource(transformation.data()),
		this->transformation_
	);
}

TransformationFacade::~TransformationFacade() {
	this->transformer_.destroyStylesheet(
		this->transformation_
	);
}

int TransformationFacade::generate(const std::string& target) {
	std::stringstream       emptyStream("<dummy/>");
	xalan::XSLTInputSource  inputSource(emptyStream);
	xalan::XSLTResultTarget outputTarget(target.data());

	return this->transformer_.transform(
		inputSource,
		this->transformation_,
		outputTarget
	);
}

}
