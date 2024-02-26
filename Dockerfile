# Copyright (C) 2024 OpenDLV
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

FROM alpine:edge as builder

RUN apk update && \
    apk --no-cache \
        --repository=http://dl-cdn.alpinelinux.org/alpine/edge/testing/ add \
        cmake \
        g++ \
        make \
        linux-headers \
        pigpio \
        pigpio-dev

ADD . /opt/sources
RUN mkdir /opt/build /opt/out && \
    cd /opt/build && \
    cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/opt/out \
      /opt/sources && \
    make && make install


FROM alpine:edge

RUN apk update && \
    apk --no-cache \
        --repository=http://dl-cdn.alpinelinux.org/alpine/edge/testing/ add \
        pigpio

COPY --from=builder /opt/out/ /usr
ENTRYPOINT ["/usr/bin/opendlv-device-motor-kiwi2"]
