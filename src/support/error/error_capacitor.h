#ifndef INPUTXSLT_SRC_SUPPORT_ERROR_ERROR_CAPACITOR_H_
#define INPUTXSLT_SRC_SUPPORT_ERROR_ERROR_CAPACITOR_H_

#include <memory>

#include "error_multiplexer.h"

namespace InputXSLT {

class ErrorCapacitor : public ErrorMultiplexer::receiver {
	public:
		class exception;

		typedef std::vector<std::string> error_cache;
		typedef std::unique_ptr<error_cache> error_cache_ptr;

		ErrorCapacitor(ErrorMultiplexer*);

		void discharge();

		virtual void receive(
			const ErrorMultiplexer::error_type,
			const std::string&
		);

	private:
		error_cache_ptr error_cache_;

};

class ErrorCapacitor::exception {
	public:
		exception(error_cache_ptr);

		const error_cache& operator*() const;

	private:
		error_cache_ptr error_cache_;

};

}

#endif  // INPUTXSLT_SRC_SUPPORT_ERROR_ERROR_CAPACITOR_H_
