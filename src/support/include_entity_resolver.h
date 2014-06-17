#ifndef INPUTXSLT_SRC_SUPPORT_INCLUDE_ENTITY_RESOLVER_H_
#define INPUTXSLT_SRC_SUPPORT_INCLUDE_ENTITY_RESOLVER_H_

#include <xercesc/sax/EntityResolver.hpp>
#include <xercesc/sax/InputSource.hpp>

#include "boost/optional.hpp"

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

		boost::filesystem::path resolve(
			const XMLCh* const) const;
		boost::optional<boost::filesystem::path> resolve(
			const boost::filesystem::path&) const;

	private:
		const std::vector<FilesystemContext> path_;

};

}

#endif  // INPUTXSLT_SRC_SUPPORT_INCLUDE_ENTITY_RESOLVER_H_
