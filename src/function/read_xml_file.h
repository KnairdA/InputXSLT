#ifndef INPUTXSLT_SRC_FUNCTION_READ_XML_FILE_H_
#define INPUTXSLT_SRC_FUNCTION_READ_XML_FILE_H_

#include <xalanc/XalanTransformer/XalanTransformer.hpp>
#include <xalanc/XPath/XObjectFactory.hpp>
#include <xalanc/XPath/Function.hpp>
#include <xalanc/XPath/XObject.hpp>
#include <xalanc/XercesParserLiaison/XercesParserLiaison.hpp>

#include <string>

#include "common.h"
#include "support/filesystem_context.h"

namespace InputXSLT {

class FunctionReadXmlFile : public xalan::Function {
	public:
		FunctionReadXmlFile(const FilesystemContext&);
		FunctionReadXmlFile(const FunctionReadXmlFile&);

		virtual xalan::XObjectPtr execute(
			xalan::XPathExecutionContext&                executionContext,
			xalan::XalanNode*                            context,
			const xalan::Function::XObjectArgVectorType& arguments,
			const xalan::Locator*                        locator
		) const;

		virtual FunctionReadXmlFile* clone(xalan::MemoryManager&) const;

		FunctionReadXmlFile& operator=(const FunctionReadXmlFile&) = delete;
		bool operator==(const FunctionReadXmlFile&) const          = delete;

	private:
		const FilesystemContext& fs_context_;
		mutable xalan::XercesParserLiaison parser_;

		const xalan::XalanDOMString& getError(xalan::XalanDOMString& result) const;

};

}

#endif  // INPUTXSLT_SRC_FUNCTION_READ_XML_FILE_H_
