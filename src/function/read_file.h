#ifndef INPUTXSLT_SRC_FUNCTION_READ_FILE_H_
#define INPUTXSLT_SRC_FUNCTION_READ_FILE_H_

#include "base.h"

namespace InputXSLT {

class FunctionReadFile : public FunctionBase<FunctionReadFile> {
	public:
		using FunctionBase<FunctionReadFile>::FunctionBase;

	protected:
		friend FunctionBase<FunctionReadFile>;

		xercesc::DOMDocument* constructDocument(
			const FilesystemContext&,
			const boost::filesystem::path&
		);

};

}

#endif  // INPUTXSLT_SRC_FUNCTION_READ_FILE_H_
