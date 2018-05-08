libversion_version_tag = custom_target('libversion_version_tag',
	build_always: true,
	command: ['bash', '-c', '''set -e; set -o pipefail;
		version=$(cd "$0" && git describe --always --dirty=.dirty --long 2>/dev/null || echo INDETERMINATE)
		if sed -e "s/$3/${version}/g" "$1" | cmp -s "$2" - ; then
			echo "already up to date"
		else
			sed -e "s/$3/${version}/g" "$1" >"$2"
		fi
	''', meson.source_root(), '@INPUT@', '@OUTPUT@', '@VCS_TAG@'],
	input: 'version.in.cpp',
	output: 'version.cpp',
)

libversion_last_modified = custom_target('libversion_last_modified',
	build_always: true,
	command: ['bash', '-c', '''set -e; set -o pipefail;
		lastmod=$(find "$0"/include "$0"/lib "$0"/tool "$0"/test -type f | while read x ; do
			date -r "$x" -u "+%Y-%m-%d %H:%M:%S %Z" ;
		done | sort -r | head -n 1)
		if sed -e "s/$3/${lastmod}/g" "$1" | cmp -s "$2" - ; then
			echo "already up to date"
		else
			sed -e "s/$3/${lastmod}/g" "$1" >"$2"
		fi
	''', meson.source_root(), '@INPUT@', '@OUTPUT@', '@LAST_MODIFIED@'],
	input: 'last_modified.in.cpp',
	output: 'last_modified.cpp',
)

libversion = declare_dependency(
	link_with: static_library('version',
		libversion_version_tag,
		libversion_last_modified,
		dependencies: [
		],
		include_directories: include,
	),
)
