#include "read_xml_file.h"

namespace InputXSLT {

FunctionReadXmlFile::FunctionReadXmlFile(const std::string& path):
	path_(path),
	parser_() { }

FunctionReadXmlFile::FunctionReadXmlFile(const FunctionReadXmlFile& src):
	path_(src.path_),
	parser_() { }

xalan::XObjectPtr FunctionReadXmlFile::execute(
	xalan::XPathExecutionContext&                executionContext,
	xalan::XalanNode*                            context,
	const xalan::Function::XObjectArgVectorType& arguments,
	const xalan::Locator*                        locator
) const {
	if ( arguments.size() != 1 ) {
		xalan::XPathExecutionContext::GetAndReleaseCachedString guard(
			executionContext
		);

		generalError(executionContext, context, locator);
	}

	xalan::XalanDOMString fileName(this->path_.data());
	fileName.append(arguments[0]->str());

	return executionContext.getXObjectFactory().createNodeSet(
		this->parser_.parseXMLStream(
			xalan::XSLTInputSource(fileName)
		)
	);
}

FunctionReadXmlFile* FunctionReadXmlFile::clone(
	xalan::MemoryManager& manager) const {
	return xalan::XalanCopyConstruct(manager, *this);
}

const xalan::XalanDOMString& FunctionReadXmlFile::getError(
	xalan::XalanDOMString& result) const {
	result.assign("The read-xml-file() function expects one argument.");

	return result;
}

}
