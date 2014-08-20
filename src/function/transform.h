#ifndef INPUTXSLT_SRC_FUNCTION_TRANSFORM_H_
#define INPUTXSLT_SRC_FUNCTION_TRANSFORM_H_

#include <xalanc/XSLT/XSLTInputSource.hpp>

#include "base.h"

namespace InputXSLT {

class FunctionTransform : public FunctionBase<
	FunctionTransform,
	xalan::XSLTInputSource,
	xalan::XSLTInputSource
> {
	public:
		using FunctionBase::FunctionBase;

	protected:
		friend FunctionBase;

		DomDocumentCache::document_ptr constructDocument(
			const FilesystemContext&,
			xalan::XSLTInputSource,
			xalan::XSLTInputSource
		) const;

};

}

#endif  // INPUTXSLT_SRC_FUNCTION_TRANSFORM_H_
