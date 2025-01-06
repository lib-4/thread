cmake_minimum_required(VERSION 3.10)

project(G_4_thread VERSION 1.0 LANGUAGES C)

set(SOURCES
  src/thread.c
  src/mutex.c
  src/condition_variable.c
)

add_library(G_4_thread STATIC ${SOURCES})
set_target_properties(G_4_thread PROPERTIES
  OUTPUT_NAME "-4_thread"
  C_STANDARD 11
)

if (MSVC)
  set_target_properties(G_4_thread PROPERTIES COMPILE_FLAGS "/experimental:c11atomics")
endif()

target_include_directories(G_4_thread
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/external_include
)

install(TARGETS G_4_thread
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

install(
  FILES
    include/-4_thread.h
  DESTINATION
    include
)

install(
  FILES
    include/-4/thread_functions.h
    include/-4/thread_shortcut.h
    include/-4/thread_types.h
  DESTINATION
    include/-4
)
