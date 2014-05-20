#ifndef INPUTXSLT_SRC_PLATTFORM_GUARD_H_
#define INPUTXSLT_SRC_PLATTFORM_GUARD_H_

#include <string>
#include <vector>

#include "support/include_entity_resolver.h"

namespace InputXSLT {

class PlattformGuard {
	public:
		PlattformGuard(const std::vector<std::string>&);
		~PlattformGuard();

		IncludeEntityResolver* getEntityResolver();

	private:
		IncludeEntityResolver include_resolver_;

};

}

#endif  // INPUTXSLT_SRC_PLATTFORM_GUARD_H_
