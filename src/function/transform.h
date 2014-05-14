#ifndef INPUTXSLT_SRC_FUNCTION_TRANSFORM_H_
#define INPUTXSLT_SRC_FUNCTION_TRANSFORM_H_

#include "base.h"

namespace InputXSLT {

class FunctionTransform : public FunctionBase<FunctionTransform, 2> {
	public:
		using FunctionBase::FunctionBase;

	protected:
		friend FunctionBase;

		xercesc::DOMDocument* constructDocument(
			const FilesystemContext&,
			const FunctionBase::argument_array&
		);

};

}

#endif  // INPUTXSLT_SRC_FUNCTION_TRANSFORM_H_
