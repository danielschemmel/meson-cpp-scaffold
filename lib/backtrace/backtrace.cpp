#include <backtrace/backtrace.h>

namespace backtrace {
	char const* backtrace::what() const noexcept {
		return trace->c_str();
	}

	char const* backtraced_exception::what() const noexcept {
		return trace->c_str();
	}
}