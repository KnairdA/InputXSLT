#ifndef INPUTXSLT_SRC_FUNCTION_TRANSFORM_H_
#define INPUTXSLT_SRC_FUNCTION_TRANSFORM_H_

#include <xalanc/XSLT/XSLTInputSource.hpp>

#include "boost/filesystem.hpp"

#include "base.h"

namespace InputXSLT {

class FunctionTransform : public FunctionBase<
	FunctionTransform,
	xalan::XSLTInputSource,
	boost::filesystem::path,
	xalan::XObjectPtr
> {
	public:
		using FunctionBase::FunctionBase;

	protected:
		friend FunctionBase;

		xercesc::DOMDocument* constructDocument(
			xalan::XSLTInputSource,
			boost::filesystem::path,
			xalan::XObjectPtr
		);

};

}

#endif  // INPUTXSLT_SRC_FUNCTION_TRANSFORM_H_
