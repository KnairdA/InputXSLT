#ifndef INPUTXSLT_SRC_TRANSFORMATION_FACADE_H_
#define INPUTXSLT_SRC_TRANSFORMATION_FACADE_H_

#include <xalanc/XalanTransformer/XalanTransformer.hpp>

#include <string>
#include <memory>
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

		static ptr try_create(
			const std::string&,
			IncludeEntityResolver*,
			const std::function<void(const ErrorCapacitor::error_cache&)>&
		);

		TransformationFacade(const std::string&, IncludeEntityResolver*);
		~TransformationFacade();

		template <typename Target>
		void generate(Target&);

		template <typename Target>
		void generate(Target&, const StylesheetParameterGuard::map&);

		template <typename Target>
		void generate(Target&, const xalan::XObjectPtr&);

		WarningCapacitor::warning_cache_ptr getCachedWarnings();

	private:
		const xalan::XalanCompiledStylesheet* transformation_;

		xalan::XalanTransformer transformer_;
		ErrorMultiplexer error_multiplexer_;
		WarningCapacitor warning_capacitor_;

		void generate(const std::string&, StylesheetParameterGuard&);
		void generate(std::basic_ostream<char>&, StylesheetParameterGuard&);
		void generate(xalan::XSLTResultTarget&&, StylesheetParameterGuard&);

};

template <typename Target>
void TransformationFacade::generate(Target& target) {
	StylesheetParameterGuard guard(this->transformer_);

	this->generate(target, guard);
}

template <typename Target>
void TransformationFacade::generate(
	Target& target,
	const StylesheetParameterGuard::map& parameters
) {
	StylesheetParameterGuard guard(this->transformer_, parameters);

	this->generate(target, guard);
}

template <typename Target>
void TransformationFacade::generate(
	Target& target,
	const xalan::XObjectPtr& parameter
) {
	StylesheetParameterGuard guard(this->transformer_);
	guard.set("parameters", parameter);

	this->generate(target, guard);
}

}

#endif  // INPUTXSLT_SRC_TRANSFORMATION_FACADE_H_
