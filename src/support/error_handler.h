#ifndef INPUTXSLT_SRC_SUPPORT_ERROR_HANDLER_H_
#define INPUTXSLT_SRC_SUPPORT_ERROR_HANDLER_H_

#include <xercesc/sax/ErrorHandler.hpp>
#include <xalanc/XSLT/ProblemListener.hpp>

#include <memory>
#include <vector>
#include <string>

#include "common.h"

namespace InputXSLT {

class ErrorHandler : public xercesc::ErrorHandler,
                     public xalan::ProblemListener {
	public:
		typedef std::vector<std::string> error_cache;
		typedef std::unique_ptr<error_cache> error_cache_ptr;

		ErrorHandler();

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
			const xalan::XalanNode*,
			const xalan::ElemTemplateElement*,
			const xalan::XalanDOMString&,
			const xalan::XalanDOMChar*,
			xalan::XalanFileLoc,
			xalan::XalanFileLoc
		);

		virtual void problem(
			xalan::ProblemListenerBase::eSource,
			xalan::ProblemListenerBase::eClassification,
			const xalan::XalanDOMString&,
			const xalan::XalanNode*
		);

		virtual void setPrintWriter(xalan::PrintWriter*);

		error_cache_ptr getCachedErrors();

	private:
		error_cache_ptr error_cache_;

		void constructErrorCache();

};

}

#endif  // INPUTXSLT_SRC_SUPPORT_ERROR_HANDLER_H_
