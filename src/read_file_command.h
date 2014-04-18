#include <xalanc/Include/PlatformDefinitions.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xalanc/XalanTransformer/XalanTransformer.hpp>
#include <xalanc/XPath/XObjectFactory.hpp>
#include <xalanc/XPath/Function.hpp>
#include <xalanc/XPath/XObject.hpp>

#include "utility.h"

namespace InputXSLT {

class FunctionReadFile : public xalan::Function {
	public:
		virtual xalan::XObjectPtr execute(
			xalan::XPathExecutionContext&                executionContext,
			xalan::XalanNode*                            context,
			const xalan::Function::XObjectArgVectorType& arguments,
			const xalan::Locator*              locator
		) const {
			if ( arguments.size() != 1 ) {
				xalan::XPathExecutionContext::GetAndReleaseCachedString guard(
					executionContext
				);

				generalError(executionContext, context, locator);
			}

			xalan::CharVectorType fileNameVector;
			std::string fileNameString;

			arguments[0]->str().transcode(fileNameVector);

			std::move(
				fileNameVector.begin(),
				fileNameVector.end(),
				fileNameString.begin()
			);

			return executionContext.getXObjectFactory().createString(
				xalan::XalanDOMString(
					InputXSLT::readFile(fileNameString).data()
				)
			);
		}

		virtual FunctionReadFile* clone(xalan::MemoryManager& manager) const {
			return xalan::XalanCopyConstruct(manager, *this);
		}

		FunctionReadFile& operator=(const FunctionReadFile&) = delete;
		bool operator==(const FunctionReadFile&) const       = delete;

	private:
		const xalan::XalanDOMString& getError(xalan::XalanDOMString& result) const {
			result.assign("The read-file() function expects one argument.");

			return result;
		}

};

}
