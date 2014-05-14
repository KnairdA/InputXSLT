#ifndef INPUTXSLT_SRC_FUNCTION_BASE_H_
#define INPUTXSLT_SRC_FUNCTION_BASE_H_

#include <xalanc/XalanTransformer/XalanTransformer.hpp>
#include <xalanc/XPath/XObjectFactory.hpp>
#include <xalanc/XPath/Function.hpp>
#include <xalanc/XPath/XObject.hpp>

#include <memory>
#include <algorithm>
#include <array>

#include "common.h"
#include "support/xalan_string.h"
#include "support/dom/document_cache.h"
#include "support/filesystem_context.h"

namespace InputXSLT {

template <
	class Implementation,
	std::size_t ArgumentCount
>
class FunctionBase : public xalan::Function {
	public:
		typedef std::array<std::string, ArgumentCount> argument_array;

		FunctionBase():
			document_cache_(std::make_shared<DomDocumentCache>()) { }

		virtual xalan::XObjectPtr execute(
			xalan::XPathExecutionContext& executionContext,
			xalan::XalanNode* context,
			const XObjectArgVectorType& rawArguments,
			const xalan::Locator* locator
		) const {
			this->validateArguments(
				rawArguments,
				executionContext,
				context,
				locator
			);

			argument_array pathArguments;

			std::transform(
				rawArguments.begin(),
				rawArguments.end(),
				pathArguments.begin(),
				[](const xalan::XObjectPtr& ptr) -> std::string {
					return toString(ptr->str());
				}
			);

			xalan::XalanDocument* const domDocument(
				this->document_cache_->create(
					static_cast<Implementation*>(
						const_cast<FunctionBase*>(this)
					)->constructDocument(
						FilesystemContext(locator),
						pathArguments
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

	private:
		std::shared_ptr<DomDocumentCache> document_cache_;

		const xalan::XalanDOMString& getError(
			xalan::XalanDOMString& result) const {
			result.assign(std::string(
				"The function expects "        +
				std::to_string(ArgumentCount)  +
				" argument(s) of type string."
			).data());

			return result;
		}

		inline void validateArguments(
			const XObjectArgVectorType& rawArguments,
			xalan::XPathExecutionContext& executionContext,
			xalan::XalanNode* context,
			const xalan::Locator* locator
		) const {
			const bool anyNull = std::any_of(
				rawArguments.begin(),
				rawArguments.end(),
				[](const xalan::XObjectPtr& ptr) -> bool {
					return ptr.null();
				}
			);

			if ( rawArguments.size() != ArgumentCount || anyNull ) {
				xalan::XPathExecutionContext::GetAndReleaseCachedString guard(
					executionContext
				);

				this->generalError(executionContext, context, locator);
			}
		}

};

}

#endif  // INPUTXSLT_SRC_FUNCTION_BASE_H_
