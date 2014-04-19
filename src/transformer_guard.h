#ifndef INPUTXSLT_SRC_TRANSFORMER_GUARD_H_
#define INPUTXSLT_SRC_TRANSFORMER_GUARD_H_

#include <string>

#include <xalanc/XalanTransformer/XalanTransformer.hpp>
#include <xalanc/XercesParserLiaison/XercesParserLiaison.hpp>

#include "plattform_guard.h"

namespace InputXSLT {

class TransformerGuard {
	public:
		TransformerGuard();

		int execute(const std::string&, const std::string&);

	private:
		const PlattformGuard plattform_;
		mutable xalan::XercesParserLiaison parser_;

		xalan::XalanTransformer transformer_;

};

}

#endif  // INPUTXSLT_SRC_TRANSFORMER_GUARD_H_
