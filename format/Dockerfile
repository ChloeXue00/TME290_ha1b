# Copyright (C) 2024 OpenDLV

FROM alpine:3.19

RUN apk update && \
    apk --no-cache add \
        clang17-extra-tools

ADD opendlv-clangformat /

ENTRYPOINT ["clang-format", "--style=file:/opendlv-clangformat"]
