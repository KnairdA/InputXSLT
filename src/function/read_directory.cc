#include "read_directory.h"

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMText.hpp>

#include "support/xerces_string_guard.h"

namespace InputXSLT {

xercesc::DOMDocument* FunctionReadDirectory::constructDocument(
	const InputXSLT::FilesystemContext& fsContext,
	const FunctionBase::parameter_tuple& parameters
) {
	const boost::filesystem::path directoryPath(
		fsContext.resolve(std::get<0>(parameters))
	);

	xercesc::DOMDocument* const domDocument(
		xercesc::DOMImplementation::getImplementation()->createDocument(
			nullptr,
			*XercesStringGuard<XMLCh>("content"),
			nullptr
		)
	);

	xercesc::DOMNode* const rootNode(
		domDocument->getDocumentElement()
	);

	if ( boost::filesystem::is_directory(directoryPath) ) {
		fsContext.iterate(
			directoryPath,
			[&domDocument, &rootNode](const boost::filesystem::path& p) {
			xercesc::DOMElement* const itemNode(
				domDocument->createElement(*XercesStringGuard<XMLCh>("result"))
			);

			switch ( boost::filesystem::status(p).type() ) {
				case boost::filesystem::regular_file: {
					itemNode->setAttribute(
						*XercesStringGuard<XMLCh>("type"),
						*XercesStringGuard<XMLCh>("file")
					);

					break;
				};
				case boost::filesystem::directory_file: {
					itemNode->setAttribute(
						*XercesStringGuard<XMLCh>("type"),
						*XercesStringGuard<XMLCh>("directory")
					);

					break;
				};
				default: {
					itemNode->setAttribute(
						*XercesStringGuard<XMLCh>("type"),
						*XercesStringGuard<XMLCh>("misc")
					);

					break;
				};
			}

			xercesc::DOMText* const textNode(
				domDocument->createTextNode(
					*XercesStringGuard<XMLCh>(p.filename().string())
				)
			);

			itemNode->appendChild(textNode);
			rootNode->appendChild(itemNode);
		});
	} else {
		xercesc::DOMElement* const resultNode(
			domDocument->createElement(*XercesStringGuard<XMLCh>("error"))
		);

		rootNode->appendChild(resultNode);
	}

	return domDocument;
}

}
