#ifndef INPUTXSLT_SRC_FUNCTION_EXTERNAL_COMMAND_H_
#define INPUTXSLT_SRC_FUNCTION_EXTERNAL_COMMAND_H_

#include <boost/filesystem.hpp>
#include <boost/optional.hpp>

#include "base.h"

namespace InputXSLT {

class FunctionExternalCommand : public FunctionBase<
	FunctionExternalCommand,
	std::string,
	boost::optional<std::string>
> {
	public:
		using FunctionBase::FunctionBase;

	protected:
		friend FunctionBase;

		DomDocumentCache::document_ptr constructDocument(
			const FilesystemContext&,
			std::string,
			boost::optional<std::string>
		) const;

};

}

#endif  // INPUTXSLT_SRC_FUNCTION_EXTERNAL_COMMAND_H_
