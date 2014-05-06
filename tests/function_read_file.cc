#include "gtest/gtest.h"

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMText.hpp>

#include <string>

#include "function/base.h"
#include "function/read_file.h"

typedef std::basic_string<XMLCh> XmlString;

class FunctionReadFileTest : public ::testing::Test {
	friend InputXSLT::FunctionReadFile;
};

TEST_F(FunctionReadFileTest, constructDocumentTest) {
	const InputXSLT::FilesystemContext fsContext("../tests");
	const boost::filesystem::path filePath(
		fsContext.resolve("./function_read_file.cc")
	);

	InputXSLT::FunctionReadFile function;

	xercesc::DOMDocument* const domDocument = function.constructDocument(
		fsContext,
		filePath
	);

	EXPECT_NE(domDocument, nullptr);

	EXPECT_EQ(
		XmlString(domDocument->getDocumentElement()->getNodeName()),
		XmlString(reinterpret_cast<const XMLCh*>(u"content"))
	);

	EXPECT_EQ(
		XmlString(domDocument->getDocumentElement()->getFirstChild()->getNodeName()),
		XmlString(reinterpret_cast<const XMLCh*>(u"result"))
	);
}
