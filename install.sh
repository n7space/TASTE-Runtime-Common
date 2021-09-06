#!/bin/bash

PREFIX=/home/taste/tool-inst
SOURCES=$(dirname $0)

mkdir -p "${PREFIX}/include/TASTE-Runtime-Common/src"
cp -r "${SOURCES}/src/Packetizer" "${PREFIX}/include/TASTE-Runtime-Common/src"
cp -r "${SOURCES}/src/Broker" "${PREFIX}/include/TASTE-Runtime-Common/src"
cp -r "${SOURCES}/src/RuntimeCommon" "${PREFIX}/include/TASTE-Runtime-Common/src"
