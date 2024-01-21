cmake_minimum_required(VERSION 3.27)

if(BUILD_TESTING)
  target_sources(
    test-controller PRIVATE
    server.cpp
    controller.cpp
    commands.cpp
  )
else()
  target_sources(
    ${PROJECT_NAME} PRIVATE
    main.cpp
    server.cpp
    controller.cpp
    commands.cpp
  )
endif()