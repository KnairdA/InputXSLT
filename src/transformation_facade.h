#ifndef INPUTXSLT_SRC_TRANSFORMATION_FACADE_H_
#define INPUTXSLT_SRC_TRANSFORMATION_FACADE_H_

#include <xalanc/XalanTransformer/XalanTransformer.hpp>

#include <string>

#include "common.h"
#include "support/error_handler.h"
#include "support/include_entity_resolver.h"
#include "support/stylesheet_parameter_guard.h"

namespace InputXSLT {

class TransformationFacade {
	public:
		typedef ErrorHandler::error_cache_ptr return_type;

		TransformationFacade(const std::string&, IncludeEntityResolver*);
		~TransformationFacade();

		template <typename Target>
		return_type generate(Target&);

		template <typename Target>
		return_type generate(Target&, const StylesheetParameterGuard::map&);

		template <typename Target>
		return_type generate(Target&, const xalan::XObjectPtr&);

	private:
		const xalan::XalanCompiledStylesheet* transformation_;

		xalan::XalanTransformer transformer_;
		ErrorHandler error_handler_;

		return_type generate(const std::string&, StylesheetParameterGuard&);
		return_type generate(std::basic_ostream<char>&, StylesheetParameterGuard&);
		return_type generate(xalan::XSLTResultTarget&&, StylesheetParameterGuard&);

};

template <typename Target>
auto TransformationFacade::generate(Target& target) -> return_type {
	StylesheetParameterGuard guard(this->transformer_);

	return this->generate(target, guard);
}

template <typename Target>
auto TransformationFacade::generate(
	Target& target,
	const StylesheetParameterGuard::map& parameters
) -> return_type {
	StylesheetParameterGuard guard(this->transformer_, parameters);

	return this->generate(target, guard);
}

template <typename Target>
auto TransformationFacade::generate(
	Target& target,
	const xalan::XObjectPtr& parameter
) -> return_type {
	StylesheetParameterGuard guard(this->transformer_);
	guard.set("parameters", parameter);

	return this->generate(target, guard);
}

}

#endif  // INPUTXSLT_SRC_TRANSFORMATION_FACADE_H_
