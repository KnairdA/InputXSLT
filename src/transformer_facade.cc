#include "transformer_facade.h"

#include <xalanc/PlatformSupport/XalanOutputStreamPrintWriter.hpp>
#include <xalanc/PlatformSupport/XalanStdOutputStream.hpp>

#include <xalanc/XMLSupport/FormatterToXML.hpp>
#include <xalanc/XercesParserLiaison/FormatterToXercesDOM.hpp>
#include <xalanc/XMLSupport/FormatterTreeWalker.hpp>

#include <xalanc/XercesParserLiaison/XercesParserLiaison.hpp>
#include <xalanc/XercesParserLiaison/XercesDOMSupport.hpp>
#include <xalanc/XalanTransformer/XercesDOMWrapperParsedSource.hpp>

#include "support/xerces_string_guard.h"
#include "support/dom/document_cache.h"

namespace InputXSLT {

TransformerFacade::TransformerFacade(IncludeEntityResolver* resolver):
	transformer_(),
	error_multiplexer_(&transformer_),
	warning_capacitor_(&error_multiplexer_) {
	this->transformer_.setEntityResolver(resolver);
}

WarningCapacitor::warning_cache_ptr TransformerFacade::getCachedWarnings() {
	return this->warning_capacitor_.discharge();
}

void TransformerFacade::generate(
	const xalan::XSLTInputSource& source,
	const xalan::XSLTInputSource& transformation,
	xalan::FormatterListener&     target
) {
	if ( source.getNode() == nullptr ) {
		ErrorCapacitor errorCapacitor(&this->error_multiplexer_);

		this->transformer_.transform(
			source,
			transformation,
			target
		);

		errorCapacitor.discharge();
	} else {
		this->generate(
			source.getNode(),
			transformation,
			target
		);
	}
}

void TransformerFacade::generate(
	const xalan::XSLTInputSource& transformation,
	xalan::FormatterListener&     target
) {
	ErrorCapacitor errorCapacitor(&this->error_multiplexer_);

	DomDocumentCache::document_ptr inputDocument(
		DomDocumentCache::createDocument()
	);

	xalan::XercesParserLiaison parserLiaison;
	xalan::XercesDOMSupport domSupport(parserLiaison);

	xalan::XercesDOMWrapperParsedSource inputParsedSource(
		inputDocument.get(),
		parserLiaison,
		domSupport
	);

	this->transformer_.transform(
		inputParsedSource,
		transformation,
		target
	);

	errorCapacitor.discharge();
}

void TransformerFacade::generate(
	xalan::XalanNode* const       source,
	const xalan::XSLTInputSource& transformation,
	xalan::FormatterListener&     target
) {
	ErrorCapacitor errorCapacitor(&this->error_multiplexer_);

	DomDocumentCache::document_ptr inputDocument(
		DomDocumentCache::createDocument()
	);

	xalan::FormatterToXercesDOM inputFormatter(
		inputDocument.get(),
		inputDocument->getDocumentElement()
	);

	xalan::FormatterTreeWalker walker(inputFormatter);
	walker.traverseSubtree(source);

	xalan::XercesParserLiaison parserLiaison;
	xalan::XercesDOMSupport domSupport(parserLiaison);

	xalan::XercesDOMWrapperParsedSource inputParsedSource(
		inputFormatter.getDocument(),
		parserLiaison,
		domSupport
	);

	this->transformer_.transform(
		inputParsedSource,
		transformation,
		target
	);

	errorCapacitor.discharge();
}

}
