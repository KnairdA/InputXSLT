#include "support/dom_document_guard.h"

#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/util/XMLString.hpp>

namespace InputXSLT {

DomDocumentGuard::DomDocumentGuard(const std::string& rootNode):
	parser_(),
	dom_support_(parser_),
	root_node_name_(
		xercesc::XMLString::transcode(rootNode.data())
	),
	document_(
		xercesc::DOMImplementation::getImplementation()->createDocument(
			nullptr,
			root_node_name_,
			nullptr
		)
	),
	parsed_source_() { }

DomDocumentGuard::~DomDocumentGuard() {
	xercesc::XMLString::release(&this->root_node_name_);

	this->document_->release();
}

xercesc::DOMDocument* DomDocumentGuard::operator->() {
	return this->document_;
}

xalan::XalanDocument* DomDocumentGuard::finalize() {
	this->parsed_source_.reset(
		new xalan::XercesDOMWrapperParsedSource(
				document_,
				parser_,
				dom_support_
		)
	);

	return this->parsed_source_->getDocument();
}

}
