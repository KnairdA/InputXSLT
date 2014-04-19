#ifndef INPUTXSLT_SRC_FUNCTION_READ_XML_FILE_H_
#define INPUTXSLT_SRC_FUNCTION_READ_XML_FILE_H_

#include <xalanc/Include/PlatformDefinitions.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xalanc/XalanTransformer/XalanTransformer.hpp>
#include <xalanc/XPath/XObjectFactory.hpp>
#include <xalanc/XPath/Function.hpp>
#include <xalanc/XPath/XObject.hpp>
#include <xalanc/XercesParserLiaison/XercesParserLiaison.hpp>

#include "utility.h"

namespace InputXSLT {

class FunctionReadXmlFile : public xalan::Function {
	public:
		FunctionReadXmlFile();
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
		mutable xalan::XercesParserLiaison parser_;

		const xalan::XalanDOMString& getError(xalan::XalanDOMString& result) const;

};

}

#endif  // INPUTXSLT_SRC_FUNCTION_READ_XML_FILE_H_
