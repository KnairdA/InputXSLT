#include "error_capacitor.h"

namespace InputXSLT {

ErrorCapacitor::ErrorCapacitor(ErrorMultiplexer* multiplexer):
	multiplexer_(multiplexer),
	error_cache_(new error_cache()) {
	this->multiplexer_->connectReceiver(this);
}

ErrorCapacitor::~ErrorCapacitor() {
	this->multiplexer_->disconnectReceiver(this);
}

void ErrorCapacitor::discharge() {
	if ( !this->error_cache_->empty() ) {
		throw exception(std::move(this->error_cache_));
	}
}

void ErrorCapacitor::receive(
	const ErrorMultiplexer::ErrorType type,
	const std::string& message
) {
	if ( type == ErrorMultiplexer::ErrorType::Error ) {
		this->error_cache_->emplace_back(message);
	}
}

ErrorCapacitor::exception::exception(error_cache_ptr ptr):
	error_cache_(std::move(ptr)) { }

auto ErrorCapacitor::exception::getCachedErrors() const -> const error_cache* {
	return this->error_cache_.get();
}

}
