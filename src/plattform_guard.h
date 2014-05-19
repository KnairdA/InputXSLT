#ifndef INPUTXSLT_SRC_PLATTFORM_GUARD_H_
#define INPUTXSLT_SRC_PLATTFORM_GUARD_H_

#include <string>
#include <vector>

namespace InputXSLT {

struct PlattformGuard {
	PlattformGuard(const std::vector<std::string>&);
	~PlattformGuard();
};

}

#endif  // INPUTXSLT_SRC_PLATTFORM_GUARD_H_
