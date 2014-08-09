#ifndef INPUTXSLT_SRC_FUNCTION_EXTERNAL_TEXT_FORMATTER_H_
#define INPUTXSLT_SRC_FUNCTION_EXTERNAL_TEXT_FORMATTER_H_

#include <boost/filesystem.hpp>

#include "base.h"

namespace InputXSLT {

class FunctionExternalTextFormatter : public FunctionBase<
	FunctionExternalTextFormatter,
	std::string,
	std::string
> {
	public:
		using FunctionBase::FunctionBase;

	protected:
		friend FunctionBase;

		DomDocumentCache::document_ptr constructDocument(
			std::string,
			std::string
		) const;

};

}

#endif  // INPUTXSLT_SRC_FUNCTION_EXTERNAL_TEXT_FORMATTER_H_
