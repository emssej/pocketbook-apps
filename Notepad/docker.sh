#!/usr/bin/env sh

docker run --rm -it --mount type=bind,source="$(pwd)",target=/project 5keeve/pocketbook-sdk:6.3.0-b288-v1
