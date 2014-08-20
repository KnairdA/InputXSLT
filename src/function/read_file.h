#ifndef INPUTXSLT_SRC_FUNCTION_READ_FILE_H_
#define INPUTXSLT_SRC_FUNCTION_READ_FILE_H_

#include <boost/filesystem.hpp>

#include "base.h"

namespace InputXSLT {

class FunctionReadFile : public FunctionBase<
	FunctionReadFile,
	boost::filesystem::path
> {
	public:
		using FunctionBase::FunctionBase;

	protected:
		friend FunctionBase;

		DomDocumentCache::document_ptr constructDocument(
			const FilesystemContext&,
			boost::filesystem::path
		) const;

};

}

#endif  // INPUTXSLT_SRC_FUNCTION_READ_FILE_H_
