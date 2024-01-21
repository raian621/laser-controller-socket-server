cmake_minimum_required(VERSION 3.27)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(controller-server)

include(CTest)
include(FetchContent)

include_directories(include)

if(BUILD_TESTING)
  add_executable(test-controller)
  add_subdirectory(tests)
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        f8d7d77 # v1.14.0
  )
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)
  target_link_libraries(
    test-controller PRIVATE
    GTest::gtest_main
  )
  if(!MOCK_PIGPIO)
    target_link_libraries(
      test-controller PRIVATE
      pigpio
    )
  endif()
  target_compile_definitions(test-controller PRIVATE MOCK_PIGPIO)
  include(GoogleTest)
  gtest_discover_tests(test-controller)
else()
  add_executable(${PROJECT_NAME})
  if(MOCK_PIGPIO)
    target_compile_definitions(${PROJECT_NAME} PRIVATE MOCK_PIGPIO)
  else()
    FetchContent_Declare(
      pigpio
      GIT_REPOSITORY https://github.com/joan2937/pigpio
      GIT_TAG        c33738a
    )
    FetchContent_MakeAvailable(pigpio)
    include_directories(build/_deps/pigpio-src)
    
    target_link_libraries(
      ${PROJECT_NAME} PRIVATE
      pigpio
      pthread
    )
  endif()
endif()


add_subdirectory(src)
