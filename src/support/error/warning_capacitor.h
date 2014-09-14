#ifndef INPUTXSLT_SRC_SUPPORT_ERROR_WARNING_CAPACITOR_H_
#define INPUTXSLT_SRC_SUPPORT_ERROR_WARNING_CAPACITOR_H_

#include <memory>

#include "error_multiplexer.h"

namespace InputXSLT {

class WarningCapacitor : public ErrorMultiplexer::receiver {
	public:
		typedef std::vector<std::string>       warning_cache;
		typedef std::unique_ptr<warning_cache> warning_cache_ptr;

		WarningCapacitor(ErrorMultiplexer*);

		warning_cache_ptr discharge();

		virtual void receive(
			const ErrorMultiplexer::error_type,
			const std::string&
		);

	private:
		warning_cache_ptr warning_cache_;

};

}

#endif  // INPUTXSLT_SRC_SUPPORT_ERROR_WARNING_CAPACITOR_H_
