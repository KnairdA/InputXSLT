#include "error_capacitor.h"

namespace InputXSLT {

ErrorCapacitor::ErrorCapacitor(ErrorMultiplexer* multiplexer):
	ErrorMultiplexer::receiver(multiplexer),
	error_cache_(std::make_unique<error_cache>()) { }

void ErrorCapacitor::discharge() {
	if ( !this->error_cache_->empty() ) {
		throw exception(std::move(this->error_cache_));
	}
}

void ErrorCapacitor::receive(
	const ErrorMultiplexer::error_type type,
	const std::string& message
) {
	if ( type == ErrorMultiplexer::error_type::error ) {
		this->error_cache_->emplace_back(message);
	}
}

ErrorCapacitor::exception::exception(std::unique_ptr<error_cache> ptr):
	error_cache_(std::move(ptr)) { }

auto ErrorCapacitor::exception::operator*() const -> const error_cache& {
	return *(this->error_cache_.get());
}

}
