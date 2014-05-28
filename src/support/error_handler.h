#ifndef INPUTXSLT_SRC_SUPPORT_ERROR_HANDLER_H_
#define INPUTXSLT_SRC_SUPPORT_ERROR_HANDLER_H_

#include <xercesc/sax/ErrorHandler.hpp>

#include <string>

namespace InputXSLT {

class ErrorHandler : public xercesc::ErrorHandler {
	public:
		ErrorHandler(const std::string&);

		virtual void warning(const xercesc::SAXParseException&);
		virtual void error(const xercesc::SAXParseException&);
		virtual void fatalError(const xercesc::SAXParseException&);
		virtual void resetErrors();

	private:
		const std::string& transformation_path_;

};

}

#endif  // INPUTXSLT_SRC_SUPPORT_ERROR_HANDLER_H_
