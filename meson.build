project('omega', 'cpp', default_options: [
	'cpp_std=c++1z',
	'warning_level=3',
	get_option('buildtype') == 'release' ? 'b_lto=true' : 'b_lto=false',
	get_option('buildtype') == 'release' ? 'b_sanitize=none' : 'b_sanitize=address,undefined',
	'b_asneeded=false', # multithreaded code tends to fail with -Wl,--as-needed
])

compiler = meson.get_compiler('cpp')
warning_level = get_option('warning_level').to_int()

if compiler.get_id() == 'gcc'
	if get_option('buildtype') == 'debug'
		add_global_arguments('-Og', language: 'cpp')
	endif
	if target_machine.cpu_family() == 'x86_64'
		add_global_arguments('-maes', language: 'cpp')
	endif

	foreach arg : [
		'-Wall',
		'-Wextra',
		'-pedantic',
		'-Wcast-align',
		'-Wcast-qual',
		'-Wconversion',
		'-Wduplicated-cond',
		'-Wfloat-equal',
		'-Wformat-signedness',
		'-Wmaybe-uninitialized',
		'-Wmissing-declarations',
		'-Wmultistatement-macros',
		'-Wnull-dereference',
		'-Wold-style-cast',
		'-Wsign-conversion',
		'-Wundef',
		'-Wuninitialized',
	]
		if compiler.has_argument(arg)
			add_global_arguments([arg], language: 'cpp')
		else
			warning('Your compiler does not support \'' + arg + '\'')
		endif
	endforeach
	if warning_level > 1
		foreach arg : [
			'-Wdouble-promotion',
			'-Wduplicated-branches',
			'-Wimplicit-fallthrough=5',
			'-Wmissing-include-dirs',
			'-Wstrict-overflow=5',
			'-Wsuggest-attribute=noreturn',
			'-Wswitch-bool',
			'-Wswitch-enum',
			'-Wzero-as-null-pointer-constant',
		]
			if compiler.has_argument(arg)
				add_global_arguments([arg], language: 'cpp')
			else
				warning('Your compiler does not support \'' + arg + '\'')
			endif
		endforeach
	endif
	if warning_level > 2
		foreach arg : [
			'-Walloc-zero',
			'-Walloca',
			'-Wcast-align=strict',
			'-Wnormalized=nfc',
			'-Wplacement-new=2',
			'-Wredundant-decls',
			'-Wsuggest-attribute=format',
			'-Wsuggest-final-methods',
			'-Wsuggest-final-types',
			'-Wsuggest-override',
			'-Wtrampolines',
			'-Wuseless-cast',
		]
			if compiler.has_argument(arg)
				add_global_arguments([arg], language: 'cpp')
			else
				warning('Your compiler does not support \'' + arg + '\'')
			endif
		endforeach
	endif
elif compiler.get_id() == 'clang'
	if get_option('buildtype') == 'debug'
		# add_global_arguments('-Og', language: 'cpp')
	endif

	if warning_level < 2
		foreach arg : [
			'-Wall',
			'-Wextra',
			'-pedantic',
			'-Wcast-align',
			'-Wcast-qual',
			'-Wconversion',
			'-Wfloat-equal',
			'-Wmissing-declarations',
			'-Wnull-dereference',
			'-Wold-style-cast',
			'-Wsign-conversion',
			'-Wundef',
			'-Wuninitialized',
		]
			if compiler.has_argument(arg)
				add_global_arguments([arg], language: 'cpp')
			else
				warning('Your compiler does not support \'' + arg + '\'')
			endif
		endforeach
	endif
	if warning_level > 1 and warning_level < 2
		foreach arg : [
			'-Wdouble-promotion',
			'-Wimplicit-fallthrough',
			'-Wmissing-include-dirs',
			'-Wstrict-overflow=5',
			'-Wswitch-bool',
			'-Wswitch-enum',
			'-Wzero-as-null-pointer-constant',
		]
			if compiler.has_argument(arg)
				add_global_arguments([arg], language: 'cpp')
			else
				warning('Your compiler does not support \'' + arg + '\'')
			endif
		endforeach
	endif
	if warning_level > 2
		if compiler.has_argument('-Weverything')
			add_global_arguments(['-Weverything'], language: 'cpp')
		else
			error('Your compiler does not support \'-Weverything\' - warning level of 2 is not supported for your version of clang')
		endif
		foreach arg : [
			'-Wno-c++98-compat',
			'-Wno-c++98-compat-pedantic',
			'-Wno-padded',
			'-Wno-shadow-field-in-constructor',
		]
			if compiler.has_argument(arg)
				add_global_arguments([arg], language: 'cpp')
			else
				warning('Your compiler does not support \'' + arg + '\'')
			endif
		endforeach
	endif
else
	error('unsupported compiler')
endif

is_std_cpp11 = compiler.run('int main() { return __cplusplus >= 201103L ? 0 : 1; }', name: 'c++11 version validation')
is_std_cpp14 = compiler.run('int main() { return __cplusplus >= 201402L ? 0 : 1; }', name: 'c++14 version validation')
is_std_cpp17 = compiler.run('int main() { return __cplusplus >= 201703 ? 0 : 1; }', name: 'c++17 version validation')
if not is_std_cpp11.compiled() or not is_std_cpp14.compiled() or not is_std_cpp17.compiled()
	error('Your compiler is very broken.')
endif
if is_std_cpp11.returncode() != 0
	error('Your compiler does not even identify as C++11 compatible.')
elif is_std_cpp14.returncode() != 0
	warning('Your compiler does not identify as C++14 compatible. You should upgrade.')
elif is_std_cpp17.returncode() != 0
	warning('Your compiler does not identify as C++17 compatible.')
endif

if not compiler.has_header('string_view')
	warning('Your compiler lacks <string_view>')
endif

if not compiler.has_header('optional')
	warning('Your compiler lacks <optional>')
endif

if not compiler.has_header('variant')
	warning('Your compiler lacks <variant>')
endif

if not compiler.compiles('#include <cstddef> int main() { ::std::byte b{0xFF}; }', name: '::std::byte check')
	warning('Your compiler lacks ::std::byte')
endif

if not compiler.has_header('filesystem')
	if compiler.has_header('experimental/filesystem')
		warning('Your compiler lacks <filesystem> (but provides <experimental/filesystem>)')
	else
		warning('Your compiler lacks <filesystem>')
	endif
endif

if target_machine.system() == 'darwin' # macos automatically links libunwind
	unwind_mode = 'libunwind'
	sys_libunwind = dependency('', required: false)
elif target_machine.system() == 'linux'
	unwind_mode = 'libunwind'
	sys_libunwind = dependency('libunwind')
else
	unwind_mode = 'disabled'
endif

include = include_directories('include')
# cxxopts = include_directories('external/cxxopts/include')

subdir('lib')
subdir('tool')
subdir('test')
