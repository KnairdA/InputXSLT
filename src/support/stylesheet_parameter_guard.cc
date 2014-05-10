#include "stylesheet_parameter_guard.h"

namespace InputXSLT {

StylesheetParameterGuard::StylesheetParameterGuard(
	xalan::XalanTransformer& transformer
): transformer_(transformer) { }

StylesheetParameterGuard::StylesheetParameterGuard(
	xalan::XalanTransformer& transformer,
	const map& parameters
): transformer_(transformer) {
	this->set(parameters);
}

StylesheetParameterGuard::~StylesheetParameterGuard() {
	this->transformer_.clearStylesheetParams();
}

void StylesheetParameterGuard::set(const map& parameters) {
	for ( auto&& parameter : parameters ) {
		this->set(parameter.first, parameter.second);
	}
}

void StylesheetParameterGuard::set(
	const std::string& key,
	const std::string& value
) {
	this->transformer_.setStylesheetParam(
		key.data(),
		std::string("'" + value + "'").data()
	);
}

}
