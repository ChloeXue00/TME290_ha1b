# Microservice templates and code format guidlines

This repository contains the official template and coding guidlines for OpenDLV
microserices. All microservices in the `core` and `community` groups.

## Microserrvice templates

This repository contains the following templates:
* `opendlv-template-microservice-basic`: This microservice is a bare minimum microservice that triggers for a single input, with no output.

## Microservice naming conventions

An OpenDLV microservice name contains the following parts (in order):
* `opendlv`/`dlv`: To indicate if the microservice is open-source or not. All microservices on `git.opendlv.org` should be open-source.
* `control`/`data`/`device`/`gateway`/`logic`/`perception`/`virtual`: To indicate the microserivce type.
* For example `camera`/`lidar`/`gnss` to indicate the sub-type, with standardized sub-types within each type.
* The protocol or function of the microserivce, for example `device-gnss-nmea` or `control-lateral-singletrackmodel`. Note that there should be NO word separation (dashes or camel case).
* Optional. An implementation variant, for example `perception-detect-trafficsign-colorfilter` or `perception-detect-trafficsign-dnn`.

