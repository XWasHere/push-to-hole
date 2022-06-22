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
CXX_FLAGS  = 
CXX_LFLAGS = 

all: cli

.PHONY: clean cli all

cli: out/push-to-hole

clean:
	mkdir -p build
	mkdir -p out
	rm -f build/*
	rm -f out/*

out/push-to-hole: build/cli.o
	${CXX} ${CXX_LFLAGS} build/cli.o -o out/push-to-hole
	
build/cli.o: src/cli.cc
	${CXX} ${CXX_FLAGS} -c src/cli.cc -o build/cli.o