#ifndef INPUTXSLT_SRC_FUNCTION_READ_DIRECTORY_H_
#define INPUTXSLT_SRC_FUNCTION_READ_DIRECTORY_H_

#include "base.h"

namespace InputXSLT {

class FunctionReadDirectory : public FunctionBase<
	FunctionReadDirectory,
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

#endif  // INPUTXSLT_SRC_FUNCTION_READ_DIRECTORY_H_
