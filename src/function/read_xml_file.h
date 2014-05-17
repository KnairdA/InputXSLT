#ifndef INPUTXSLT_SRC_FUNCTION_READ_XML_FILE_H_
#define INPUTXSLT_SRC_FUNCTION_READ_XML_FILE_H_

#include "base.h"

namespace InputXSLT {

class FunctionReadXmlFile : public FunctionBase<
	FunctionReadXmlFile,
	std::string
> {
	public:
		using FunctionBase::FunctionBase;

	protected:
		friend FunctionBase;

		xercesc::DOMDocument* constructDocument(
			const FilesystemContext&,
			const FunctionBase::parameter_tuple&
		);

};

}

#endif  // INPUTXSLT_SRC_FUNCTION_READ_XML_FILE_H_
