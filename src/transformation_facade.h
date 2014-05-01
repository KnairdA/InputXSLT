#ifndef INPUTXSLT_SRC_TRANSFORMATION_FACADE_H_
#define INPUTXSLT_SRC_TRANSFORMATION_FACADE_H_

#include <xalanc/XalanTransformer/XalanTransformer.hpp>

#include <string>
#include <unordered_map>

#include "common.h"
#include "support/filesystem_context.h"

namespace InputXSLT {

class TransformationFacade {
	public:
		typedef std::unordered_map<std::string, std::string> parameter_map;

		TransformationFacade(const std::string&);
		~TransformationFacade();

		int generate(const std::string&);
		int generate(const std::string&, const parameter_map&);

	private:
		const FilesystemContext fs_context_;
		const xalan::XalanCompiledStylesheet* transformation_;

		xalan::XalanTransformer transformer_;

};

}

#endif  // INPUTXSLT_SRC_TRANSFORMATION_FACADE_H_
