#include "backtrace/backtrace.h"

#include <utility>
#include <array>
#include <memory>
#include <iomanip>
#include <cstdlib>

using namespace ::std::literals;

#include <cxxabi.h>

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-macros"
#endif

#define UNW_LOCAL_ONLY

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#include <libunwind.h>

namespace {
	unw_word_t get_function_address(unw_cursor_t& cursor) {
		unw_proc_info_t proc_info{};
		if(unw_get_proc_info(&cursor, &proc_info) < 0) {
			return 0;
		}
		return proc_info.start_ip;
	}
}

namespace backtrace {
	#if defined(__cpp_lib_string_view)
		backtrace::backtrace(::std::string_view indent) {
	#else
		backtrace::backtrace(::std::string const& indent) {
	#endif
		::std::ostringstream out;
		out << ::std::hex;

		unw_context_t context;
		unw_getcontext(&context);
		unw_cursor_t cursor;
		unw_init_local(&cursor, &context);
		auto buffer = ::std::make_unique<::std::array<char, 1024>>();
		::std::unique_ptr<char, decltype(&::std::free)> demangled_str(nullptr, &::std::free);
		::std::size_t demangled_len = 0;
		do {
			out << indent;
			auto faddr = get_function_address(cursor);
			unw_word_t offset;
			if(auto rc = unw_get_proc_name(&cursor, buffer->data(), buffer->size(), &offset); rc == 0 || rc == UNW_ENOMEM) {
				int status;
				{ // Sometimes a pointer needs to be unwrapped to properly access a C-style API. To remain exception safe, immediately wrap it again!
					char* raw = demangled_str.release();
					if(char* result = ::abi::__cxa_demangle(buffer->data(), raw, &demangled_len, &status)) {
						demangled_str.reset(result);
					} else {
						demangled_str.reset(raw);
					}
				}
				if(status == 0) {
					out << demangled_str.get();
				} else {
					out << buffer->data();
				}
				if(faddr) {
					out << " @ 0x"sv << faddr;
				}
				out << " + 0x"sv << offset << "\n"sv;
			} else {
				if(faddr) {
					out << "[unknown stack frame] @ "sv << faddr << "\n"sv;
				} else {
					out << "[unknown stack frame]\n"sv;
				}
			}
		} while(unw_step(&cursor) > 0);

		auto str = out.str();
		if(str.empty()) {
			str = "[no call stack information available]"s;
		}
		trace = ::std::make_shared<::std::string const>(::std::move(str));
	}
}
