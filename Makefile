BUILD_DIR?=build
MAKE_FLAGS?=-j1

default:
	@grep -v "^	" Makefile | grep -v "^$$"

clean:
	rm -Rf ${BUILD_DIR}
	rm -Rf include/cpput/cpput_config.h

build:
	git submodule update --init
	mkdir -p ${BUILD_DIR}
	cd ${BUILD_DIR}; cmake ..
	cd ${BUILD_DIR}; make ${MAKE_FLAGS}

test: build
	cd build; ${MAKE} ${MAKE_FLAGS} test


#----------------------------------------------
# utils
#----------------------------------------------

addutils:
	git remote add cmakeut https://github.com/asherikov/cmakeut
	git remote add cpput https://github.com/asherikov/cpput

updateutils:
	git fetch --all
	git show remotes/cmakeut/master:cmake/FindEigen3.cmake                          > cmake/FindEigen3.cmake
	git show remotes/cmakeut/master:cmake/cmakeut_add_external_git_project.cmake    > cmake/cmakeut_add_external_git_project.cmake
	git show remotes/cmakeut/master:cmake/cmakeut_compiler_flags.cmake              > cmake/cmakeut_compiler_flags.cmake
	git show remotes/cmakeut/master:cmake/cmakeut_detect_func_macro.cmake           > cmake/cmakeut_detect_func_macro.cmake
	git show remotes/cpput/master:cmake/cpput_config.cmake      > cmake/cpput_config.cmake
	git show remotes/cpput/master:include/cpput/config.h.in     > include/eigenut/cpput/config.h.in
	git show remotes/cpput/master:include/cpput/exception.h     > include/eigenut/cpput/exception.h
	git show remotes/cpput/master:include/cpput/visibility.h    > include/eigenut/cpput/visibility.h

.PHONY: build test