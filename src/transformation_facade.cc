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
	TransformationFacade(
		std::string{},
		transformation,
		resolver
	) { }

TransformationFacade::TransformationFacade(
	const std::string& input,
	const std::string& transformation,
	IncludeEntityResolver* resolver
):
	input_{},
	transformation_{},
	transformer_(),
	error_multiplexer_(&transformer_),
	warning_capacitor_(&error_multiplexer_) {
	this->transformer_.setEntityResolver(resolver);

	ErrorCapacitor errorCapacitor(&this->error_multiplexer_);

	if ( input.empty() ) {
		std::stringstream dummyStream("<dummy/>");

		this->transformer_.parseSource(
			xalan::XSLTInputSource(dummyStream),
			this->input_
		);
	} else {
		this->transformer_.parseSource(
			xalan::XSLTInputSource(input.data()),
			this->input_
		);
	}

	this->transformer_.compileStylesheet(
		xalan::XSLTInputSource(transformation.data()),
		this->transformation_
	);

	errorCapacitor.discharge();
}

TransformationFacade::~TransformationFacade() {
	this->transformer_.destroyParsedSource(
		this->input_
	);

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

	this->transformer_.transform(
		*(this->input_),
		this->transformation_,
		outputTarget
	);

	errorCapacitor.discharge();
}

}
