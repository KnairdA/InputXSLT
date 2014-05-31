#ifndef INPUTXSLT_SRC_SUPPORT_RESULT_NODE_FACADE_H_
#define INPUTXSLT_SRC_SUPPORT_RESULT_NODE_FACADE_H_

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMElement.hpp>

#include <string>

namespace InputXSLT {

class ResultNodeFacade {
	public:
		ResultNodeFacade(
			xercesc::DOMDocument*,
			xercesc::DOMNode*,
			const std::string&
		);
		~ResultNodeFacade();

		xercesc::DOMNode* getNode();

		void setAttribute(const std::string&, const std::string&);
		void setValueNode(const std::string&, const std::string&);

		void setContent(const std::string&);
		void setContent(xercesc::DOMNode*);
		void setContent(xercesc::DOMNodeList*);

	private:
		xercesc::DOMDocument* const dom_document_;
		xercesc::DOMElement*  const result_node_;
		xercesc::DOMNode*     const root_node_;

};

}

#endif  // INPUTXSLT_SRC_SUPPORT_RESULT_NODE_FACADE_H_
