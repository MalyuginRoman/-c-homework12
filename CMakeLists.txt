cmake_minimum_required(VERSION 3.7.2)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(mapreduce VERSION ${PROJECT_VERSION})

configure_file(version.h.in version.h)

add_executable(mapreduce_cli
    main.cpp
    block_reader.cpp
    file_splitter.cpp
    min_prefix_function.cpp)
add_library(mapreduce lib.cpp)

set_target_properties(mapreduce_cli mapreduce PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
target_include_directories(mapreduce
    PRIVATE "${CMAKE_BINARY_DIR}"
)

target_link_libraries(mapreduce_cli PRIVATE
    mapreduce
)

if (MSVC)
    target_compile_options(mapreduce_cli PRIVATE
        /W4
    )
    target_compile_options(mapreduce PRIVATE
        /W4
    )
else ()
    target_compile_options(mapreduce_cli PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(mapreduce PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

install(TARGETS mapreduce RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)
