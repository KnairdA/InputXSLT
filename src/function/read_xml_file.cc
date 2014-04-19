#include "read_xml_file.h"

namespace InputXSLT {

FunctionReadXmlFile::FunctionReadXmlFile():
	parser_() { }

FunctionReadXmlFile::FunctionReadXmlFile(const FunctionReadXmlFile&):
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

	return executionContext.getXObjectFactory().createNodeSet(
		this->parser_.parseXMLStream(
			xalan::XSLTInputSource(arguments[0]->str())
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
