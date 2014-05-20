#ifndef INPUTXSLT_SRC_FUNCTION_TRANSFORM_H_
#define INPUTXSLT_SRC_FUNCTION_TRANSFORM_H_

#include "base.h"

#include "support/include_entity_resolver.h"

namespace InputXSLT {

class FunctionTransform : public FunctionBase<
	FunctionTransform,
	std::string,
	std::string,
	xalan::XObjectPtr
> {
	public:
		FunctionTransform(IncludeEntityResolver*);

	protected:
		friend FunctionBase;

		xercesc::DOMDocument* constructDocument(
			const FilesystemContext&,
			const FunctionBase::parameter_tuple&
		);

	private:
		IncludeEntityResolver* const include_resolver_;

};

}

#endif  // INPUTXSLT_SRC_FUNCTION_TRANSFORM_H_
