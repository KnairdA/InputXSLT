#include "warning_capacitor.h"

namespace InputXSLT {

WarningCapacitor::WarningCapacitor(ErrorMultiplexer* multiplexer):
	ErrorMultiplexer::receiver(multiplexer),
	warning_cache_(new warning_cache()) { }

auto WarningCapacitor::discharge() -> warning_cache_ptr {
	warning_cache_ptr tmp(std::move(this->warning_cache_));

	this->warning_cache_.reset(new warning_cache());

	return std::move(tmp);
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
