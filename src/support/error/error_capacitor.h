#ifndef INPUTXSLT_SRC_SUPPORT_ERROR_ERROR_CAPACITOR_H_
#define INPUTXSLT_SRC_SUPPORT_ERROR_ERROR_CAPACITOR_H_

#include <memory>

#include "error_multiplexer.h"

namespace InputXSLT {

class ErrorCapacitor : public ErrorMultiplexer::receiver {
	public:
		class exception;

		typedef std::vector<std::string> error_cache;

		ErrorCapacitor(ErrorMultiplexer*);

		void discharge();

		virtual void receive(
			const ErrorMultiplexer::error_type,
			const std::string&
		);

	private:
		std::unique_ptr<error_cache> error_cache_;

};

class ErrorCapacitor::exception {
	public:
		exception(std::unique_ptr<error_cache>);

		const error_cache& operator*() const;

	private:
		std::unique_ptr<error_cache> error_cache_;

};

}

#endif  // INPUTXSLT_SRC_SUPPORT_ERROR_ERROR_CAPACITOR_H_
