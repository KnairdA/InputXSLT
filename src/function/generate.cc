#include "generate.h"

#include <xalanc/PlatformSupport/XalanOutputStreamPrintWriter.hpp>
#include <xalanc/PlatformSupport/XalanStdOutputStream.hpp>
#include <xalanc/XercesParserLiaison/FormatterToXercesDOM.hpp>
#include <xalanc/XMLSupport/FormatterToXML.hpp>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include "transformer_facade.h"
#include "support/xerces_string_guard.h"
#include "support/dom/result_node_facade.h"
#include "support/error/error_capacitor.h"

namespace InputXSLT {

DomDocumentCache::document_ptr FunctionGenerate::constructDocument(
	const FilesystemContext&,
	xalan::XSLTInputSource                   inputSource,
	xalan::XSLTInputSource                   transformationSource,
	boost::optional<boost::filesystem::path> targetPath
) const {
	DomDocumentCache::document_ptr domDocument(
		DomDocumentCache::createDocument("content")
	);

	ResultNodeFacade  result(domDocument.get(), "generation");
	TransformerFacade transformer(this->include_resolver_);

	try {
		if ( targetPath ) {
			result.setAttribute("path", (*targetPath).string());

			boost::filesystem::create_directories(
				boost::filesystem::absolute(*targetPath).parent_path()
			);

			boost::filesystem::ofstream file(*targetPath);

			if ( file.is_open() ) {
				xalan::XalanStdOutputStream         output(file);
				xalan::XalanOutputStreamPrintWriter writer(output);
				xalan::FormatterToXML               targetFormatter(writer);

				transformer.generate(
					inputSource,
					transformationSource,
					targetFormatter
				);
			} else {
				result.setAttribute("result", "error");
			}
		} else {
			xalan::FormatterToXercesDOM targetFormatter(
				domDocument.get(),
				result.getResultElement()
			);

			transformer.generate(
				inputSource,
				transformationSource,
				targetFormatter
			);
		}

		result.setAttribute("result", "success");
	}
	catch (const ErrorCapacitor::exception& exception) {
		result.setAttribute("result", "error");

		for ( auto&& error : *exception ) {
			result.setValueNode("error", error);
		}
	}

	WarningCapacitor::warning_cache_ptr warnings(
		transformer.getCachedWarnings()
	);

	for ( auto&& warning : *warnings ) {
		result.setValueNode("warning", warning);
	}

	return domDocument;
}

}
