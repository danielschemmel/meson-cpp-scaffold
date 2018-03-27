#pragma once

#include <memory>
#include <tuple>
#include <vector>
#include <string>
#if __has_include(<string_view>)
	#include <string_view>
#endif
#include <ostream>
#include <sstream>
#include <exception>

namespace backtrace {
	class backtrace : public ::std::exception {
		// exceptions should be noexcept-copy-able
		::std::shared_ptr<::std::string const> trace;

	public:
		#if defined(__cpp_lib_string_view)
			backtrace() : backtrace(::std::string_view{}) { }
			backtrace(::std::string_view indent);
		#else
			backtrace() : backtrace(::std::string{}) { }
			backtrace(::std::string const& indent);
		#endif
		backtrace(backtrace const&) noexcept = default;
		backtrace &operator=(backtrace const&) noexcept = default;
		backtrace(backtrace&&) noexcept = default;
		backtrace &operator=(backtrace&&) noexcept = default;

		// polymorphic classes should have at least one virtual member defined out of line
		char const* what() const noexcept override;

		friend ::std::ostream& operator<<(::std::ostream& out, backtrace const& self) {
			return out << *self.trace;
		}
	};

	class backtraced_exception : public ::std::exception {
		// exceptions should be noexcept-copy-able
		::std::shared_ptr<::std::string const> trace;

	public:
		backtraced_exception(::std::exception const& base, backtrace const& bt = backtrace("  ")) {
			using namespace ::std::literals;

			::std::stringstream out;
			#if defined(__cpp_lib_string_view)
				out << base.what() << "\nBacktrace:\n"sv << bt.what();
			#else
				out << base.what() << "\nBacktrace:\n" << bt.what();
			#endif
			trace = ::std::make_shared<::std::string const>(out.str());
		}
		backtraced_exception(backtraced_exception const&) noexcept = default;
		backtraced_exception &operator=(backtraced_exception const&) noexcept = default;
		backtraced_exception(backtraced_exception&&) noexcept = default;
		backtraced_exception &operator=(backtraced_exception&&) noexcept = default;

		// polymorphic classes should have at least one virtual member defined out of line
		char const* what() const noexcept override;

		friend ::std::ostream& operator<<(::std::ostream& out, backtraced_exception const& self) {
			return out << *self.trace;
		}
	};
}
