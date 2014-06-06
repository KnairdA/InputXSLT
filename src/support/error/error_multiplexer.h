#ifndef INPUTXSLT_SRC_SUPPORT_ERROR_ERROR_MULTIPLEXER_H_
#define INPUTXSLT_SRC_SUPPORT_ERROR_ERROR_MULTIPLEXER_H_

#include <xercesc/sax/ErrorHandler.hpp>

#include <xalanc/XSLT/ProblemListener.hpp>
#include <xalanc/XalanTransformer/XalanTransformer.hpp>

#include <vector>
#include <string>

#include "common.h"

namespace InputXSLT {

class ErrorMultiplexer : public xercesc::ErrorHandler,
                         public xalan::ProblemListener {
	public:
		enum class error_type;
		class receiver;

		ErrorMultiplexer(xalan::XalanTransformer*);
		~ErrorMultiplexer();

		void connectReceiver(receiver*);
		void disconnectReceiver(receiver*);

		virtual void warning(const xercesc::SAXParseException&);
		virtual void error(const xercesc::SAXParseException&);
		virtual void fatalError(const xercesc::SAXParseException&);
		virtual void resetErrors();

		virtual void problem(
			xalan::ProblemListenerBase::eSource,
			xalan::ProblemListenerBase::eClassification,
			const xalan::XalanDOMString&,
			const xalan::Locator*,
			const xalan::XalanNode*
		);

		virtual void problem(
			xalan::ProblemListenerBase::eSource,
			xalan::ProblemListenerBase::eClassification,
			const xalan::XalanDOMString&,
			const xalan::XalanNode*
		);

		virtual void problem(
			xalan::ProblemListenerBase::eSource,
			xalan::ProblemListenerBase::eClassification,
			const xalan::XalanNode*,
			const xalan::ElemTemplateElement*,
			const xalan::XalanDOMString&,
			const xalan::XalanDOMChar*,
			xalan::XalanFileLoc,
			xalan::XalanFileLoc
		);

		virtual void setPrintWriter(xalan::PrintWriter*);

	private:
		xalan::XalanTransformer* const transformer_;

		std::vector<receiver*> receivers_;

		void multiplex(const error_type, const std::string&);

};

enum class ErrorMultiplexer::error_type {
	warning,
	error
};

class ErrorMultiplexer::receiver {
	public:
		receiver(ErrorMultiplexer*);
		virtual ~receiver();

		virtual void receive(
			const ErrorMultiplexer::error_type,
			const std::string&
		) = 0;

	private:
		ErrorMultiplexer* const multiplexer_;

};

}

#endif  // INPUTXSLT_SRC_SUPPORT_ERROR_ERROR_MULTIPLEXER_H_
