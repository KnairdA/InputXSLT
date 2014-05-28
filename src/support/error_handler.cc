#include "error_handler.h"

#include <xercesc/sax/SAXParseException.hpp>

#include <iostream>

#include "support/xerces_string_guard.h"

namespace InputXSLT {

ErrorHandler::ErrorHandler(const std::string& transformation):
	transformation_path_(transformation) { }

void ErrorHandler::warning(const xercesc::SAXParseException& e) {
	std::cerr << "Warning in "
	          << "'"
	          << this->transformation_path_
	          << "': "
	          << *XercesStringGuard<char>(e.getMessage())
	          << std::endl;
}

void ErrorHandler::error(const xercesc::SAXParseException& e) {
	std::cerr << "Error in "
	          << "'"
	          << this->transformation_path_
	          << "': "
	          << *XercesStringGuard<char>(e.getMessage())
	          << std::endl;
}

void ErrorHandler::fatalError(const xercesc::SAXParseException& e) {
	std::cerr << "Fatal error in "
	          << "'"
	          << this->transformation_path_
	          << "': "
	          << *XercesStringGuard<char>(e.getMessage())
	          << std::endl;
}

void ErrorHandler::resetErrors() { }

}
