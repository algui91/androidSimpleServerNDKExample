#!/bin/bash

ndk-build
ant debug
adb install -r "$1"
