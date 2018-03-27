#include "backtrace/backtrace.h"

#include <functional>
#include <iostream>

namespace {
	void thrower[[noreturn]]() {
		throw ::backtrace::backtrace("  ");
	}

	void intermediate(bool const breakit) {
		if(breakit) {
			try {
				thrower();
			} catch(::backtrace::backtrace const& bt) {
				throw ::backtrace::backtraced_exception(bt);
			}
		}
	}

	void weird(bool const breakit = true) {
		try {
			return intermediate(breakit);
		} catch(...) {
			auto lambda = []{
				intermediate(true);
			};
			::std::invoke(lambda);
			throw;
		}
	}
}

int main() {
	::backtrace::backtrace bt("  ");
	// throw bt; // try what happens when this line is enabled!

	try {
		intermediate(true);
	} catch(::std::exception const& e) {
		::std::cout << "exception from intermediate function:\n" << e.what() << "\n";
	}

	try {
		weird(true);
	} catch(::std::exception const& e) {
		::std::cout << "exception from weird function:\n" << e.what() << "\n";
	}

	::std::cout << "main backtrace:\n" << bt << "\n";

	return 0;
}
