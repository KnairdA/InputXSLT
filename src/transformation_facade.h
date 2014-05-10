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

		int generate(const std::string&);
		int generate(
			const std::string&,
			const StylesheetParameterGuard::map&
		);

	private:
		const xalan::XalanCompiledStylesheet* transformation_;

		xalan::XalanTransformer transformer_;

		int generate(const std::string&, StylesheetParameterGuard&);

};

}

#endif  // INPUTXSLT_SRC_TRANSFORMATION_FACADE_H_
