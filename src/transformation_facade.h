#ifndef INPUTXSLT_SRC_TRANSFORMATION_FACADE_H_
#define INPUTXSLT_SRC_TRANSFORMATION_FACADE_H_

#include <xalanc/XalanTransformer/XalanTransformer.hpp>

#include <string>

#include "common.h"
#include "support/stylesheet_parameter_guard.h"

namespace InputXSLT {

class TransformationFacade {
	public:
		explicit TransformationFacade(const std::string&);
		~TransformationFacade();

		template <typename Target>
		int generate(Target&);

		template <typename Target>
		int generate(Target&, const StylesheetParameterGuard::map&);

		template <typename Target>
		int generate(Target&, const xalan::XObjectPtr&);

	private:
		const xalan::XalanCompiledStylesheet* transformation_;

		xalan::XalanTransformer transformer_;

		int generate(const std::string&, StylesheetParameterGuard&);
		int generate(std::basic_ostream<char>&, StylesheetParameterGuard&);
		int generate(xalan::XSLTResultTarget&&, StylesheetParameterGuard&);

};

template <typename Target>
int TransformationFacade::generate(Target& target) {
	StylesheetParameterGuard guard(this->transformer_);

	return this->generate(target, guard);
}

template <typename Target>
int TransformationFacade::generate(
	Target& target,
	const StylesheetParameterGuard::map& parameters
) {
	StylesheetParameterGuard guard(this->transformer_, parameters);

	return this->generate(target, guard);
}

template <typename Target>
int TransformationFacade::generate(
	Target& target,
	const xalan::XObjectPtr& parameter
) {
	StylesheetParameterGuard guard(this->transformer_);
	guard.set("parameters", parameter);

	return this->generate(target, guard);
}

}

#endif  // INPUTXSLT_SRC_TRANSFORMATION_FACADE_H_
