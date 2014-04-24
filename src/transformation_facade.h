#ifndef INPUTXSLT_SRC_TRANSFORMATION_FACADE_H_
#define INPUTXSLT_SRC_TRANSFORMATION_FACADE_H_

#include <string>

#include <xalanc/XalanTransformer/XalanTransformer.hpp>
#include <xalanc/XercesParserLiaison/XercesParserLiaison.hpp>

#include "common.h"
#include "support/filesystem_context.h"

namespace InputXSLT {

class TransformationFacade {
	public:
		TransformationFacade(const std::string&);
		~TransformationFacade();

		int generate(const std::string&);

	private:
		const FilesystemContext fs_context_;
		const xalan::XalanCompiledStylesheet* transformation_;

		xalan::XalanTransformer transformer_;

};

}

#endif  // INPUTXSLT_SRC_TRANSFORMATION_FACADE_H_
