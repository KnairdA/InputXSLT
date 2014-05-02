#ifndef INPUTXSLT_SRC_FUNCTION_READ_XML_FILE_H_
#define INPUTXSLT_SRC_FUNCTION_READ_XML_FILE_H_

#include <xalanc/XalanTransformer/XalanTransformer.hpp>
#include <xalanc/XPath/XObjectFactory.hpp>
#include <xalanc/XPath/Function.hpp>
#include <xalanc/XPath/XObject.hpp>

#include <string>
#include <memory>

#include "common.h"
#include "support/dom/document_cache.h"

namespace InputXSLT {

class FunctionReadXmlFile : public xalan::Function {
	public:
		FunctionReadXmlFile();

		virtual xalan::XObjectPtr execute(
			xalan::XPathExecutionContext&,
			xalan::XalanNode*,
			const xalan::XObjectPtr,
			const xalan::Locator*
		) const;

		virtual FunctionReadXmlFile* clone(xalan::MemoryManager&) const;

		FunctionReadXmlFile& operator=(const FunctionReadXmlFile&) = delete;
		bool operator==(const FunctionReadXmlFile&) const          = delete;

	private:
		std::shared_ptr<DomDocumentCache> document_cache_;

		const xalan::XalanDOMString& getError(xalan::XalanDOMString& result) const;

};

}

#endif  // INPUTXSLT_SRC_FUNCTION_READ_XML_FILE_H_
