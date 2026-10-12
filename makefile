VCPKG-PATH=$(HOME)/vcpkg/scripts/buildsystems/vcpkg.cmake
BUILD-TYPE=Debug

SCENE-EXE=build/k-lib
TEST-EXE=build/k-lib-tests


.PHONY: build test check-warnings memcheck static-analysis clean


# --------------------------------------------------
# Build application
# --------------------------------------------------

build: clean
	cmake -B build/ \
		-DCMAKE_BUILD_TYPE=$(BUILD-TYPE) \
		-DCMAKE_TOOLCHAIN_FILE=$(VCPKG-PATH)

	cmake --build ./build --target Kabuki


# --------------------------------------------------
# Run tests
# --------------------------------------------------
test:
	cmake -B build/ \
		-DCMAKE_BUILD_TYPE=$(BUILD-TYPE) \
		-DCMAKE_TOOLCHAIN_FILE=$(VCPKG-PATH)

	cmake --build ./build --target k-lib-tests

	ctest --test-dir build --output-on-failure


# --------------------------------------------------
# Check warnings
# --------------------------------------------------

check-warnings: clean
	cmake -B build/ \
		-DCMAKE_BUILD_TYPE=$(BUILD-TYPE) \
		-DCMAKE_TOOLCHAIN_FILE=$(VCPKG-PATH) \
		-DCMAKE_COMPILE_WARNING_AS_ERROR=0

	cmake --build ./build --target k-lib


# --------------------------------------------------
# Valgrind
# --------------------------------------------------

memcheck:
	valgrind \
		--quiet \
		--leak-check=full \
		--leak-resolution=med \
		--track-origins=yes \
		--error-exitcode=-1 \
		--vgdb=no \
		$(SCENE-EXE) \
		--gtest_filter=-*Playground*

	@# No leaks but invalid accesses are detected
	@#valgrind --quiet --leak-check=no --leak-resolution=med --track-origins=yes --error-exitcode=-1 --vgdb=no $(SCENE-EXE) --gtest_filter=*


# --------------------------------------------------
# Static analysis
# --------------------------------------------------

static-analysis:
	cmake -B build/ \
		-DCMAKE_BUILD_TYPE=$(BUILD-TYPE) \
		-DCMAKE_TOOLCHAIN_FILE=$(VCPKG-PATH)

	cmake --build ./build --target clang-tidy


# --------------------------------------------------
# Clean
# --------------------------------------------------

clean:
	rm -rf build/