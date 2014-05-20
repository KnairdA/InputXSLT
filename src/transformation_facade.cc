#include "transformation_facade.h"

#include <xalanc/XSLT/XSLTInputSource.hpp>
#include <xalanc/XalanTransformer/XalanCompiledStylesheet.hpp>

#include "boost/filesystem.hpp"

#include <sstream>
#include <iostream>

namespace InputXSLT {

TransformationFacade::TransformationFacade(
	const std::string& transformation,
	IncludeEntityResolver* resolver
):
	transformation_{},
	transformer_() {
	this->transformer_.setEntityResolver(resolver);

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

int TransformationFacade::generate(
	const std::string& targetPath,
	StylesheetParameterGuard& parameters
) {
	const boost::filesystem::path targetPathHelper(
		boost::filesystem::absolute(targetPath)
	);

	parameters.set(
		"target-file",      targetPathHelper.filename().string()
	);
	parameters.set(
		"parent-directory", targetPathHelper.parent_path().filename().string()
	);

	return this->generate(
		xalan::XSLTResultTarget(targetPath.data()),
		parameters
	);
}

int TransformationFacade::generate(
	std::basic_ostream<char>& targetStream,
	StylesheetParameterGuard& parameters
) {
	return this->generate(
		xalan::XSLTResultTarget(targetStream),
		parameters
	);
}

int TransformationFacade::generate(
	xalan::XSLTResultTarget&& outputTarget,
	StylesheetParameterGuard&
) {
	std::stringstream       emptyStream("<dummy/>");
	xalan::XSLTInputSource  inputSource(emptyStream);

	const int resultCode = this->transformer_.transform(
		inputSource,
		this->transformation_,
		outputTarget
	);

	if ( resultCode != 0 ) {
		std::cerr << this->transformer_.getLastError() << std::endl;
	}

	return resultCode;
}

}
