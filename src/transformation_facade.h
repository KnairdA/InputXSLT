#ifndef INPUTXSLT_SRC_TRANSFORMATION_FACADE_H_
#define INPUTXSLT_SRC_TRANSFORMATION_FACADE_H_

#include <xalanc/XalanTransformer/XalanTransformer.hpp>

#include <string>
#include <memory>
#include <sstream>
#include <functional>

#include "common.h"
#include "support/include_entity_resolver.h"
#include "support/stylesheet_parameter_guard.h"
#include "support/error/error_multiplexer.h"
#include "support/error/error_capacitor.h"
#include "support/error/warning_capacitor.h"

namespace InputXSLT {

class TransformationFacade {
	public:
		typedef std::unique_ptr<TransformationFacade> ptr;

		template <typename... Arguments>
		static ptr try_create(
			const std::function<void(const ErrorCapacitor::error_cache&)>&,
			Arguments&&...
		);

		TransformationFacade(
			xalan::XSLTInputSource,
			IncludeEntityResolver*
		);

		TransformationFacade(
			xalan::XSLTInputSource,
			xalan::XSLTInputSource,
			IncludeEntityResolver*
		);

		void generate(std::basic_ostream<char>&);
		void generate(std::basic_ostream<char>&, const StylesheetParameterGuard::map&);
		void generate(std::basic_ostream<char>&, const xalan::XObjectPtr&);

		WarningCapacitor::warning_cache_ptr getCachedWarnings();

	private:
		const xalan::XalanParsedSource* input_;
		const xalan::XalanCompiledStylesheet* transformation_;

		xalan::XalanTransformer transformer_;
		ErrorMultiplexer error_multiplexer_;
		WarningCapacitor warning_capacitor_;

		void generate(
			std::basic_ostream<char>&,
			StylesheetParameterGuard&
		);

};

template <typename... Arguments>
auto TransformationFacade::try_create(
	const std::function<void(const ErrorCapacitor::error_cache&)>& handleErrors,
	Arguments&&... arguments
) -> ptr {
	try {
		return ptr(
			new InputXSLT::TransformationFacade(
				std::forward<Arguments>(arguments)...
			)
		);
	}
	catch (const ErrorCapacitor::exception& exception) {
		handleErrors(*exception);

		return ptr();
	}
}

}

#endif  // INPUTXSLT_SRC_TRANSFORMATION_FACADE_H_
