#ifndef INPUTXSLT_SRC_FUNCTION_TRANSFORM_H_
#define INPUTXSLT_SRC_FUNCTION_TRANSFORM_H_

#include <xalanc/XSLT/XSLTInputSource.hpp>

#include "base.h"

namespace InputXSLT {

class FunctionTransform : public FunctionBase<
	FunctionTransform,
	xalan::XSLTInputSource,
	std::string,
	xalan::XObjectPtr
> {
	public:
		using FunctionBase::FunctionBase;

	protected:
		friend FunctionBase;

		xercesc::DOMDocument* constructDocument(
			const FilesystemContext&,
			xalan::XSLTInputSource,
			std::string,
			xalan::XObjectPtr
		);

};

}

#endif  // INPUTXSLT_SRC_FUNCTION_TRANSFORM_H_
