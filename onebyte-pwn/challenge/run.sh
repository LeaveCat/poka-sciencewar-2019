#!/bin/bash
PORT=1234
docker build -t "onebyte-pwn" .
docker run -d -p "0.0.0.0:$PORT:9999" -h "onebyte-pwn" --name="onebyte-pwn" onebyte-pwn
