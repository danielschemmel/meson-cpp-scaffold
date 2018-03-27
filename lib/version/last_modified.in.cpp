#include "version/version.h"

using namespace ::std::literals;

namespace version {
	#if defined(__cpp_lib_string_view)
		::std::string_view last_modified() noexcept {
			return "@LAST_MODIFIED@"sv;
		}
	#else
		::std::string last_modified() {
			return "@LAST_MODIFIED@"s;
		}
	#endif
}
