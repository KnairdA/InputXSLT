#ifndef INPUTXSLT_SRC_TRANSFORMER_FACADE_H_
#define INPUTXSLT_SRC_TRANSFORMER_FACADE_H_

#include <string>

#include <xalanc/XalanTransformer/XalanTransformer.hpp>
#include <xalanc/XercesParserLiaison/XercesParserLiaison.hpp>

#include "common.h"
#include "support/filesystem_context.h"

namespace InputXSLT {

class TransformerFacade {
	public:
		TransformerFacade(const std::string&);

		int execute(const std::string&, const std::string&);

	private:
		const FilesystemContext fs_context_;
		mutable xalan::XercesParserLiaison parser_;

		xalan::XalanTransformer transformer_;

};

}

#endif  // INPUTXSLT_SRC_TRANSFORMER_FACADE_H_
