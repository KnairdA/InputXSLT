#ifndef INPUTXSLT_SRC_FUNCTION_READ_FILE_H_
#define INPUTXSLT_SRC_FUNCTION_READ_FILE_H_

#include <xalanc/XalanTransformer/XalanTransformer.hpp>
#include <xalanc/XPath/XObjectFactory.hpp>
#include <xalanc/XPath/Function.hpp>
#include <xalanc/XPath/XObject.hpp>

#include <string>

#include "common.h"
#include "support/filesystem_context.h"

namespace InputXSLT {

class FunctionReadFile : public xalan::Function {
	public:
		FunctionReadFile(const FilesystemContext&);

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
		const FilesystemContext& fs_context_;

		const xalan::XalanDOMString& getError(xalan::XalanDOMString&) const;

};

}

#endif  // INPUTXSLT_SRC_FUNCTION_READ_FILE_H_
