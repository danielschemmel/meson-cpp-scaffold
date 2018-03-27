#pragma once

#if __has_include(<string_view>)
	#include <string_view>
#endif

#if defined(__cpp_lib_string_view)
	namespace version {
		::std::string_view version() noexcept;
		::std::string_view last_modified() noexcept;
	}
#else
	#include <string>

	namespace version {
		::std::string version();
		::std::string last_modified();
	}
#endif
