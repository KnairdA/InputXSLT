#ifndef INPUTXSLT_SRC_FUNCTION_BASE_H_
#define INPUTXSLT_SRC_FUNCTION_BASE_H_

#include <xalanc/XPath/XObjectFactory.hpp>
#include <xalanc/XPath/Function.hpp>
#include <xalanc/XPath/XObject.hpp>

#include <memory>
#include <tuple>

#include "common.h"
#include "support/xalan_string.h"
#include "support/filesystem_context.h"
#include "support/include_entity_resolver.h"
#include "support/dom/document_cache.h"
#include "support/type/sequence.h"
#include "support/type/xobject_value.h"

namespace InputXSLT {

template <
	typename Implementation,
	typename... Types
>
class FunctionBase : public xalan::Function {
	static const std::size_t parameter_count = sizeof...(Types);

	public:
		FunctionBase(IncludeEntityResolver* resolver):
			include_resolver_(resolver),
			document_cache_(std::make_shared<DomDocumentCache>()) { }

		virtual xalan::XObjectPtr execute(
			xalan::XPathExecutionContext& executionContext,
			xalan::XalanNode*             context,
			const XObjectArgVectorType&   parameters,
			const xalan::Locator*         locator
		) const {
			this->validateParameters(
				parameters,
				executionContext,
				context,
				locator
			);

			xalan::XalanDocument* const domDocument(
				this->callConstructDocument(
					parameters,
					locator,
					typename IndexSequence<parameter_count>::type()
				)
			);

			xalan::XPathExecutionContext::BorrowReturnMutableNodeRefList nodes(
				executionContext
			);

			nodes->addNodes(
				*domDocument->getDocumentElement()->getChildNodes()
			);

			return executionContext.getXObjectFactory().createNodeSet(nodes);
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
		IncludeEntityResolver* const include_resolver_;

	private:
		std::shared_ptr<DomDocumentCache> document_cache_;

		const xalan::XalanDOMString& getError(
			xalan::XalanDOMString& result) const {
			result.assign(std::string(
				"The function expects "         +
				std::to_string(parameter_count) +
				" parameter(s)"
			).data());

			return result;
		}

		template <std::size_t... Index>
		inline xalan::XalanDocument* callConstructDocument(
			const XObjectArgVectorType& parameters,
			const xalan::Locator* locator,
			Sequence<Index...>
		) const {
			XObjectValue valueGetter(
				this->include_resolver_->resolve(
					locator->getSystemId()
				),
				this->include_resolver_
			);

			return this->document_cache_->create(
				static_cast<Implementation*>(
					const_cast<FunctionBase*>(this)
				)->constructDocument(
					valueGetter.get<typename std::tuple_element<
						Index,
						std::tuple<Types...>
					>::type>(parameters[Index])...
				)
			);
		}

		inline void validateParameters(
			const XObjectArgVectorType& parameters,
			xalan::XPathExecutionContext& executionContext,
			xalan::XalanNode* context,
			const xalan::Locator* locator
		) const {
			const bool anyNull = std::any_of(
				parameters.begin(),
				parameters.end(),
				[](const xalan::XObjectPtr& ptr) -> bool {
					return ptr.null();
				}
			);

			if ( parameters.size() != parameter_count || anyNull ) {
				xalan::XPathExecutionContext::GetAndReleaseCachedString guard(
					executionContext
				);

				this->generalError(executionContext, context, locator);
			}
		}

};

}

#endif  // INPUTXSLT_SRC_FUNCTION_BASE_H_
