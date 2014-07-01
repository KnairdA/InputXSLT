#include "transformation_facade.h"

#include <xalanc/XSLT/XSLTInputSource.hpp>
#include <xalanc/XalanTransformer/XalanCompiledStylesheet.hpp>

#include "support/xerces_string_guard.h"

namespace InputXSLT {

TransformationFacade::TransformationFacade(
	xalan::XSLTInputSource transformation,
	IncludeEntityResolver* resolver
):
	input_{},
	transformation_{},
	transformer_(),
	error_multiplexer_(&transformer_),
	warning_capacitor_(&error_multiplexer_) {
	this->transformer_.setEntityResolver(resolver);

	ErrorCapacitor errorCapacitor(&this->error_multiplexer_);

	std::stringstream dummyStream("<dummy/>");

	this->transformer_.parseSource(
		xalan::XSLTInputSource(dummyStream),
		this->input_
	);

	this->transformer_.compileStylesheet(
		transformation,
		this->transformation_
	);

	errorCapacitor.discharge();
}

TransformationFacade::TransformationFacade(
	xalan::XSLTInputSource input,
	xalan::XSLTInputSource transformation,
	IncludeEntityResolver* resolver
):
	input_{},
	transformation_{},
	transformer_(),
	error_multiplexer_(&transformer_),
	warning_capacitor_(&error_multiplexer_) {
	this->transformer_.setEntityResolver(resolver);

	ErrorCapacitor errorCapacitor(&this->error_multiplexer_);

	this->transformer_.parseSource(
		input,
		this->input_
	);

	this->transformer_.compileStylesheet(
		transformation,
		this->transformation_
	);

	errorCapacitor.discharge();
}

WarningCapacitor::warning_cache_ptr TransformationFacade::getCachedWarnings() {
	return this->warning_capacitor_.discharge();
}

void TransformationFacade::generate(std::basic_ostream<char>& targetStream) {
	StylesheetParameterGuard guard(this->transformer_);

	this->generate(targetStream, guard);
}

void TransformationFacade::generate(
	std::basic_ostream<char>&            targetStream,
	const StylesheetParameterGuard::map& parameters
) {
	StylesheetParameterGuard guard(this->transformer_, parameters);

	this->generate(targetStream, guard);
}

void TransformationFacade::generate(
	std::basic_ostream<char>& targetStream,
	const xalan::XObjectPtr&  parameter
) {
	StylesheetParameterGuard guard(this->transformer_);
	guard.set("parameters", parameter);

	this->generate(targetStream, guard);
}


void TransformationFacade::generate(
	std::basic_ostream<char>& targetStream,
	StylesheetParameterGuard&
) {
	ErrorCapacitor errorCapacitor(&this->error_multiplexer_);

	this->transformer_.transform(
		*(this->input_),
		this->transformation_,
		targetStream
	);

	errorCapacitor.discharge();
}

}
