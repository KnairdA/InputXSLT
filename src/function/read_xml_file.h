#ifndef INPUTXSLT_SRC_FUNCTION_READ_XML_FILE_H_
#define INPUTXSLT_SRC_FUNCTION_READ_XML_FILE_H_

#include "base.h"

namespace InputXSLT {

class FunctionReadXmlFile : public FunctionBase<FunctionReadXmlFile> {
	public:
		using FunctionBase<FunctionReadXmlFile>::FunctionBase;

		xercesc::DOMDocument* constructDocument(
			const FilesystemContext&,
			const boost::filesystem::path&
		);

};

}

#endif  // INPUTXSLT_SRC_FUNCTION_READ_XML_FILE_H_
