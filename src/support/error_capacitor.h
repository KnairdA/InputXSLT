#ifndef INPUTXSLT_SRC_SUPPORT_ERROR_CAPACITOR_H_
#define INPUTXSLT_SRC_SUPPORT_ERROR_CAPACITOR_H_

#include <xercesc/sax/ErrorHandler.hpp>
#include <xalanc/XSLT/ProblemListener.hpp>
#include <xalanc/XalanTransformer/XalanTransformer.hpp>

#include <memory>
#include <vector>
#include <string>

#include "common.h"

namespace InputXSLT {

class ErrorCapacitor : public xercesc::ErrorHandler,
                     public xalan::ProblemListener {
	public:
		class exception;

		typedef std::vector<std::string> error_cache;
		typedef std::unique_ptr<error_cache> error_cache_ptr;

		ErrorCapacitor(xalan::XalanTransformer*);
		~ErrorCapacitor();

		void discharge();

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

	private:
		xalan::XalanTransformer* const transformer_;
		error_cache_ptr error_cache_;

};

class ErrorCapacitor::exception {
	public:
		exception(error_cache_ptr);

		const error_cache* getCachedErrors() const;

	private:
		error_cache_ptr error_cache_;

};

}

#endif  // INPUTXSLT_SRC_SUPPORT_ERROR_CAPACITOR_H_
