#ifndef INPUTXSLT_SRC_FUNCTION_READ_FILE_H_
#define INPUTXSLT_SRC_FUNCTION_READ_FILE_H_

#include <xalanc/XalanTransformer/XalanTransformer.hpp>
#include <xalanc/XPath/XObjectFactory.hpp>
#include <xalanc/XPath/Function.hpp>
#include <xalanc/XPath/XObject.hpp>

#include <string>

#include "common.h"
#include "support/dom/document_cache.h"

namespace InputXSLT {

class FunctionReadFile : public xalan::Function {
	public:
		FunctionReadFile();

		virtual xalan::XObjectPtr execute(
			xalan::XPathExecutionContext&,
			xalan::XalanNode*,
			const xalan::XObjectPtr,
			const xalan::Locator*
		) const;

		virtual FunctionReadFile* clone(xalan::MemoryManager&) const;

		FunctionReadFile& operator=(const FunctionReadFile&) = delete;
		bool operator==(const FunctionReadFile&) const       = delete;

	private:
		std::shared_ptr<DomDocumentCache> document_cache_;

		const xalan::XalanDOMString& getError(xalan::XalanDOMString&) const;

};

}

#endif  // INPUTXSLT_SRC_FUNCTION_READ_FILE_H_
