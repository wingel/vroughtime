#!/bin/bash

set -euxo pipefail

CC=clang

rm -f *.o client
$CC -c ../tweetnacl.c -o tweetnacl.o
$CC -c ../base64.c -o base64.o
$CC -Wall -g -DDEBUG -c ../vrt.c -o vrt.o

$CC -I ../ -Wall -g -DDEBUG -o client vrt_client_unix.c vrt.o tweetnacl.o base64.o

set +e
./client
./client roughtime.weinigel.se 2002 "GdGp9m5AbwqCOpTVYq+yliBIrzh1wed7VYRS5CxMY3U=" 1
./client roughtime.cloudflare.com 2002 "gD63hSj3ScS+wuOeGrubXlq35N1c5Lby/S+T7MNTjxo=" 0

# go run test_integration.go
