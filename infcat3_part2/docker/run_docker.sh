#!/bin/sh
docker build -t "infcat3_part2" .
docker run -id -p 36975:36975 -h "infcat3_part2" --name="infcat3_part2" infcat3_part2
