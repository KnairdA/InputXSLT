#ifndef INPUTXSLT_SRC_FUNCTION_READ_FILE_H_
#define INPUTXSLT_SRC_FUNCTION_READ_FILE_H_

#include <xalanc/XalanTransformer/XalanTransformer.hpp>
#include <xalanc/XPath/XObjectFactory.hpp>
#include <xalanc/XPath/Function.hpp>
#include <xalanc/XPath/XObject.hpp>

#include "boost/filesystem.hpp"

#include <string>

#include "common.h"

namespace InputXSLT {

class FunctionReadFile : public xalan::Function {
	public:
		FunctionReadFile(const std::string&);

		virtual xalan::XObjectPtr execute(
			xalan::XPathExecutionContext&,
			xalan::XalanNode*,
			const xalan::Function::XObjectArgVectorType&,
			const xalan::Locator*
		) const;

		virtual FunctionReadFile* clone(xalan::MemoryManager&) const;

		FunctionReadFile& operator=(const FunctionReadFile&) = delete;
		bool operator==(const FunctionReadFile&) const       = delete;

	private:
		const boost::filesystem::path path_;

		const xalan::XalanDOMString& getError(xalan::XalanDOMString&) const;

};

}

#endif  // INPUTXSLT_SRC_FUNCTION_READ_FILE_H_
