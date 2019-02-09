BUILD_DIR?=build
MAKE_FLAGS?=-j1

default:
	@grep -v "^	" Makefile | grep -v "^$$"

clean:
	rm -Rf ${BUILD_DIR}
	ls include/cpput/*.in | sed 's/\.in$$//g' | xargs rm -f

build:
	mkdir -p ${BUILD_DIR}
	cd ${BUILD_DIR}; cmake ..
	cd ${BUILD_DIR}; make ${MAKE_FLAGS}

test: build
	cd build; ${MAKE} ${MAKE_FLAGS} test


addutils:
	git remote add cmakeut https://github.com/asherikov/cmakeut
	git remote add better_enums https://github.com/aantron/better-enums
	git remote add popl https://github.com/badaix/popl
	git remote add safe_int https://github.com/dcleblanc/SafeInt

3rdparty:
	git fetch --all
	git show remotes/cmakeut/master:cmake/cmakeut_add_cpp_test.cmake        > cmake/cmakeut_add_cpp_test.cmake
	git show remotes/cmakeut/master:cmake/cmakeut_compiler_flags.cmake      > cmake/cmakeut_compiler_flags.cmake
	git show remotes/cmakeut/master:cmake/cmakeut_detect_func_macro.cmake   > cmake/cmakeut_detect_func_macro.cmake
	git show remotes/cmakeut/master:cmake/cmakeut_list_filenames.cmake      > cmake/cmakeut_list_filenames.cmake
	git show remotes/better_enums/master:enum.h    > include/cpput/better_enum.h
	git show remotes/popl/master:include/popl.hpp  > include/cpput/popl.h
	git show remotes/safe_int/master:SafeInt.hpp   > include/cpput/safe_int.h
	${MAKE} allheader

allheader:
	echo "#ifndef H_CPPUT_ALL" > include/cpput/all.h
	echo "#define H_CPPUT_ALL" >> include/cpput/all.h
	cd include/cpput; ls *.h | grep -v "all.h" | sed 's/^\(.*\)/#include "\1"/' >> all.h
	cd include/cpput; ls *.h.in | grep -v "all.h" | sed 's/\.in$$//g' | sed 's/^\(.*\)/#include "\1"/' >> all.h
	echo "#endif" >> include/cpput/all.h
	cat include/cpput/all.h

gitignore:
	echo "build" > .gitignore
	ls include/cpput/*.in | sed 's/\.in$$//g' >> .gitignore

.PHONY: build test 3rdparty
