#include "backtrace/backtrace.h"

#include <utility>

using namespace ::std::literals;

namespace backtrace {
	#if defined(__cpp_lib_string_view)
		backtrace::backtrace(::std::string_view indent) {
			constexpr static auto message = "[stack unwinding not supported on this platform]\n"sv;
			::std::string str;
			str.reserve(indent.size() + message.size());
			str.append(indent);
			str.append(message);
			trace = ::std::make_shared<::std::string const>(::std::move(str));
		}
	#else
		backtrace::backtrace(::std::string const& indent) {
			constexpr static auto message = "[stack unwinding not supported on this platform]\n";
			::std::string str;
			str.append(indent);
			str.append(message);
			trace = ::std::make_shared<::std::string const>(::std::move(str));
		}
	#endif
}