FROM debian:testing-slim

RUN ["/bin/bash", "-c", "set -e ; set -o pipefail ; apt-get update ; apt-get dist-upgrade -y ; apt-get autoremove -y ; apt-get install -y sudo build-essential git ccache meson pkg-config libunwind-dev g++-8 clang-6.0 lldb-6.0 gdb valgrind"]

CMD ["/bin/bash", "-l"]
WORKDIR /project
ENV CCACHE_DIR=/project/.ccache
