#!/bin/sh
docker build -t "bbs" .
docker run -id -p 30428:30428 -h "bbs" --name="bbs" bbs
