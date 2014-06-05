#include "transformation_facade.h"

#include <xalanc/XSLT/XSLTInputSource.hpp>
#include <xalanc/XalanTransformer/XalanCompiledStylesheet.hpp>

#include "boost/filesystem.hpp"

#include <sstream>

#include "support/error/error_capacitor.h"

namespace InputXSLT {

TransformationFacade::TransformationFacade(
	const std::string& transformation,
	IncludeEntityResolver* resolver
):
	transformation_{},
	transformer_(),
	error_multiplexer_(&transformer_) {
	this->transformer_.setEntityResolver(resolver);

	ErrorCapacitor errorCapacitor(&this->error_multiplexer_);

	this->transformer_.compileStylesheet(
		xalan::XSLTInputSource(transformation.data()),
		this->transformation_
	);

	errorCapacitor.discharge();
}

TransformationFacade::~TransformationFacade() {
	this->transformer_.destroyStylesheet(
		this->transformation_
	);
}

void TransformationFacade::generate(
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

	this->generate(
		xalan::XSLTResultTarget(targetPath.data()),
		parameters
	);
}

void TransformationFacade::generate(
	std::basic_ostream<char>& targetStream,
	StylesheetParameterGuard& parameters
) {
	this->generate(
		xalan::XSLTResultTarget(targetStream),
		parameters
	);
}

void TransformationFacade::generate(
	xalan::XSLTResultTarget&& outputTarget,
	StylesheetParameterGuard&
) {
	ErrorCapacitor errorCapacitor(&this->error_multiplexer_);

	std::stringstream      emptyStream("<dummy/>");
	xalan::XSLTInputSource inputSource(emptyStream);

	this->transformer_.transform(
		inputSource,
		this->transformation_,
		outputTarget
	);

	errorCapacitor.discharge();
}

}
