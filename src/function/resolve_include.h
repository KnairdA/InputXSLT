#ifndef INPUTXSLT_SRC_FUNCTION_RESOLVE_INCLUDE_H_
#define INPUTXSLT_SRC_FUNCTION_RESOLVE_INCLUDE_H_

#include <xalanc/XPath/XObjectFactory.hpp>
#include <xalanc/XPath/Function.hpp>
#include <xalanc/XPath/XObject.hpp>

#include "boost/filesystem.hpp"

#include <memory>
#include <string>
#include <vector>

#include "common.h"
#include "support/filesystem_context.h"

namespace InputXSLT {

class FunctionResolveInclude : public xalan::Function {
	public:
		FunctionResolveInclude(const std::vector<std::string>&);

		virtual xalan::XObjectPtr execute(
			xalan::XPathExecutionContext&,
			xalan::XalanNode*,
			const xalan::XObjectPtr,
			const xalan::Locator*
		) const;

		virtual FunctionResolveInclude* clone(xalan::MemoryManager&) const;

		FunctionResolveInclude& operator=(const FunctionResolveInclude&) = delete;
		bool operator==(const FunctionResolveInclude&) const             = delete;

	private:
		const std::shared_ptr<std::vector<FilesystemContext>> path_;

		const xalan::XalanDOMString& getError(xalan::XalanDOMString&) const;

};

}

#endif  // INPUTXSLT_SRC_FUNCTION_RESOLVE_INCLUDE_H_
