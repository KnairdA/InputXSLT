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
#include "support/tuple/mapper.h"
#include "support/dom/document_cache.h"

namespace InputXSLT {

template <
	typename Implementation,
	typename... Types
>
class FunctionBase : public xalan::Function {
	public:
		typedef std::tuple<Types...> parameter_tuple;

		FunctionBase(IncludeEntityResolver* resolver):
			include_resolver_(resolver),
			document_cache_(std::make_shared<DomDocumentCache>()) { }

		virtual xalan::XObjectPtr execute(
			xalan::XPathExecutionContext& executionContext,
			xalan::XalanNode* context,
			const XObjectArgVectorType& parameters,
			const xalan::Locator* locator
		) const {
			this->validateParameters(
				parameters,
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
						Mapper::template construct<parameter_tuple>(parameters)
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
		IncludeEntityResolver* const include_resolver_;

	private:
		std::shared_ptr<DomDocumentCache> document_cache_;

		const xalan::XalanDOMString& getError(
			xalan::XalanDOMString& result) const {
			result.assign(std::string(
				"The function expects "                                 +
				std::to_string(std::tuple_size<parameter_tuple>::value) +
				" parameter(s)"
			).data());

			return result;
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

			if ( parameters.size() != std::tuple_size<parameter_tuple>::value || anyNull ) {
				xalan::XPathExecutionContext::GetAndReleaseCachedString guard(
					executionContext
				);

				this->generalError(executionContext, context, locator);
			}
		}

};

}

#endif  // INPUTXSLT_SRC_FUNCTION_BASE_H_
