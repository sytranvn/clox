.PHONY: clean config build

all: config build

clean:
	rm -rf build

config:
	cmake -S. -Bbuild

build:
	cmake --build build

