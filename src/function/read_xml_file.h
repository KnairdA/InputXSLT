#ifndef INPUTXSLT_SRC_FUNCTION_READ_XML_FILE_H_
#define INPUTXSLT_SRC_FUNCTION_READ_XML_FILE_H_

#include "base.h"

namespace InputXSLT {

class FunctionReadXmlFile : public FunctionBase<FunctionReadXmlFile, 1> {
	public:
		using FunctionBase::FunctionBase;

	protected:
		friend FunctionBase;

		xercesc::DOMDocument* constructDocument(
			const FilesystemContext&,
			const FunctionBase::argument_array&
		);

};

}

#endif  // INPUTXSLT_SRC_FUNCTION_READ_XML_FILE_H_
