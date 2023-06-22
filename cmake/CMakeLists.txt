if (ANDROID)
    # See the following for detailed information:
    # https://gitlab.kitware.com/cmake/cmake/-/issues/21772
    # https://gitlab.kitware.com/cmake/cmake/-/merge_requests/8170
    cmake_minimum_required(VERSION 3.26)
else()
    cmake_minimum_required(VERSION 3.9)
endif()

project(Verovio)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if (APPLE)
    set(CMAKE_MACOSX_RPATH ON)
endif (APPLE)

option(VRV_DYNAMIC_CAST         "Enable <dynamic_cast>"                        OFF)
option(NO_DARMS_SUPPORT         "Disable DARMS support"                        ON)
option(NO_PAE_SUPPORT           "Disable Plaine & Easie support"               OFF)
option(NO_ABC_SUPPORT           "Disable ABC support"                          OFF)
option(NO_MUSICXML_SUPPORT      "Disable MusicXML support"                     OFF)
option(NO_MXL_SUPPORT           "Disable compressed MusicXML support"          OFF)
option(NO_HUMDRUM_SUPPORT       "Disable Humdrum support"                      OFF)
option(MUSICXML_DEFAULT_HUMDRUM "Enable MusicXML to Humdrum by default"        OFF)
option(NO_RUNTIME               "Disable runtime clock support"                ON)
option(BUILD_AS_LIBRARY         "Build Verovio as library"                     OFF)
option(BUILD_AS_RUST_LIBRARY    "Build Verovio as library for Rust"            OFF)
option(RUST_LIBRARY_STATIC      "Build Verovio as static library for Rust"     OFF)
option(USE_PAE_OLD_PARSER       "Use old PAE parser"                           OFF)

if (NO_HUMDRUM_SUPPORT AND MUSICXML_DEFAULT_HUMDRUM)
    message(SEND_ERROR "Default MusicXML to Humdrum cannot be enabled by default without Humdrum support")
endif()

# Sets the compile-time value for the RESOURCE_DIR macro in the source code.
add_compile_definitions(RESOURCE_DIR="${CMAKE_INSTALL_PREFIX}/share/verovio")

set(VEROVIO_INCLUDE_DIRECTORIES
    "${CMAKE_CURRENT_SOURCE_DIR}/../include"
    "${CMAKE_CURRENT_SOURCE_DIR}/../include/crc"
    "${CMAKE_CURRENT_SOURCE_DIR}/../include/midi"
    "${CMAKE_CURRENT_SOURCE_DIR}/../include/hum"
    "${CMAKE_CURRENT_SOURCE_DIR}/../include/json"
    "${CMAKE_CURRENT_SOURCE_DIR}/../include/pugi"
    "${CMAKE_CURRENT_SOURCE_DIR}/../include/zip"
    "${CMAKE_CURRENT_SOURCE_DIR}/../include/vrv"
    "${CMAKE_CURRENT_SOURCE_DIR}/../libmei/dist"
    "${CMAKE_CURRENT_SOURCE_DIR}/../libmei/addons"
)

if (ANDROID)
    find_library(log-lib log)
    link_libraries(${log-lib})
endif()

if(MSVC)
    add_definitions(/W2)
    add_definitions(/wd4244)          # suppress warning of possible loss of precision
    list(APPEND VEROVIO_MACRO_DEFINITIONS -DNO_PAE_SUPPORT) # regex is working differently under Windows so PAE is not supported (yet)
    list(APPEND VEROVIO_MACRO_DEFINITIONS -DUSE_PAE_OLD_PARSER)
    add_definitions(/std:c++17)
    list(APPEND VEROVIO_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/../include/win32")
else()
    if(CMAKE_BUILD_TYPE MATCHES Debug)
        message(STATUS "Enabling debug information")
        add_definitions(-g)
        list(APPEND VEROVIO_MACRO_DEFINITIONS -DDEBUG)
    endif()

    # jsonxx raises -Wdollar-in-identifier-extension
    # gcc 8.3.1 does not like -Wdollar-in-identifier-extension option.
    add_definitions(-Wall -W -pedantic -Wno-unused-parameter -Wno-dollar-in-identifier-extension)
    add_definitions(-std=c++17)

    # extra warnings similar to Xcode compiling settings (most probably covered by -Wall):
    # https://github.com/llvm-mirror/clang/blob/master/include/clang/Basic/DiagnosticGroups.td
    # non-gcc: add_definitions(-Wblock-capture-autoreleasing)
    # non-gcc: add_definitions(-Wbool-conversion)
    # non-gcc: add_definitions(-Wconstant-conversion)
    add_definitions(-Wdeprecated-declarations)
    add_definitions(-Wempty-body)
    # non-gcc: add_definitions(-Wenum-conversion)
    # non-gcc: add_definitions(-Winfinite-recursion)
    # non-gcc (ObjC only): add_definitions(-Wint-conversion)
    add_definitions(-Winvalid-offsetof)
    # non-gcc: add_definitions(-Wmove)
    # non-gcc: add_definitions(-Wno-c++11-extensions)
    # non-gcc: add_definitions(-Wno-comma)
    add_definitions(-Wno-conversion)
    # non-gcc: add_definitions(-Wno-exit-time-destructors)
    add_definitions(-Wno-float-conversion)
    # non-gcc: add_definitions(-Wno-four-char-constants)
    add_definitions(-Wno-missing-braces)
    add_definitions(-Wno-missing-field-initializers)
    # non-gcc (ObjC only): add_definitions(-Wno-missing-prototypes)
    # non-gcc: add_definitions(-Wno-newline-eof)
    # non-gcc: add_definitions(-Wno-non-virtual-dtor)
    add_definitions(-Wno-overloaded-virtual)
    # non-gcc: add_definitions(-Wno-semicolon-before-method-body)
    add_definitions(-Wno-shadow)
    add_definitions(-Wno-sign-conversion)
    add_definitions(-Wno-trigraphs)
    add_definitions(-Wno-unknown-pragmas)
    add_definitions(-Wno-unused-label)
    # non-gcc: add_definitions(-Wnon-literal-null-conversion)
    # non-gcc: add_definitions(-Wobjc-literal-conversion)
    add_definitions(-Wparentheses)
    # non-gcc: add_definitions(-Wrange-loop-analysis)
    # non-gcc: add_definitions(-Wshorten-64-to-32)
    # non-gcc (ObjC only: add_definitions(-Wstrict-prototypes)
    add_definitions(-Wswitch)
    add_definitions(-Wuninitialized)
    add_definitions(-Wunreachable-code)
    add_definitions(-Wunused-function)
    add_definitions(-Wunused-value)
    add_definitions(-Wunused-variable)

endif()

include_directories(${VEROVIO_INCLUDE_DIRECTORIES})

execute_process(
    COMMAND ../tools/get_git_commit.sh
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    OUTPUT_VARIABLE GIT_COMMIT
)

if(VRV_DYNAMIC_CAST)
    list(APPEND VEROVIO_MACRO_DEFINITIONS -DVRV_DYNAMIC_CAST)
endif()

if(NO_DARMS_SUPPORT)
    list(APPEND VEROVIO_MACRO_DEFINITIONS -DNO_DARMS_SUPPORT)
endif()

if(NO_PAE_SUPPORT)
    list(APPEND VEROVIO_MACRO_DEFINITIONS -DNO_PAE_SUPPORT)
elseif(USE_PAE_OLD_PARSER)
    list(APPEND VEROVIO_MACRO_DEFINITIONS -DUSE_PAE_OLD_PARSER)
endif()

if(NO_MUSICXML_SUPPORT)
    list(APPEND VEROVIO_MACRO_DEFINITIONS -DNO_MUSICXML_SUPPORT)
endif()

if(NO_MXL_SUPPORT)
    list(APPEND VEROVIO_MACRO_DEFINITIONS -DNO_MXL_SUPPORT)
endif()

if(NO_HUMDRUM_SUPPORT)
    list(APPEND VEROVIO_MACRO_DEFINITIONS -DNO_HUMDRUM_SUPPORT)
else()
    file(GLOB hum_SRC "../src/hum/*.cpp")
    if (MUSICXML_DEFAULT_HUMDRUM)
        list(APPEND VEROVIO_MACRO_DEFINITIONS -DMUSICXML_DEFAULT_HUMDRUM)
    endif()
endif()

if(NO_RUNTIME)
    list(APPEND VEROVIO_MACRO_DEFINITIONS -DNO_RUNTIME)
endif()

add_definitions(${VEROVIO_MACRO_DEFINITIONS})

file(GLOB verovio_SRC "../src/*.cpp")
file(GLOB libmei_dist_SRC "../libmei/dist/*.cpp")
file(GLOB libmei_addons_SRC "../libmei/addons/*.cpp")
file(GLOB midi_SRC "../src/midi/*.cpp")
file(GLOB crc_SRC "../src/crc/*.cpp")

# Add header files to custom target, otherwise they are not shown in some IDEs (e.g. QtCreator)
file(GLOB_RECURSE LibFiles "../include/*.h")
add_custom_target(headers SOURCES ${LibFiles})

set(all_SRC
    ${verovio_SRC}
    ${libmei_dist_SRC}
    ${libmei_addons_SRC}
    ${hum_SRC}
    ${crc_SRC}
    ${midi_SRC}
    ../src/json/jsonxx.cc
    ../src/pugi/pugixml.cpp
)

##################
# Shared library #
##################

if (BUILD_AS_RUST_LIBRARY)
    if (RUST_LIBRARY_STATIC)
        message(STATUS "***** Building Verovio as static library for Rust *****")
    else()
        message(STATUS "***** Building Verovio as shared library for Rust *****")
    endif()

    if(NOT CMAKE_BUILD_TYPE MATCHES Debug)
        include(CheckIPOSupported)
        check_ipo_supported(RESULT IPO_SUPPORTED OUTPUT IPO_ERROR)
        if (NOT IPO_SUPPORTED) 
            message(FATAL_ERROR "IPO is not IPO_SUPPORTED: ${IPO_ERROR}")
        endif()
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
    endif()

    if (WIN32)
        set(AUTOCXX_GEN_ANNOTATIONS "__declspec(dllexport)")
    else()
        set(AUTOCXX_GEN_ANNOTATIONS "__attribute__((visibility(\"default\")))")
    endif()

    # Increase the following as the binding is developed
    set(AUTOCXX_NUM_GENERATED_FILES 10)
    set(AUTOCXX_CXX_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/autocxx-generated/cxx")
    set(AUTOCXX_RUST_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/autocxx-generated/rust")
    set(AUTOCXX_RUST_WRAPPER "${CMAKE_CURRENT_SOURCE_DIR}/../tools/rust_wrapper.rs")
    set(AUTOCXX_GENERATED_RUST_FILES "${AUTOCXX_RUST_OUTPUT_DIR}/ghost")

    math(EXPR AUTOCXX_GENERATED_FILES_RANGE_STOP "${AUTOCXX_NUM_GENERATED_FILES} - 1")
    foreach (idx RANGE ${AUTOCXX_GENERATED_FILES_RANGE_STOP})
        list(
            APPEND
            AUTOCXX_GENERATED_CXX_FILES
            "${AUTOCXX_CXX_OUTPUT_DIR}/gen${idx}.cpp"
        )
    endforeach()

    foreach (inc_dir ${VEROVIO_INCLUDE_DIRECTORIES})
        list(
            APPEND
            AUTOCXX_INC_ARGUMENTS
            --inc ${inc_dir}
        )
        file(GLOB inc_dir_headers "${inc_dir}/*.h")
        list(
            APPEND
            AUTOCXX_TARGET_HEADERS
            ${inc_dir_headers}
        )
    endforeach()

    if (RUST_LIBRARY_STATIC)
        set(
            AUTOCXX_CXX_ARGUMENTS
            --cpp-extension cpp
            --gen-cpp
            --generate-cxx-h
            --generate-exact ${AUTOCXX_NUM_GENERATED_FILES}
        )
    else()
        set(
            AUTOCXX_CXX_ARGUMENTS
            --cpp-extension cpp
            --gen-cpp
            --generate-cxx-h
            --cxx-impl-annotations ${AUTOCXX_GEN_ANNOTATIONS}
            --generate-exact ${AUTOCXX_NUM_GENERATED_FILES}
        )
    endif()
    set(
        AUTOCXX_RUST_ARGUMENTS
        --gen-rs-include
    )

    function(invoke_autocxx LANGUAGE)
        add_custom_command(
            OUTPUT ${AUTOCXX_GENERATED_${LANGUAGE}_FILES}
            COMMAND 
                ${CMAKE_COMMAND} ARGS -E env
                # Unset $ENV{TARGET} for iOS
                $<IF:$<OR:$<STREQUAL:$ENV{TARGET},aarch64-apple-ios>,$<STREQUAL:$ENV{TARGET},aarch64-apple-ios-sim>,$<STREQUAL:$ENV{TARGET},x86_64-apple-ios>,$<STREQUAL:$ENV{TARGET},x86_64-apple-ios-sim>>,--unset=TARGET,DUMMY=DUMMY>
                autocxx-gen
                --outdir ${AUTOCXX_${LANGUAGE}_OUTPUT_DIR}
                ${AUTOCXX_INC_ARGUMENTS}
                ${AUTOCXX_${LANGUAGE}_ARGUMENTS}
                ${AUTOCXX_RUST_WRAPPER}
                --
                -std=c++17
                ${VEROVIO_MACRO_DEFINITIONS}
                -DRUST_LIBRARY
            DEPENDS
                ${AUTOCXX_RUST_WRAPPER}
                ${AUTOCXX_TARGET_HEADERS}
            VERBATIM
        )
    endfunction()

    invoke_autocxx(CXX)
    invoke_autocxx(RUST)

    add_custom_target(verovio-rs-generated DEPENDS ${AUTOCXX_GENERATED_RUST_FILES})
    add_definitions(-DRUST_LIBRARY)
    if (RUST_LIBRARY_STATIC)
        add_library(verovio ${all_SRC} ${AUTOCXX_GENERATED_CXX_FILES})
    else()
        add_library(verovio SHARED ${all_SRC} ${AUTOCXX_GENERATED_CXX_FILES})
    endif()
    add_dependencies(verovio verovio-rs-generated)

elseif (BUILD_AS_LIBRARY)
    message(STATUS "***** Building Verovio as shared library *****")
    add_library(verovio SHARED ../tools/c_wrapper.cpp ${all_SRC})

###########
# WASM BC #
###########

elseif (BUILD_AS_WASM)
    if (EMSCRIPTEN_VERSION VERSION_LESS "3.1.27")
        message(FATAL_ERROR "You need em++ 3.1.27 or greater")
    endif()
    message(STATUS "***** Building Verovio as WASM BC *****")
    add_library(verovio STATIC ../tools/c_wrapper.cpp ${all_SRC})

#################
# Python module #
#################

elseif (BUILD_AS_PYTHON)
    message(STATUS "***** Building Verovio as Python module *****")

    # clear out policies
    cmake_policy(SET CMP0078 NEW)
    cmake_policy(SET CMP0086 NEW)

    find_package(SWIG REQUIRED)
    include(${SWIG_USE_FILE})
    set(CMAKE_SWIG_FLAGS "")

    set(PYTHON_VERSION "3" CACHE STRING "Python Version to use")
    find_package(Python EXACT ${PYTHON_VERSION} MODULE REQUIRED)

    if(${SWIG_VERSION} VERSION_GREATER_EQUAL "4.1.0")
        list(APPEND CMAKE_SWIG_FLAGS "-doxygen")
    else()
        list(APPEND CMAKE_SWIG_FLAGS "-py3;-DPY3")
    endif()

    set_source_files_properties(../bindings/python/verovio.i PROPERTIES CPLUSPLUS ON)

    # needed for static build on linux
    add_definitions(-fPIC)

    ### For building for found version of Python (comment when specific version is set)
    find_package(Python COMPONENTS Interpreter Development)
    include_directories(${Python_INCLUDE_DIRS})
    message(STATUS "***** Building for Python ${Python_VERSION} *****")
    string(REPLACE "." "_" OUTPUT_VERSION_STRING ${Python_VERSION})
    swig_add_library(verovio_module_${OUTPUT_VERSION_STRING} LANGUAGE python TYPE MODULE SOURCES ../bindings/python/verovio.i)
    swig_link_libraries(verovio_module_${OUTPUT_VERSION_STRING} verovio ${Python_LIBRARIES})

    add_library(verovio STATIC ../tools/c_wrapper.cpp ${all_SRC})

#####################
# Command-line tool #
#####################

else()
    message(STATUS "***** Building Verovio as command-line tool *****")
    add_executable(verovio ../tools/main.cpp ${all_SRC})

endif()

install(
    TARGETS verovio DESTINATION bin
)
install(
    DIRECTORY ../data/
    DESTINATION share/verovio
    FILES_MATCHING PATTERN "*.xml" PATTERN "*.svg" PATTERN "*.css"
)
