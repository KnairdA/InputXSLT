#ifndef INPUTXSLT_SRC_SUPPORT_ERROR_ERROR_CAPACITOR_H_
#define INPUTXSLT_SRC_SUPPORT_ERROR_ERROR_CAPACITOR_H_

#include <memory>

#include "error_multiplexer.h"

namespace InputXSLT {

class ErrorCapacitor : public ErrorMultiplexer::Receiver {
	public:
		class exception;

		typedef std::vector<std::string> error_cache;
		typedef std::unique_ptr<error_cache> error_cache_ptr;

		ErrorCapacitor(ErrorMultiplexer*);
		~ErrorCapacitor();

		void discharge();

		virtual void receive(
			const ErrorMultiplexer::ErrorType,
			const std::string&
		);

	private:
		ErrorMultiplexer* const multiplexer_;
		error_cache_ptr error_cache_;

};

class ErrorCapacitor::exception {
	public:
		exception(error_cache_ptr);

		const error_cache* getCachedErrors() const;

	private:
		error_cache_ptr error_cache_;

};

}

#endif  // INPUTXSLT_SRC_SUPPORT_ERROR_ERROR_CAPACITOR_H_
