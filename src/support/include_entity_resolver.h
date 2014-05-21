#ifndef INPUTXSLT_SRC_SUPPORT_INCLUDE_ENTITY_RESOLVER_H_
#define INPUTXSLT_SRC_SUPPORT_INCLUDE_ENTITY_RESOLVER_H_

#include <xercesc/sax/EntityResolver.hpp>
#include <xercesc/sax/InputSource.hpp>

#include <string>
#include <vector>

#include "filesystem_context.h"

namespace InputXSLT {

class IncludeEntityResolver : public xercesc::EntityResolver {
	public:
		IncludeEntityResolver(const std::vector<std::string>&);

		xercesc::InputSource* resolveEntity(
			const XMLCh* const,
			const XMLCh* const
		);

		std::pair<bool, boost::filesystem::path> resolve(const std::string&);

	private:
		std::vector<FilesystemContext> path_;

};

}

#endif  // INPUTXSLT_SRC_SUPPORT_INCLUDE_ENTITY_RESOLVER_H_
