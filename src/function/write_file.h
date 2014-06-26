#ifndef INPUTXSLT_SRC_FUNCTION_WRITE_FILE_H_
#define INPUTXSLT_SRC_FUNCTION_WRITE_FILE_H_

#include "base.h"

namespace InputXSLT {

class FunctionWriteFile : public FunctionBase<
	FunctionWriteFile,
	boost::filesystem::path,
	std::string
> {
	public:
		using FunctionBase::FunctionBase;

	protected:
		friend FunctionBase;

		xercesc::DOMDocument* constructDocument(
			boost::filesystem::path,
			std::string
		);

};

}

#endif  // INPUTXSLT_SRC_FUNCTION_WRITE_FILE_H_
