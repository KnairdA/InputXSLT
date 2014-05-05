#ifndef INPUTXSLT_SRC_FUNCTION_READ_DIRECTORY_H_
#define INPUTXSLT_SRC_FUNCTION_READ_DIRECTORY_H_

#include "base.h"

namespace InputXSLT {

class FunctionReadDirectory : public FunctionBase<FunctionReadDirectory> {
	public:
		using FunctionBase<FunctionReadDirectory>::FunctionBase;

	protected:
		friend FunctionBase<FunctionReadDirectory>;

		xercesc::DOMDocument* constructDocument(
			const FilesystemContext&,
			const boost::filesystem::path&
		);

};

}

#endif  // INPUTXSLT_SRC_FUNCTION_READ_DIRECTORY_H_
