#include "error_capacitor.h"

#include <xercesc/sax/SAXParseException.hpp>

#include <iostream>

#include "support/xalan_string.h"
#include "support/xerces_string_guard.h"

namespace {

inline std::string getMessage(const xercesc::SAXParseException& exception) {
	return std::string(
		*InputXSLT::XercesStringGuard<char>(exception.getMessage())
	);
}

}

namespace InputXSLT {

ErrorCapacitor::ErrorCapacitor(xalan::XalanTransformer* transformer):
	transformer_(transformer),
	error_cache_(new error_cache()) {
	this->transformer_->setErrorHandler(this);
	this->transformer_->setProblemListener(this);
}

ErrorCapacitor::~ErrorCapacitor() {
	this->transformer_->setErrorHandler(nullptr);
	this->transformer_->setProblemListener(nullptr);
}

void ErrorCapacitor::discharge() {
	if ( !this->error_cache_->empty() ) {
		throw exception(std::move(this->error_cache_));
	}
}

void ErrorCapacitor::warning(const xercesc::SAXParseException& exception) {
	this->error_cache_->emplace_back(
		"Warning: " + getMessage(exception)
	);
}

void ErrorCapacitor::error(const xercesc::SAXParseException& exception) {
	this->error_cache_->emplace_back(
		"Error: " + getMessage(exception)
	);
}

void ErrorCapacitor::fatalError(const xercesc::SAXParseException& exception) {
	this->error_cache_->emplace_back(
		"Fatal error: " + getMessage(exception)
	);
}

void ErrorCapacitor::resetErrors() { }

void ErrorCapacitor::problem(
	xalan::ProblemListenerBase::eSource,
	xalan::ProblemListenerBase::eClassification,
	const xalan::XalanDOMString& message,
	const xalan::Locator*,
	const xalan::XalanNode*
) {
	this->error_cache_->emplace_back(
		"XSLT problem: " + toString(message)
	);
}

void ErrorCapacitor::problem(
	xalan::ProblemListenerBase::eSource,
	xalan::ProblemListenerBase::eClassification,
	const xalan::XalanNode*,
	const xalan::ElemTemplateElement*,
	const xalan::XalanDOMString& message,
	const xalan::XalanDOMChar*,
	xalan::XalanFileLoc,
	xalan::XalanFileLoc
) {
	this->error_cache_->emplace_back(
		"XSLT problem: " + toString(message)
	);
}

void ErrorCapacitor::problem(
	xalan::ProblemListenerBase::eSource,
	xalan::ProblemListenerBase::eClassification,
	const xalan::XalanDOMString& message,
	const xalan::XalanNode*
) {
	this->error_cache_->emplace_back(
		"XSLT problem: " + toString(message)
	);
}

void ErrorCapacitor::setPrintWriter(xalan::PrintWriter*) { }

ErrorCapacitor::exception::exception(error_cache_ptr ptr):
	error_cache_(std::move(ptr)) { }

auto ErrorCapacitor::exception::getCachedErrors() const -> const error_cache* {
	return this->error_cache_.get();
}

}
