#!/bin/sh
docker build -t "infcat3" .
docker run -id -p 36973:36973 -h "infcat3" --name="infcat3" infcat3
