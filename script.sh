#!/usr/bin/bash

mkdir bin

g++ ./src/server.cpp -o ./bin/server
g++ ./src/client.cpp -o ./bin/client
