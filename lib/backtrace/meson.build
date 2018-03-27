deps = []

if unwind_mode == 'libunwind'
	deps += [ sys_libunwind ]
elif unwind_mode == 'disabled'
else
	error('unknown unwind mode "' + unwind_mode + '"')
endif

libbacktrace = declare_dependency(
	link_with: static_library('backtrace',
		'backtrace-' + unwind_mode + '.cpp',
		'backtrace.cpp',
		dependencies: [
		] + deps,
		include_directories: include,
	),
)
