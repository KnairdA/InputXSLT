#ifndef INPUTXSLT_SRC_FUNCTION_READ_DIRECTORY_H_
#define INPUTXSLT_SRC_FUNCTION_READ_DIRECTORY_H_

#include <xalanc/XalanTransformer/XalanTransformer.hpp>
#include <xalanc/XPath/XObjectFactory.hpp>
#include <xalanc/XPath/Function.hpp>
#include <xalanc/XPath/XObject.hpp>

#include <string>

#include "common.h"
#include "support/filesystem_context.h"

namespace InputXSLT {

class FunctionReadDirectory : public xalan::Function {
	public:
		FunctionReadDirectory(const FilesystemContext&);

		virtual xalan::XObjectPtr execute(
			xalan::XPathExecutionContext&,
			xalan::XalanNode*,
			const xalan::Function::XObjectArgVectorType&,
			const xalan::Locator*
		) const;

		virtual FunctionReadDirectory* clone(xalan::MemoryManager&) const;

		FunctionReadDirectory& operator=(const FunctionReadDirectory&) = delete;
		bool operator==(const FunctionReadDirectory&) const            = delete;

	private:
		const FilesystemContext& fs_context_;

		const xalan::XalanDOMString& getError(xalan::XalanDOMString&) const;

};

}

#endif  // INPUTXSLT_SRC_FUNCTION_READ_DIRECTORY_H_
