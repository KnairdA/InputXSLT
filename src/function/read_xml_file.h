#ifndef INPUTXSLT_SRC_FUNCTION_READ_XML_FILE_H_
#define INPUTXSLT_SRC_FUNCTION_READ_XML_FILE_H_

#include <xalanc/XalanTransformer/XalanTransformer.hpp>
#include <xalanc/XPath/XObjectFactory.hpp>
#include <xalanc/XPath/Function.hpp>
#include <xalanc/XPath/XObject.hpp>
#include <xalanc/XercesParserLiaison/XercesParserLiaison.hpp>

#include "boost/filesystem.hpp"

#include <string>

#include "common.h"

namespace InputXSLT {

class FunctionReadXmlFile : public xalan::Function {
	public:
		FunctionReadXmlFile(const std::string&);
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
		const boost::filesystem::path path_;
		mutable xalan::XercesParserLiaison parser_;

		const xalan::XalanDOMString& getError(xalan::XalanDOMString& result) const;

};

}

#endif  // INPUTXSLT_SRC_FUNCTION_READ_XML_FILE_H_
