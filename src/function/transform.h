#ifndef INPUTXSLT_SRC_FUNCTION_TRANSFORM_H_
#define INPUTXSLT_SRC_FUNCTION_TRANSFORM_H_

#include <xalanc/XalanTransformer/XalanTransformer.hpp>
#include <xalanc/XPath/XObjectFactory.hpp>
#include <xalanc/XPath/Function.hpp>
#include <xalanc/XPath/XObject.hpp>

#include <memory>

#include "common.h"
#include "support/dom/document_cache.h"
#include "support/filesystem_context.h"

namespace InputXSLT {

class FunctionTransform : public xalan::Function {
	public:
		FunctionTransform();

		virtual xalan::XObjectPtr execute(
			xalan::XPathExecutionContext&,
			xalan::XalanNode*,
			const xalan::XObjectPtr,
			const xalan::XObjectPtr,
			const xalan::Locator*
		) const;

		virtual FunctionTransform* clone(xalan::MemoryManager&) const;

		FunctionTransform& operator=(const FunctionTransform&) = delete;
		bool operator==(const FunctionTransform&) const        = delete;

	private:
		std::shared_ptr<DomDocumentCache> document_cache_;

		const xalan::XalanDOMString& getError(xalan::XalanDOMString&) const;

};

}

#endif  // INPUTXSLT_SRC_FUNCTION_TRANSFORM_H_
