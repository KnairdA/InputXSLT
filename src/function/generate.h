#ifndef INPUTXSLT_SRC_FUNCTION_GENERATE_H_
#define INPUTXSLT_SRC_FUNCTION_GENERATE_H_

#include <xalanc/XSLT/XSLTInputSource.hpp>

#include "base.h"

namespace InputXSLT {

class FunctionGenerate : public FunctionBase<
	FunctionGenerate,
	xalan::XSLTInputSource,
	xalan::XSLTInputSource,
	boost::optional<boost::filesystem::path>
> {
	public:
		using FunctionBase::FunctionBase;

	protected:
		friend FunctionBase;

		DomDocumentCache::document_ptr constructDocument(
			const FilesystemContext&,
			xalan::XSLTInputSource,
			xalan::XSLTInputSource,
			boost::optional<boost::filesystem::path>
		) const;

};

}

#endif  // INPUTXSLT_SRC_FUNCTION_GENERATE_H_
