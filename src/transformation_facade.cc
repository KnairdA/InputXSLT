#include "transformation_facade.h"

#include <xalanc/XSLT/XSLTInputSource.hpp>
#include <xalanc/XalanTransformer/XalanCompiledStylesheet.hpp>

#include "boost/filesystem.hpp"

#include <sstream>

namespace InputXSLT {

auto TransformationFacade::try_create(
	const std::string& transformation,
	IncludeEntityResolver* resolver,
	const std::function<void(const ErrorCapacitor::error_cache&)>& handleErrors
) -> ptr{
	try {
		return ptr(
			new InputXSLT::TransformationFacade(transformation, resolver)
		);
	}
	catch (const ErrorCapacitor::exception& exception) {
		handleErrors(*exception);

		return ptr();
	}
}

TransformationFacade::TransformationFacade(
	const std::string& transformation,
	IncludeEntityResolver* resolver
):
	transformation_{},
	transformer_(),
	error_multiplexer_(&transformer_),
	warning_capacitor_(&error_multiplexer_) {
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

WarningCapacitor::warning_cache_ptr TransformationFacade::getCachedWarnings() {
	return this->warning_capacitor_.discharge();
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
