FROM ubuntu:25.10

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
        gcc-15 \
        g++-15 \
        cmake \
        make \
        git \
        gdb \
        valgrind \
    && update-alternatives --install /usr/bin/gcc  gcc  /usr/bin/gcc-15  15 \
    && update-alternatives --install /usr/bin/g++  g++  /usr/bin/g++-15  15 \
    && update-alternatives --install /usr/bin/cc   cc   /usr/bin/gcc-15  15 \
    && update-alternatives --install /usr/bin/c++  c++  /usr/bin/g++-15  15 \
    && rm -rf /var/lib/apt/lists/*

# versions
RUN gcc --version && g++ --version && cmake --version

WORKDIR /project

CMD ["/bin/bash"]
