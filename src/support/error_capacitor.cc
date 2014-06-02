#include "error_capacitor.h"

#include <xercesc/sax/SAXParseException.hpp>

#include <xalanc/PlatformSupport/DOMStringPrintWriter.hpp>

#include <iostream>

#include "support/xalan_string.h"
#include "support/xerces_string_guard.h"

namespace {

using InputXSLT::XercesStringGuard;

inline std::string getMessage(const xercesc::SAXParseException& exception) {
	return (
		std::string(*XercesStringGuard<char>(exception.getMessage()))
		+ ". (Occurred in entity '"
		+ std::string(*XercesStringGuard<char>(exception.getSystemId()))
		+ "', at line "
		+ std::to_string(exception.getLineNumber())
		+ ", column "
		+ std::to_string(exception.getColumnNumber())
		+ ".)"
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
	xalan::ProblemListenerBase::eSource         source,
	xalan::ProblemListenerBase::eClassification classification,
	const xalan::XalanDOMString&                message,
	const xalan::Locator*                       locator,
	const xalan::XalanNode*                     node
) {
	xalan::XalanDOMString problemSummary;
	xalan::DOMStringPrintWriter writer(problemSummary);

	defaultFormat(
		writer,
		source,
		classification,
		message,
		locator,
		node
	);

	this->error_cache_->emplace_back(toString(problemSummary));
}

void ErrorCapacitor::problem(
	xalan::ProblemListenerBase::eSource         source,
	xalan::ProblemListenerBase::eClassification classification,
	const xalan::XalanDOMString&                message,
	const xalan::XalanNode*                     node
) {
	xalan::XalanDOMString problemSummary;
	xalan::DOMStringPrintWriter writer(problemSummary);

	defaultFormat(
		writer,
		source,
		classification,
		message,
		node
	);

	this->error_cache_->emplace_back(toString(problemSummary));
}

void ErrorCapacitor::problem(
	xalan::ProblemListenerBase::eSource,
	xalan::ProblemListenerBase::eClassification,
	const xalan::XalanNode*,
	const xalan::ElemTemplateElement*,
	const xalan::XalanDOMString&,
	const xalan::XalanDOMChar*,
	xalan::XalanFileLoc,
	xalan::XalanFileLoc
) { }

void ErrorCapacitor::setPrintWriter(xalan::PrintWriter*) { }

ErrorCapacitor::exception::exception(error_cache_ptr ptr):
	error_cache_(std::move(ptr)) { }

auto ErrorCapacitor::exception::getCachedErrors() const -> const error_cache* {
	return this->error_cache_.get();
}

}
