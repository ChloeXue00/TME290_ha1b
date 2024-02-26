# opendlv-device-motor-kiwi2

TODO: Complete this file.

## Compile

For `amd64` (cross compile for `arm64`):

> docker run --privileged --rm tonistiigi/binfmt --install all
> docker buildx create --name multiplatformbuilder --use
> docker buildx build --platform "linux/arm64" .

## Run

> docker run -ti --rm registry.opendlv.org/testing/opendlv-device-motor-kiwi2:1 --cid=111 --verbose
