#include "error_multiplexer.h"

#include <xercesc/sax/SAXParseException.hpp>

#include <xalanc/PlatformSupport/DOMStringPrintWriter.hpp>

#include <algorithm>

#include "support/xalan_string.h"
#include "support/xerces_string_guard.h"

namespace {

using InputXSLT::XercesStringGuard;
using InputXSLT::ErrorMultiplexer;

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

inline ErrorMultiplexer::ErrorType toErrorType(
	const xalan::ProblemListenerBase::eClassification classification) {
	switch ( classification ) {
		case xalan::ProblemListenerBase::eClassification::eMessage ||
		     xalan::ProblemListenerBase::eClassification::eWarning: {
			return ErrorMultiplexer::ErrorType::Warning;
		}
		default: {
			return ErrorMultiplexer::ErrorType::Error;
		}
	}
}

}

namespace InputXSLT {

ErrorMultiplexer::ErrorMultiplexer(xalan::XalanTransformer* transformer):
	transformer_(transformer),
	receivers_() {
	this->transformer_->setErrorHandler(this);
	this->transformer_->setProblemListener(this);
}

ErrorMultiplexer::~ErrorMultiplexer() {
	this->transformer_->setErrorHandler(nullptr);
	this->transformer_->setProblemListener(nullptr);
}

void ErrorMultiplexer::connectReceiver(Receiver* receiver) {
	this->receivers_.push_back(receiver);
}

void ErrorMultiplexer::disconnectReceiver(Receiver* receiver) {
	this->receivers_.erase(
		std::remove(
			this->receivers_.begin(),
			this->receivers_.end(),
			receiver
		)
	);
}

void ErrorMultiplexer::warning(const xercesc::SAXParseException& exception) {
	this->multiplex(
		ErrorType::Warning,
		"Warning: " + getMessage(exception)
	);
}

void ErrorMultiplexer::error(const xercesc::SAXParseException& exception) {
	this->multiplex(
		ErrorType::Error,
		"Error: " + getMessage(exception)
	);
}

void ErrorMultiplexer::fatalError(const xercesc::SAXParseException& exception) {
	this->multiplex(
		ErrorType::Error,
		"Fatal error: " + getMessage(exception)
	);
}

void ErrorMultiplexer::resetErrors() { }

void ErrorMultiplexer::problem(
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

	this->multiplex(
		toErrorType(classification),
		toString(problemSummary)
	);
}

void ErrorMultiplexer::problem(
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

	this->multiplex(
		toErrorType(classification),
		toString(problemSummary)
	);
}

void ErrorMultiplexer::problem(
	xalan::ProblemListenerBase::eSource,
	xalan::ProblemListenerBase::eClassification,
	const xalan::XalanNode*,
	const xalan::ElemTemplateElement*,
	const xalan::XalanDOMString&,
	const xalan::XalanDOMChar*,
	xalan::XalanFileLoc,
	xalan::XalanFileLoc
) { }

void ErrorMultiplexer::setPrintWriter(xalan::PrintWriter*) { }

void ErrorMultiplexer::multiplex(
	const ErrorType type,
	const std::string& message
) {
	std::for_each(
		this->receivers_.begin(),
		this->receivers_.end(),
		[&type, &message](Receiver* const receiver) -> void {
			receiver->receive(type, message);
		}
	);
}

}
