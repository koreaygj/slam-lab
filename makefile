prepare:
	rm -rf build
	mkdir build

compile: 
	cd ./build && cmake ..
	cd build && make
	ln -sf build/compile_commands.json .

execute:
	cd build && ./eigenMatrix