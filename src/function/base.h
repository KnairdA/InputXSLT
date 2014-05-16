#ifndef INPUTXSLT_SRC_FUNCTION_BASE_H_
#define INPUTXSLT_SRC_FUNCTION_BASE_H_

#include <xalanc/XPath/XObjectFactory.hpp>
#include <xalanc/XPath/Function.hpp>
#include <xalanc/XPath/XObject.hpp>

#include <memory>
#include <tuple>

#include "common.h"
#include "support/xalan_string.h"
#include "support/tuple/mapper.h"
#include "support/dom/document_cache.h"
#include "support/filesystem_context.h"

namespace InputXSLT {

template <
	typename Implementation,
	typename... Types
>
class FunctionBase : public xalan::Function {
	public:
		typedef std::tuple<Types...> argument_tuple;

		FunctionBase():
			document_cache_(std::make_shared<DomDocumentCache>()) { }

		virtual xalan::XObjectPtr execute(
			xalan::XPathExecutionContext& executionContext,
			xalan::XalanNode* context,
			const XObjectArgVectorType& arguments,
			const xalan::Locator* locator
		) const {
			this->validateArguments(
				arguments,
				executionContext,
				context,
				locator
			);

			xalan::XalanDocument* const domDocument(
				this->document_cache_->create(
					static_cast<Implementation*>(
						const_cast<FunctionBase*>(this)
					)->constructDocument(
						FilesystemContext(locator),
						Mapper::template construct<argument_tuple>(arguments)
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
				"The function expects "                                +
				std::to_string(std::tuple_size<argument_tuple>::value) +
				" argument(s)"
			).data());

			return result;
		}

		inline void validateArguments(
			const XObjectArgVectorType& arguments,
			xalan::XPathExecutionContext& executionContext,
			xalan::XalanNode* context,
			const xalan::Locator* locator
		) const {
			const bool anyNull = std::any_of(
				arguments.begin(),
				arguments.end(),
				[](const xalan::XObjectPtr& ptr) -> bool {
					return ptr.null();
				}
			);

			if ( arguments.size() != std::tuple_size<argument_tuple>::value || anyNull ) {
				xalan::XPathExecutionContext::GetAndReleaseCachedString guard(
					executionContext
				);

				this->generalError(executionContext, context, locator);
			}
		}

};

}

#endif  // INPUTXSLT_SRC_FUNCTION_BASE_H_
