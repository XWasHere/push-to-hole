#    hole uploader
#    Copyright (C) 2022 xwashere
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <https://www.gnu.org/licenses/>.

CXX        = g++
CXX_FLAGS  = --std=c++23
CXX_SFLAGS = -fPIC
CXX_LFLAGS = 

all: cli libhole

.PHONY: clean cli all libhole

libhole: out/libhole.so
out/libhole.so: build/hole.o build/client.o build/object.o
	${CXX} ${CXX_LFLAGS} ${CXX_SFLAGS} -rdynamic -shared build/hole.o build/client.o build/object.o -o out/libhole.so -lcurl

build/client.o: src/client.cc src/client.h src/object.h
	${CXX} ${CXX_FLAGS} ${CXX_SFLAGS} -c src/client.cc -o build/client.o

build/hole.o: src/hole.cc src/hole.h
	${CXX} ${CXX_FLAGS} ${CXX_SFLAGS} -c src/hole.cc -o build/hole.o

build/object.o: src/object.cc src/object.h
	${CXX} ${CXX_FLAGS} ${CXX_SFLAGS} -c src/object.cc -o build/object.o

clean:
	mkdir -p build
	mkdir -p out
	rm -f build/*
	rm -f out/*

cli: out/push-to-hole 
out/push-to-hole: build/cli.o libhole
	${CXX} ${CXX_LFLAGS} build/cli.o -o out/push-to-hole -Lout -lhole
	
build/cli.o: src/cli.cc
	${CXX} ${CXX_FLAGS} -c src/cli.cc -o build/cli.o