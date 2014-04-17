#include <xalanc/Include/PlatformDefinitions.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xalanc/XalanTransformer/XalanTransformer.hpp>
#include <xalanc/XPath/XObjectFactory.hpp>
#include <xalanc/XPath/Function.hpp>

#include "utility.h"

XALAN_USING_XALAN(Function)
XALAN_USING_XALAN(Locator)
XALAN_USING_XALAN(XPathExecutionContext)
XALAN_USING_XALAN(XalanDOMString)
XALAN_USING_XALAN(XalanNode)
XALAN_USING_XALAN(XObjectPtr)
XALAN_USING_XALAN(MemoryManager)
XALAN_USING_XALAN(XalanCopyConstruct)

class FunctionFileRead : public Function {
	public:
		virtual XObjectPtr execute(
			XPathExecutionContext&          executionContext,
			XalanNode*                      context,
			const XObjectArgVectorType&     args,
			const Locator*                  locator
		) const {
			if ( args.size() != 1 ) {
				XPathExecutionContext::GetAndReleaseCachedString guard(executionContext);

				generalError(executionContext, context, locator);
			}

			xalanc_1_11::CharVectorType tmpFileName;
			std::string fileName;

			args[0]->str().transcode(tmpFileName);
			std::move(tmpFileName.begin(), tmpFileName.end(), fileName.begin());

			std::string content(readFile(fileName));

			return executionContext.getXObjectFactory().createString(
				XalanDOMString(content.data())
			);
		}

		virtual FunctionFileRead* clone(MemoryManager& manager) const {
			return XalanCopyConstruct(manager, *this);
		}

	protected:
		const XalanDOMString& getError(XalanDOMString& result) const {
			result.assign("The read-file() function expects one argument.");

			return result;
		}

	private:
		FunctionFileRead& operator=(const FunctionFileRead&);
		bool operator==(const FunctionFileRead&) const;

};
