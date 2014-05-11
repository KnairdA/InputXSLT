#ifndef INPUTXSLT_SRC_FUNCTION_BASE_H_
#define INPUTXSLT_SRC_FUNCTION_BASE_H_

#include <xalanc/XalanTransformer/XalanTransformer.hpp>
#include <xalanc/XPath/XObjectFactory.hpp>
#include <xalanc/XPath/Function.hpp>
#include <xalanc/XPath/XObject.hpp>

#include <memory>

#include "common.h"
#include "support/dom/document_cache.h"
#include "support/filesystem_context.h"

namespace InputXSLT {

template <class Implementation>
class FunctionBase : public xalan::Function {
	public:
		FunctionBase():
			document_cache_(std::make_shared<DomDocumentCache>()) { }

		virtual xalan::XObjectPtr execute(
			xalan::XPathExecutionContext& executionContext,
			xalan::XalanNode*,
			const xalan::XObjectPtr argument,
			const xalan::Locator* locator
		) const {
			const FilesystemContext fsContext(locator);

			xalan::XalanDocument* const domDocument(
				this->document_cache_->create(
					static_cast<Implementation*>(
						const_cast<FunctionBase*>(this)
					)->constructDocument(
						fsContext,
						fsContext.resolve(argument->str())
					)
				)
			);

			xalan::XPathExecutionContext::BorrowReturnMutableNodeRefList nodeList(
				executionContext
			);

			nodeList->addNodes(
				*domDocument->getDocumentElement()->getChildNodes()
			);

			return executionContext.getXObjectFactory().createNodeSet(nodeList);
		}

		virtual FunctionBase* clone(
			xalan::MemoryManager& manager) const {
			return xalan::XalanCopyConstruct(
				manager,
				static_cast<const Implementation&>(*this)
			);
		}

		FunctionBase& operator=(const FunctionBase&) = delete;
		bool operator==(const FunctionBase&) const   = delete;

	protected:
		std::shared_ptr<DomDocumentCache> document_cache_;

		const xalan::XalanDOMString& getError(
			xalan::XalanDOMString& result) const {
			result.assign("The function expects one argument of type string.");

			return result;
		}

};

}

#endif  // INPUTXSLT_SRC_FUNCTION_BASE_H_
