.PHONY:
run: build
	cp build/pico-i2c-driver.c.uf2 /Volumes/RPI-RP2/

build: *.c
	rm -rf build
	mkdir -p build
	touch build/.nosync
	cd build && cmake .. && make -j8
