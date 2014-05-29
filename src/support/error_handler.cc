#include "error_handler.h"

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

ErrorHandler::ErrorHandler():
	error_cache_{} { }

void ErrorHandler::warning(const xercesc::SAXParseException& exception) {
	this->constructErrorCache();

	this->error_cache_->emplace_back(
		"Warning: " + getMessage(exception)
	);
}

void ErrorHandler::error(const xercesc::SAXParseException& exception) {
	this->constructErrorCache();

	this->error_cache_->emplace_back(
		"Error: " + getMessage(exception)
	);
}

void ErrorHandler::fatalError(const xercesc::SAXParseException& exception) {
	this->constructErrorCache();

	this->error_cache_->emplace_back(
		"Fatal error: " + getMessage(exception)
	);
}

void ErrorHandler::resetErrors() { }

void ErrorHandler::problem(
	xalan::ProblemListenerBase::eSource,
	xalan::ProblemListenerBase::eClassification,
	const xalan::XalanDOMString& message,
	const xalan::Locator*,
	const xalan::XalanNode*
) {
	this->constructErrorCache();

	this->error_cache_->emplace_back(
		"XSLT problem: " + toString(message)
	);
}

void ErrorHandler::problem(
	xalan::ProblemListenerBase::eSource,
	xalan::ProblemListenerBase::eClassification,
	const xalan::XalanNode*,
	const xalan::ElemTemplateElement*,
	const xalan::XalanDOMString& message,
	const xalan::XalanDOMChar*,
	xalan::XalanFileLoc,
	xalan::XalanFileLoc
) {
	this->constructErrorCache();

	this->error_cache_->emplace_back(
		"XSLT problem: " + toString(message)
	);
}

void ErrorHandler::problem(
	xalan::ProblemListenerBase::eSource,
	xalan::ProblemListenerBase::eClassification,
	const xalan::XalanDOMString& message,
	const xalan::XalanNode*
) {
	this->constructErrorCache();

	this->error_cache_->emplace_back(
		"XSLT problem: " + toString(message)
	);
}

void ErrorHandler::setPrintWriter(xalan::PrintWriter*) { }

auto ErrorHandler::getCachedErrors() -> error_cache_ptr {
	return error_cache_ptr(this->error_cache_.release());
}

void ErrorHandler::constructErrorCache() {
	if ( !this->error_cache_ ) {
		this->error_cache_.reset(new error_cache());
	}
}

}
