#include <xalanc/Include/PlatformDefinitions.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xalanc/XalanTransformer/XalanTransformer.hpp>
#include <xalanc/XPath/XObjectFactory.hpp>
#include <xalanc/XPath/Function.hpp>
#include <xalanc/XPath/XObject.hpp>
#include <xalanc/XercesParserLiaison/XercesParserLiaison.hpp>

#include "utility.h"

#include <memory>

namespace InputXSLT {

class FunctionReadXmlFile : public xalan::Function {
	public:
		virtual xalan::XObjectPtr execute(
			xalan::XPathExecutionContext&                executionContext,
			xalan::XalanNode*                            context,
			const xalan::Function::XObjectArgVectorType& arguments,
			const xalan::Locator*                        locator
		) const {
			if ( arguments.size() != 1 ) {
				xalan::XPathExecutionContext::GetAndReleaseCachedString guard(
					executionContext
				);

				generalError(executionContext, context, locator);
			}

			if ( !this->parser_ ) {
				this->parser_ = std::make_shared<xalan::XercesParserLiaison>();
			}

			return executionContext.getXObjectFactory().createNodeSet(
				this->parser_->parseXMLStream(
					xalan::XSLTInputSource(arguments[0]->str())
				)
			);
		}

		virtual FunctionReadXmlFile* clone(xalan::MemoryManager& manager) const {
			return xalan::XalanCopyConstruct(manager, *this);
		}

		FunctionReadXmlFile& operator=(const FunctionReadXmlFile&) = delete;
		bool operator==(const FunctionReadXmlFile&) const          = delete;

	private:
		mutable std::shared_ptr<xalan::XercesParserLiaison> parser_;

		const xalan::XalanDOMString& getError(xalan::XalanDOMString& result) const {
			result.assign("The read-xml-file() function expects one argument.");

			return result;
		}

};

}
