# Documentation

You may want to add some documentation to the [doc folder](doc/index.md).

# Directory structure

* `tool/foo/`        One distinct executable `foo`. It can contain internal headers and will be compiled to `${BUILD}/tool/foo/foo`.
* `test/sample.foo/` One distinct unit test `sample.foo`. It can contain internal headers and be run with `meson test -C ${BUILD} sample.foo`. All unit tests can be run with, e.g., `cd ${BUILD} ; meson test`.
* `lib/foo/`         One distinct library `foo`. It can contain internal headers.
* `include/foo/`     One distinct include directory for the library `foo`. It could be used by library `bar`.

We suggest to configure the following directories for compiling:

* `debug/`           Configure with `meson --buildtype=debug debug` and use for your day-to-day development.
* `release/`         Configure with `meson --buildtype=release release` and use for performance-sensitive experiments.

# Build System

This project uses `meson`, which enables a two-stage build process. To begin, configure a debug build, e.g., by `meson --buildtype=debug debug`. You can now build the project with your chosen backend (typically ninja): `ninja -C debug`.

Meson uses `meson.build` files to describe the compilation process. For example, `tool/backtracer/meson.build` contains the specification for the `backtracer` example tool, which consists of a single translation unit `main.cpp` and a library called `backtrace`. Similarly, `lib/backtrace/meson.build` describes how libbacktrace is built from one of two compilation units and a system library (`libunwind`). The definition of `sys_libunwind` comes from the `meson.build` file in the base directory.

## Warnings

The default warning level is intentionally chosen to be rather high. You may add additional warnings, but do not remove any warnings from the build configuration.

We will compile with the default warning level, 3, and expect your code to compile in the provided docker container without any warnings.

## Sanitizers

By default, debug builds will use both the Address Sanitizer and the Undefined Behavior Sanitizer, while release builds will not use either so as to not compromise performance.

## Unit Tests

Meson supports unit tests, which can be run with, e.g., `meson test -C debug` or `ninja -C debug test`.

Unit tests are implemented as small programs, which indicate success by their return code:
* A return code of 0 implies success.
* A return code of 77 indicates that the test was skipped.
* Any other return code can be used to indicate failure.

## Debugging

To debug your programs with `lldb`, `gdb`, `valgrind` and friends, you may need to disable sanitizers.

```
$ cd "${PROJECT}"
$ meson --buildtype=debug debug
$ cd debug
$ meson configure -Db_sanitize=none
$ ninja
$ lldb "${PROGRAM}"
(lldb) r
```

## Building On '${OS}'

While compilation may succeed on older compilers, it is strongly suggested to use either gcc in version 7 or newer or clang in version 6 or newer. Especially for debugging, support for ASAN and UBSAN is extremely useful.

### Using the Debian Dockerfile

The Debian Dockerfile provides an up-to-date Debian Testing based system.

```
$ cd "${PROJECT}/docker/debian"
$ docker build -t meson-scaffold . # build image
$ docker run -it --privileged -v $(realpath ${PROJECT}):/project/ -u $(id -u) --rm meson meson --buildtype=debug debug # oneshot command
$ docker run -it --privileged -v $(realpath ${PROJECT}):/project/ -u $(id -u) meson # persistent container w/ default g++
$ docker run -it --privileged -v $(realpath ${PROJECT}):/project/ -u $(id -u) -e CXX=g++-8 meson # persistent container w/ g++-8
```

### Arch Linux

```
$ sudo pacman -S base-devel ccache meson libunwind
$ cd "${PROJECT}"
$ meson --buildtype=debug debug
$ ninja -C debug
```

### Debian

Debian stretch (the "newest" stable branch at the time of writing) defaults to old software. Modern software is available on the testing branch.

```
$ sudo vi /etc/apt/sources.list # replace every occurence of "stretch" with the word "testing"
$ sudo apt update && sudo apt dist-upgrade && sudo apt autoremove
$ sudo apt install build-essential git ccache meson pkg-config libunwind-dev
$ cd "${PROJECT}"
$ meson --buildtype=debug debug
$ ninja -C debug
```

If you feel especially adventurous, you may install gcc version 8 with `sudo apt install g++-8` and instead configure with `env CXX=g++-8 meson --buildtype=debug debug`.

### MacOS

The XCode CLI utils provide a rather old version of `clang++` and alias it with the `c++` and `g++` commands. Homebrew provides an up-to-date version of `g++` and many libraries that are easily accessible on Linux.

```
$ # Install the XCode command line tools and Homebrew
$ brew install gcc ccache meson
$ cd "${PROJECT}"
$ export CXX="${GPLUSPLUS_PATH}" # typically /usr/local/bin/g++-7
$ meson --buildtype=debug debug
$ ninja -C debug
```

### Windows Subsystem for Linux (WSL)

Please remember that WSL is still very much experimental.
WSL works only in a rather limited fashion, as ASAN will only be reasonably supported as of Windows 10 build 17093. Microsoft has announced that the release of the feature upgrade that would have been based on this build is delayed due to a bug.
You want ASAN.

```
$ # Setup WSL and install the Debian Linux "App" from the Microsoft Store
$ sudo vi /etc/apt/sources.list # replace every occurence of "stretch" with the word "testing"
$ sudo apt update && sudo apt dist-upgrade && sudo apt autoremove
$ sudo apt install build-essential git ccache meson pkg-config libunwind-dev
```

Without ASAN:
```
$ cd "${PROJECT}"
$ meson --buildtype=debug debug
$ cd debug
$ meson configure -Db_sanitize=none
$ ninja
```

With ASAN:
```
$ sudo bash -c 'echo 1 > /proc/sys/vm/overcommit_memory' # setting t
$ cd "${PROJECT}"
$ meson --buildtype=debug debug
$ ninja -C debug
```

If you feel especially adventurous, you may install gcc version 8 with `sudo apt install g++-8` and instead configure with `env CXX=g++-8 meson --buildtype=debug debug`.

### Cygwin

While compilation can succeed, the Cygwin compiler is fairly old (g++ 6.4 at the time of writing) and neither ASAN nor UBSAN are supported.

### MinGW

Configuration is not currently successful on MinGW. While MinGW would support a modern `g++`, neither ASAN or UBSAN are supported.
