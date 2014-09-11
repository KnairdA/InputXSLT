#include "transformer_facade.h"

#include <xalanc/PlatformSupport/XalanOutputStreamPrintWriter.hpp>
#include <xalanc/PlatformSupport/XalanStdOutputStream.hpp>

#include <xalanc/XSLT/StylesheetRoot.hpp>
#include <xalanc/XalanTransformer/XalanCompiledStylesheet.hpp>
#include <xalanc/XMLSupport/FormatterToXML.hpp>
#include <xalanc/XercesParserLiaison/FormatterToXercesDOM.hpp>
#include <xalanc/XMLSupport/FormatterTreeWalker.hpp>

#include <xalanc/XercesParserLiaison/XercesParserLiaison.hpp>
#include <xalanc/XercesParserLiaison/XercesDOMSupport.hpp>
#include <xalanc/XalanTransformer/XercesDOMWrapperParsedSource.hpp>

#include "support/xerces_string_guard.h"
#include "support/dom/document_cache.h"

namespace {

std::unique_ptr<xalan::FormatterToXML> augmentFormatterToXML(
	const xalan::XalanCompiledStylesheet* const transformation,
	xalan::FormatterListener&                   formatter
) {
	const xalan::StylesheetRoot* const stylesheetRoot(
		transformation->getStylesheetRoot()
	);

	xalan::XalanDOMString outputVersion;
	xalan::XalanDOMString outputEncoding;
	xalan::XalanDOMString outputMediaType;
	xalan::XalanDOMString outputDoctypePublic;
	xalan::XalanDOMString outputDoctypeSystem;
	xalan::XalanDOMString outputStandalone;

	return std::unique_ptr<xalan::FormatterToXML>(
		new xalan::FormatterToXML(
			*(formatter.getWriter()),
			stylesheetRoot->getOutputVersion(outputVersion),
			stylesheetRoot->getOutputIndent(),
			xalan::FormatterToXML::eDefaultIndentAmount,
			stylesheetRoot->getOutputEncoding(outputEncoding),
			stylesheetRoot->getOutputMediaType(outputMediaType),
			stylesheetRoot->getOutputDoctypeSystem(outputDoctypeSystem),
			stylesheetRoot->getOutputDoctypePublic(outputDoctypePublic),
			!stylesheetRoot->getOmitOutputXMLDecl(),
			stylesheetRoot->getOutputStandalone(outputStandalone)
		)
	);
}

class compiled_stylesheet_deleter {
	public:
		compiled_stylesheet_deleter(xalan::XalanTransformer* transformer):
			transformer_(transformer) { }

		void operator()(const xalan::XalanCompiledStylesheet* transformation) {
			this->transformer_->destroyStylesheet(transformation);
		}

	private:
		xalan::XalanTransformer* const transformer_;

};

typedef std::unique_ptr<
	const xalan::XalanCompiledStylesheet,
	compiled_stylesheet_deleter
> CompiledStylesheetPtr;

CompiledStylesheetPtr compileStylesheet(
	xalan::XalanTransformer* const transformer,
	const xalan::XSLTInputSource&  transformation
) {
	const xalan::XalanCompiledStylesheet* compiled{};

	if ( transformer->compileStylesheet(transformation, compiled) == 0 ) {
		return CompiledStylesheetPtr(
			compiled,
			compiled_stylesheet_deleter(transformer)
		);
	} else {
		return CompiledStylesheetPtr(
			nullptr,
			compiled_stylesheet_deleter(transformer)
		);
	}
}

}

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

		this->dispatchTransformer(
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

	this->dispatchTransformer(
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

	this->dispatchTransformer(
		inputParsedSource,
		transformation,
		target
	);

	errorCapacitor.discharge();
}

template <typename Source>
void TransformerFacade::dispatchTransformer(
	const Source&                 source,
	const xalan::XSLTInputSource& transformation,
	xalan::FormatterListener&     target
) {
	if ( auto compiledTransformation = compileStylesheet(
		&(this->transformer_),
		transformation
	) ) {
		switch ( target.getOutputFormat() ) {
			case xalan::FormatterListener::eFormat::OUTPUT_METHOD_XML: {
				auto formatter = augmentFormatterToXML(
					compiledTransformation.get(),
					target
				);

				this->transformer_.transform(
					source,
					compiledTransformation.get(),
					*formatter
				);

				break;
			}
			default: {
				this->transformer_.transform(
					source,
					compiledTransformation.get(),
					target
				);

				break;
			}
		}
	}
}

}
