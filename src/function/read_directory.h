#ifndef INPUTXSLT_SRC_FUNCTION_READ_DIRECTORY_H_
#define INPUTXSLT_SRC_FUNCTION_READ_DIRECTORY_H_

#include <xalanc/XalanTransformer/XalanTransformer.hpp>
#include <xalanc/XPath/XObjectFactory.hpp>
#include <xalanc/XPath/Function.hpp>
#include <xalanc/XPath/XObject.hpp>

#include "common.h"
#include "support/filesystem_context.h"
#include "support/dom/document_cache.h"

namespace InputXSLT {

class FunctionReadDirectory : public xalan::Function {
	public:
		FunctionReadDirectory(const FilesystemContext&, DomDocumentCache&);

		virtual xalan::XObjectPtr execute(
			xalan::XPathExecutionContext&,
			xalan::XalanNode*,
			const xalan::XObjectPtr,
			const xalan::Locator*
		) const;

		virtual FunctionReadDirectory* clone(xalan::MemoryManager&) const;

		FunctionReadDirectory& operator=(const FunctionReadDirectory&) = delete;
		bool operator==(const FunctionReadDirectory&) const            = delete;

	private:
		const FilesystemContext& fs_context_;
		DomDocumentCache& document_cache_;

		const xalan::XalanDOMString& getError(xalan::XalanDOMString&) const;

};

}

#endif  // INPUTXSLT_SRC_FUNCTION_READ_DIRECTORY_H_
