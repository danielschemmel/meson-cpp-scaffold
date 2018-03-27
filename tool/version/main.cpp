#include "version/version.h"

#include <cassert>
#include <iostream>

namespace {
	::std::ostream& print_cplusplus_version(::std::ostream& out) {
		out << __cplusplus;
		if(__cplusplus < 199711L) {
			out << " (this is a non-standard value)";
		} else if(__cplusplus == 199711L) {
			out << " (this is the standard value for C++98/03)";
		} else if(__cplusplus > 199711L && __cplusplus < 201103L) {
			out << " (this is a non-standard value indicating a version between C++98/03 and C++11)";
		} else if(__cplusplus == 201103L) {
			out << " (this is the standard value for C++11)";
		} else if(__cplusplus > 201103L && __cplusplus < 201402L) {
			out << " (this is a non-standard value indicating a version between C++11 and C++14)";
		} else if(__cplusplus == 201402L) {
			out << " (this is the standard value for C++14";
		} else if(__cplusplus > 201402L && __cplusplus < 201703L) {
			out << " (this is a non-standard value indicating a version between C++14 and C++17)";
		} else if(__cplusplus == 201703L) {
			out << " (this is the standard value for C++17)";
		} else if(__cplusplus > 201703L) {
			out << " (this is a non-standard value indicating a version after C++17)";
		} else assert(0 && "unreachable");
		return out;
	}

	::std::ostream& print_os(::std::ostream& out) {
		#if defined(_WIN32)
			// also defined for WIN64
			out << "Windows";
		#elif defined(__APPLE__)
			#include "TargetConditionals.h"
			#if TARGET_IPHONE_SIMULATOR
				out << "iOS Simulator";
			#elif TARGET_OS_IPHONE
				out << "iOS";
			#elif TARGET_OS_MAC
				out << "MacOS";
			#else
				#error "Unknown Apple platform"
			#endif
		#elif defined(__CYGWIN__)
			out << "Cygwin";
		#elif defined(__linux__)
			out << "Linux";
		#elif defined(__unix__)
			out << "Generic UNIX";
		#elif defined(_POSIX_VERSION)
			out << "Generic POSIX-compatible Platform";
		#else
			#error "Unknown Platform"
		#endif
		return out;
	}

	::std::ostream& print_arch(::std::ostream& out) {
		#if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64) || defined(_M_X64) || defined(_M_AMD64)
			out << "x86_64";
		#elif defined(__i386__) || defined(_M_IX86) || defined(__X86__) || defined(__I86__) || defined(__THW_INTEL__) || defined(__INTEL__) || defined(__386)
			out << "x86";
		#elif defined(__aarch64__)
			out << "ARM64";
		#elif defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
			out << "ARM7";
		#elif defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) || defined(__ARM_ARCH_6ZK__) || defined(__ARM_ARCH_6T2__)
			out << "ARM6";
		#elif defined(__ARM_ARCH_5__) || defined(__ARM_ARCH_5E__) || defined(__ARM_ARCH_5T__) || defined(__ARM_ARCH_5TE__) || defined(__ARM_ARCH_5TEJ__)
			out << "ARM5";
		#elif defined(__ARM_ARCH_4__) || defined(__ARM_ARCH_4T__)
			out << "ARM4";
		#elif defined(__ARM_ARCH_3__) || defined(__ARM_ARCH_3M__)
			out << "ARM3";
		#elif defined(__ARM_ARCH_2__)
			out << "ARM2";
		#else
			#error "Unknown Architecture"
		#endif
		return out;
	}
}

int main() {
	::std::cout << "Version " << ::version::version() << "\n";
	::std::cout << "Last Modified " << ::version::last_modified() << "\n";
	print_cplusplus_version(::std::cout << "C++ Version Macro: ") << "\n";
	print_arch(print_os(::std::cout << "Built for ") << " (") << ")\n";

	return 0;
}
