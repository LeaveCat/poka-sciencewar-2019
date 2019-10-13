#!/bin/bash
PORT=8888
docker build -t "easyvm-pwn" .
docker run -d -p "0.0.0.0:$PORT:8888" -h "easyvm-pwn" --name="easyvm-pwn" easyvm-pwn
