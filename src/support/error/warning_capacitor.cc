#include "warning_capacitor.h"

namespace InputXSLT {

WarningCapacitor::WarningCapacitor(ErrorMultiplexer* multiplexer):
	ErrorMultiplexer::receiver(multiplexer),
	warning_cache_(std::make_unique<warning_cache>()) { }

auto WarningCapacitor::discharge() -> warning_cache_ptr {
	warning_cache_ptr tmp(
		std::make_unique<warning_cache>()
	);

	std::swap(tmp, this->warning_cache_);

	return tmp;
}

void WarningCapacitor::receive(
	const ErrorMultiplexer::error_type type,
	const std::string& message
) {
	if ( type == ErrorMultiplexer::error_type::warning ) {
		this->warning_cache_->emplace_back(message);
	}
}

}
