#ifndef INPUTXSLT_SRC_FUNCTION_EXTERNAL_TEXT_FORMATTER_H_
#define INPUTXSLT_SRC_FUNCTION_EXTERNAL_TEXT_FORMATTER_H_

#include <boost/filesystem.hpp>

#include "base.h"

namespace InputXSLT {

class FunctionExternalTextFormatter : public FunctionBase<
	FunctionExternalTextFormatter,
	boost::filesystem::path,
	std::string
> {
	public:
		using FunctionBase::FunctionBase;

	protected:
		friend FunctionBase;

		DomDocumentCache::document_ptr constructDocument(
			boost::filesystem::path,
			std::string
		);

};

}

#endif  // INPUTXSLT_SRC_FUNCTION_EXTERNAL_TEXT_FORMATTER_H_
