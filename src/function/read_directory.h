#ifndef INPUTXSLT_SRC_FUNCTION_READ_DIRECTORY_H_
#define INPUTXSLT_SRC_FUNCTION_READ_DIRECTORY_H_

#include <boost/filesystem.hpp>

#include "base.h"

namespace InputXSLT {

class FunctionReadDirectory : public FunctionBase<
	FunctionReadDirectory,
	boost::filesystem::path
> {
	public:
		using FunctionBase::FunctionBase;

	protected:
		friend FunctionBase;

		DomDocumentCache::document_ptr constructDocument(
			boost::filesystem::path) const;

};

}

#endif  // INPUTXSLT_SRC_FUNCTION_READ_DIRECTORY_H_
