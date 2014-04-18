#include <xalanc/Include/PlatformDefinitions.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xalanc/XalanTransformer/XalanTransformer.hpp>
#include <xalanc/XPath/XObjectFactory.hpp>
#include <xalanc/XPath/Function.hpp>
#include <xalanc/XPath/XObject.hpp>
#include <xalanc/XercesParserLiaison/XercesParserLiaison.hpp>

#include "utility.h"

namespace xalan = xalanc_1_11;

class FunctionReadXmlFile : public xalan::Function {
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

			if ( this->liaison == nullptr ) {
				const_cast<FunctionReadXmlFile*>(this)->liaison = new xalan::XercesParserLiaison();
			}

			xalan::XSLTInputSource file(args[0]->str());

			return executionContext.getXObjectFactory().createNodeSet(
				liaison->parseXMLStream(file)
			);
		}

		virtual FunctionReadXmlFile* clone(xalan::MemoryManager& manager) const {
			return xalan::XalanCopyConstruct(manager, *this);
		}

	protected:
		xalan::XercesParserLiaison* liaison = nullptr;

		const xalan::XalanDOMString& getError(xalan::XalanDOMString& result) const {
			result.assign("The read-file() function expects one argument.");

			return result;
		}

	private:
		FunctionReadXmlFile& operator=(const FunctionReadXmlFile&);
		bool operator==(const FunctionReadXmlFile&) const;

};
