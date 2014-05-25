#ifndef INPUTXSLT_SRC_FUNCTION_EXECUTE_H_
#define INPUTXSLT_SRC_FUNCTION_EXECUTE_H_

#include "base.h"

namespace InputXSLT {

class FunctionExecute : public FunctionBase<
	FunctionExecute,
	std::string,
	std::string
> {
	public:
		using FunctionBase::FunctionBase;

	protected:
		friend FunctionBase;

		xercesc::DOMDocument* constructDocument(
			const FilesystemContext&,
			const FunctionBase::parameter_tuple&
		);

};

}

#endif  // INPUTXSLT_SRC_FUNCTION_EXECUTE_H_
