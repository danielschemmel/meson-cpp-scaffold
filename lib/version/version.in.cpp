#include "version/version.h"

using namespace ::std::literals;

namespace version {
	#if defined(__cpp_lib_string_view)
		::std::string_view version() noexcept {
			return "@VCS_TAG@"sv;
		}
	#else
		::std::string version() {
			return "@VCS_TAG@"s;
		}
	#endif
}
