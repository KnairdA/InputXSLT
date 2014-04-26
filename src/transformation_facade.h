#ifndef INPUTXSLT_SRC_TRANSFORMATION_FACADE_H_
#define INPUTXSLT_SRC_TRANSFORMATION_FACADE_H_

#include <xalanc/XalanTransformer/XalanTransformer.hpp>

#include <string>

#include "common.h"
#include "support/filesystem_context.h"
#include "support/dom/document_cache.h"

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
		DomDocumentCache document_cache_;

};

}

#endif  // INPUTXSLT_SRC_TRANSFORMATION_FACADE_H_
