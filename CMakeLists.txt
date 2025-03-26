# Link with the "Threads library (required to link with AFF3CT after)
set(CMAKE_THREAD_PREFER_PTHREAD ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)
set(PYBIND11_NEWPYTHON ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

include_directories(${CMAKE_SOURCE_DIR}/lib/include)
find_package(pybind11 CONFIG REQUIRED )

find_package(Threads REQUIRED)
find_package(cpptrace REQUIRED)
find_package(streampu REQUIRED)
#find_package(pystreampu REQUIRED)

file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/pybind11/)
pybind11_add_module(ads_b MODULE ${SOURCES})

set_target_properties(ads_b PROPERTIES
                      OUTPUT_NAME ads_b
                      POSITION_INDEPENDENT_CODE ON) # set -fpic

target_link_libraries     (ads_b PRIVATE spu::spu-static-lib)
target_include_directories(ads_b PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

