#linux only maybe macos too
debug:
	cmake --no-warn-unused-cli -DCMAKE_BUILD_TYPE:STRING=Debug -S . -B ./build/Debug
	cmake --build ./build/Debug --config Debug --target all -j`nproc 2>/dev/null || getconf NPROCESSORS_CONF`

release:
	cmake --no-warn-unused-cli -DCMAKE_BUILD_TYPE:STRING=Release -S . -B ./build/Release
	cmake --build ./build/Release --config Release --target all -j`nproc 2>/dev/null || getconf NPROCESSORS_CONF`
