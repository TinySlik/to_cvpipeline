cmake_minimum_required(VERSION 3.1)

project( hello )

add_subdirectory(../asyncplusplus)
INCLUDE_DIRECTORIES(../asyncplusplus/include)

link_libraries(Async++)

# subdir_depends(../to_pipeline_params_server)
set(LIBHELLO_SRC hello.cpp)

add_executable(main ${LIBHELLO_SRC})
# target_link_libraries(main paramserv)



