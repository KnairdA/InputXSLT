#include "result_node_facade.h"

#include <xercesc/dom/DOMText.hpp>
#include <xercesc/dom/DOMNodeList.hpp>

#include "support/xerces_string_guard.h"

namespace InputXSLT {

ResultNodeFacade::ResultNodeFacade(
	xercesc::DOMDocument* document,
	xercesc::DOMNode* node,
	const std::string& name
):
	dom_document_(document),
	result_node_(
		dom_document_->createElement(*XercesStringGuard<XMLCh>(name))
	),
	root_node_(node) { }

ResultNodeFacade::~ResultNodeFacade() {
	this->root_node_->appendChild(this->result_node_);
}

xercesc::DOMElement* ResultNodeFacade::getResultElement() {
	return this->result_node_;
}

void ResultNodeFacade::setAttribute(
	const std::string& name,
	const std::string& value
) {
	this->result_node_->setAttribute(
		*XercesStringGuard<XMLCh>(name),
		*XercesStringGuard<XMLCh>(value)
	);
}

void ResultNodeFacade::setValueNode(
	const std::string& name,
	const std::string& value
) {
	xercesc::DOMElement* const nameNode(
		this->dom_document_->createElement(*XercesStringGuard<XMLCh>(name))
	);

	xercesc::DOMText* const valueNode(
		this->dom_document_->createTextNode(*XercesStringGuard<XMLCh>(value))
	);

	nameNode->appendChild(valueNode);

	this->result_node_->appendChild(nameNode);
}

void ResultNodeFacade::setContent(const std::string& content) {
	this->result_node_->appendChild(
		this->dom_document_->createTextNode(*XercesStringGuard<XMLCh>(content))
	);
}

void ResultNodeFacade::setContent(xercesc::DOMNode* node) {
	this->result_node_->appendChild(node);
}

void ResultNodeFacade::setContent(xercesc::DOMNodeList* nodes) {
	for ( std::size_t index = 0; index != nodes->getLength(); ++index ) {
		this->setContent(nodes->item(index));
	}
}

}
