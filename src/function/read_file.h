#ifndef INPUTXSLT_SRC_FUNCTION_READ_FILE_H_
#define INPUTXSLT_SRC_FUNCTION_READ_FILE_H_

#include "base.h"

namespace InputXSLT {

class FunctionReadFile : public FunctionBase<
	FunctionReadFile,
	std::string
> {
	public:
		using FunctionBase::FunctionBase;

	protected:
		friend FunctionBase;

		xercesc::DOMDocument* constructDocument(
			const FilesystemContext&,
			std::string
		);

};

}

#endif  // INPUTXSLT_SRC_FUNCTION_READ_FILE_H_
