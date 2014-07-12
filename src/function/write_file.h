#ifndef INPUTXSLT_SRC_FUNCTION_WRITE_FILE_H_
#define INPUTXSLT_SRC_FUNCTION_WRITE_FILE_H_

#include "base.h"

namespace InputXSLT {

class FunctionWriteFile : public FunctionBase<
	FunctionWriteFile,
	boost::filesystem::path,
	xalan::XalanNode*
> {
	public:
		using FunctionBase::FunctionBase;

	protected:
		friend FunctionBase;

		DomDocumentCache::document_ptr constructDocument(
			boost::filesystem::path,
			xalan::XalanNode* const
		) const;

};

}

#endif  // INPUTXSLT_SRC_FUNCTION_WRITE_FILE_H_
