#include "transformation_facade.h"

#include <xalanc/XSLT/XSLTInputSource.hpp>
#include <xalanc/XalanTransformer/XalanCompiledStylesheet.hpp>

#include "boost/filesystem.hpp"

#include <sstream>

namespace InputXSLT {

TransformationFacade::TransformationFacade(
	const std::string& transformation,
	IncludeEntityResolver* resolver
):
	transformation_{},
	transformer_(),
	error_handler_() {
	this->transformer_.setEntityResolver(resolver);
	this->transformer_.setErrorHandler(&this->error_handler_);
	this->transformer_.setProblemListener(&this->error_handler_);

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

auto TransformationFacade::generate(
	const std::string& targetPath,
	StylesheetParameterGuard& parameters
) -> return_type {
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

auto TransformationFacade::generate(
	std::basic_ostream<char>& targetStream,
	StylesheetParameterGuard& parameters
) -> return_type {
	return this->generate(
		xalan::XSLTResultTarget(targetStream),
		parameters
	);
}

auto TransformationFacade::generate(
	xalan::XSLTResultTarget&& outputTarget,
	StylesheetParameterGuard&
) -> return_type {
	if ( this->transformation_ != nullptr ) {
		std::stringstream      emptyStream("<dummy/>");
		xalan::XSLTInputSource inputSource(emptyStream);

		this->transformer_.transform(
			inputSource,
			this->transformation_,
			outputTarget
		);
	}

	return this->error_handler_.getCachedErrors();
}

}
