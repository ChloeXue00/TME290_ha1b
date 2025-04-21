# Copyright (C) 2024 OpenDLV

FROM alpine:3.19 AS builder

RUN apk update && \
    apk --no-cache add \
        cmake \
        g++ \
        make \
        linux-headers \
        git \
        openssl \
        libstdc++ \
        clang

# install Cluon
RUN apk add libcluon --no-cache --repository \
      https://chrberger.github.io/libcluon/alpine/v3.13 --allow-untrusted
#copy the entire proj
ADD . /opt/sources
WORKDIR /opt/sources
# RUN mkdir /tmp/build && cd /tmp/build && \
#     cmake /opt/sources && \
#     make

# ADD CMakeLists.txt FindCluonMsc.cmake /opt/sources/
# ADD src /opt/sources/src

#build and install 
RUN mkdir /opt/build /opt/out && \
    cd /opt/build && \
    cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/opt/out \
      /opt/sources && \
    make && make install


FROM alpine:3.19

#RUN apk update && \
#    apk --no-cache add \
#        some-runtime-package
RUN apk update && apk --no-cache add libstdc++ && mkdir -p /usr/lib

COPY --from=builder /opt/out/ /usr
ENTRYPOINT ["/usr/bin/opendlv-template-microservice"]
CMD ["--cid=132", "--verbose"]
