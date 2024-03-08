# Copyright (C) 2024 OpenDLV

FROM alpine:3.19 as builder

RUN apk update && \
    apk --no-cache add \
        cmake \
        g++ \
        make \
        linux-headers

ADD . /opt/sources
RUN mkdir /opt/build /opt/out && \
    cd /opt/build && \
    cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/opt/out \
      /opt/sources && \
    make && make install


FROM alpine:3.19

#RUN apk update && \
#    apk --no-cache add \
#        some-runtime-package

COPY --from=builder /opt/out/ /usr
ENTRYPOINT ["/usr/bin/opendlv-template-microservice"]
