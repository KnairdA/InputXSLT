#ifndef INPUTXSLT_SRC_FUNCTION_TRANSFORM_H_
#define INPUTXSLT_SRC_FUNCTION_TRANSFORM_H_

#include "base.h"

namespace InputXSLT {

class FunctionTransform : public FunctionBase<
	FunctionTransform,
	std::string,
	std::string,
	xalan::XObjectPtr
> {
	public:
		using FunctionBase::FunctionBase;

	protected:
		friend FunctionBase;

		xercesc::DOMDocument* constructDocument(
			const FilesystemContext&,
			const FunctionBase::argument_tuple&
		);

};

}

#endif  // INPUTXSLT_SRC_FUNCTION_TRANSFORM_H_
