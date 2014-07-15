#ifndef INPUTXSLT_SRC_PLATFORM_GUARD_H_
#define INPUTXSLT_SRC_PLATFORM_GUARD_H_

#include <string>
#include <vector>

#include "support/include_entity_resolver.h"

namespace InputXSLT {

class PlatformGuard {
	public:
		PlatformGuard(const std::vector<std::string>&);
		~PlatformGuard();

		IncludeEntityResolver* getEntityResolver();

	private:
		IncludeEntityResolver include_resolver_;

};

}

#endif  // INPUTXSLT_SRC_PLATFORM_GUARD_H_
