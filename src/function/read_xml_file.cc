#include "read_xml_file.h"

#include "boost/filesystem/fstream.hpp"

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

	xalan::CharVectorType castHelper;
	arguments[0]->str().transcode(castHelper);

	const std::string fileName(
		castHelper.begin(),
		castHelper.end()
	);

	boost::filesystem::ifstream file(this->path_ / fileName);

	return executionContext.getXObjectFactory().createNodeSet(
		this->parser_.parseXMLStream(
			xalan::XSLTInputSource(file)
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
