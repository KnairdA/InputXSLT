#include "transformation_facade.h"

#include <xalanc/XSLT/XSLTInputSource.hpp>
#include <xalanc/XalanTransformer/XalanCompiledStylesheet.hpp>

#include "boost/filesystem.hpp"

#include <sstream>
#include <iostream>

namespace InputXSLT {

TransformationFacade::TransformationFacade(const std::string& transformation):
	transformation_{},
	transformer_() {
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
	StylesheetParameterGuard guard(this->transformer_);

	return this->generate(target, guard);
}

int TransformationFacade::generate(
	const std::string& target,
	const StylesheetParameterGuard::map& parameters
) {
	StylesheetParameterGuard guard(this->transformer_, parameters);

	return this->generate(target, guard);
}

int TransformationFacade::generate(
	const std::string& target,
	const xalan::XObjectPtr& parameter
) {
	StylesheetParameterGuard guard(this->transformer_);
	guard.set("parameters", parameter);

	return this->generate(target, guard);
}

int TransformationFacade::generate(
	const std::string& target,
	StylesheetParameterGuard& parameters
) {
	const boost::filesystem::path targetFilePath(
		boost::filesystem::absolute(target)
	);

	parameters.set(
		"target-file",      targetFilePath.filename().string()
	);
	parameters.set(
		"parent-directory", targetFilePath.parent_path().filename().string()
	);

	std::stringstream       emptyStream("<dummy/>");
	xalan::XSLTInputSource  inputSource(emptyStream);
	xalan::XSLTResultTarget outputTarget(target.data());

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
