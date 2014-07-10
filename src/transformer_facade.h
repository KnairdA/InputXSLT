#ifndef INPUTXSLT_SRC_TRANSFORMER_FACADE_H_
#define INPUTXSLT_SRC_TRANSFORMER_FACADE_H_

#include <xalanc/XalanTransformer/XalanTransformer.hpp>

#include "common.h"
#include "support/include_entity_resolver.h"
#include "support/error/error_multiplexer.h"
#include "support/error/error_capacitor.h"
#include "support/error/warning_capacitor.h"

namespace InputXSLT {

class TransformerFacade {
	public:
		TransformerFacade(IncludeEntityResolver*);

		void generate(
			const xalan::XSLTInputSource&,
			const xalan::XSLTInputSource&,
			xalan::FormatterListener&
		);

		void generate(
			const xalan::XSLTInputSource&,
			xalan::FormatterListener&
		);

		WarningCapacitor::warning_cache_ptr getCachedWarnings();

	private:
		xalan::XalanTransformer transformer_;
		ErrorMultiplexer error_multiplexer_;
		WarningCapacitor warning_capacitor_;

		void generate(
			xalan::XalanNode* const,
			const xalan::XSLTInputSource&,
			xalan::FormatterListener&
		);

		template <typename Source>
		void dispatchTransformer(
			const Source&,
			const xalan::XSLTInputSource&,
			xalan::FormatterListener&
		);


};

}

#endif  // INPUTXSLT_SRC_TRANSFORMER_FACADE_H_
