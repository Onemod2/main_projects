cmake_minimum_required(VERSION 3.9)
# add_compile_options(-fsanitize=address)
# add_link_options(-fsanitize=address)
project(bounded_synch_associative_buffer)

find_package(Threads REQUIRED)
add_executable(synch_buffer main.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../producer_consumer_test)
target_link_libraries(synch_buffer Threads::Threads)
if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif(MSVC)
set_target_properties(synch_buffer PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  )