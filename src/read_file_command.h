#include <xalanc/Include/PlatformDefinitions.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xalanc/XalanTransformer/XalanTransformer.hpp>
#include <xalanc/XPath/XObjectFactory.hpp>
#include <xalanc/XPath/Function.hpp>
#include <xalanc/XPath/XObject.hpp>

#include "utility.h"

namespace xalan = xalanc_1_11;

class FunctionFileRead : public xalan::Function {
	public:
		virtual xalan::XObjectPtr execute(
			xalan::XPathExecutionContext&      executionContext,
			xalan::XalanNode*                  context,
			const xalan::Function::XObjectArgVectorType& args,
			const xalan::Locator*              locator
		) const {
			if ( args.size() != 1 ) {
				xalan::XPathExecutionContext::GetAndReleaseCachedString guard(
					executionContext
				);

				generalError(executionContext, context, locator);
			}

			xalan::CharVectorType tmpFileName;
			std::string fileName;

			args[0]->str().transcode(tmpFileName);

			std::move(
				tmpFileName.begin(),
				tmpFileName.end(),
				fileName.begin()
			);

			std::string content(readFile(fileName));

			return executionContext.getXObjectFactory().createString(
				xalan::XalanDOMString(content.data())
			);
		}

		virtual FunctionFileRead* clone(xalan::MemoryManager& manager) const {
			return xalan::XalanCopyConstruct(manager, *this);
		}

	protected:
		const xalan::XalanDOMString& getError(xalan::XalanDOMString& result) const {
			result.assign("The read-file() function expects one argument.");

			return result;
		}

	private:
		FunctionFileRead& operator=(const FunctionFileRead&);
		bool operator==(const FunctionFileRead&) const;

};
