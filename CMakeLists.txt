cmake_minimum_required(VERSION 3.24)
project(isaac_course_project VERSION 0.1.0 LANGUAGES CXX)

include(CTest)
include(FetchContent)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_SANITIZERS "Enable address and undefined-behavior sanitizers" OFF)
option(ISAAC_FETCH_SFML "Fetch pinned SFML when it is not installed" ON)

if(ENABLE_SANITIZERS AND CMAKE_CXX_COMPILER_ID MATCHES "AppleClang|Clang|GNU")
  add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
  add_link_options(-fsanitize=address,undefined)
endif()

find_package(SFML 3 COMPONENTS Graphics Window System Audio QUIET)
if(NOT SFML_FOUND AND ISAAC_FETCH_SFML)
  set(SFML_BUILD_AUDIO ON CACHE BOOL "" FORCE)
  set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
  set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
  set(SFML_BUILD_TEST_SUITE OFF CACHE BOOL "" FORCE)
  FetchContent_Declare(SFML
    URL https://github.com/SFML/SFML/archive/refs/tags/3.0.1.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
  FetchContent_MakeAvailable(SFML)
endif()

add_library(isaac_common INTERFACE)
target_include_directories(isaac_common INTERFACE src)

add_library(isaac_model_interface INTERFACE)
target_link_libraries(isaac_model_interface INTERFACE isaac_common)

add_library(isaac_model STATIC
  src/model/BossSystem.cpp
  src/model/CharacterCatalog.cpp
  src/model/Components.cpp
  src/model/EnemySystem.cpp
  src/model/GameSession.cpp
  src/model/ItemSystem.cpp
  src/model/Level.cpp
  src/model/Player.cpp)
target_link_libraries(isaac_model PUBLIC isaac_model_interface)
target_include_directories(isaac_model PUBLIC src)

add_library(isaac_viewmodel STATIC src/viewmodel/GameViewModel.cpp)
target_link_libraries(isaac_viewmodel PUBLIC isaac_model_interface)
target_include_directories(isaac_viewmodel PUBLIC src)

add_library(isaac_resource STATIC src/resource/ResourceManager.cpp)
target_link_libraries(isaac_resource PUBLIC SFML::Graphics SFML::Audio)
target_include_directories(isaac_resource PUBLIC src)

add_library(isaac_view STATIC src/view/GameView.cpp)
target_link_libraries(isaac_view PUBLIC isaac_common isaac_resource SFML::Graphics SFML::Audio)
target_include_directories(isaac_view PUBLIC src)

add_executable(isaac_course_game src/app/main.cpp src/app/Application.cpp)
target_link_libraries(isaac_course_game PRIVATE isaac_view isaac_viewmodel isaac_model isaac_resource)

add_custom_target(architecture_check
  COMMAND ${CMAKE_COMMAND} -DPROJECT_ROOT=${CMAKE_SOURCE_DIR}
          -P ${CMAKE_SOURCE_DIR}/tests/architecture/check_architecture.cmake
  COMMENT "Checking strict MVVM include boundaries")

if(BUILD_TESTING)
  add_executable(isaac_model_tests tests/model/test_main.cpp)
  target_link_libraries(isaac_model_tests PRIVATE isaac_model)
  add_test(NAME model_regression_suite COMMAND isaac_model_tests)
  add_executable(isaac_flow_tests tests/viewmodel/test_flow.cpp)
  target_link_libraries(isaac_flow_tests PRIVATE isaac_viewmodel isaac_model)
  add_test(NAME viewmodel_course_flow COMMAND isaac_flow_tests)
  add_executable(isaac_viewmodel_seam_tests tests/viewmodel/test_model_seam.cpp)
  target_link_libraries(isaac_viewmodel_seam_tests PRIVATE isaac_viewmodel)
  add_test(NAME viewmodel_model_seam COMMAND isaac_viewmodel_seam_tests)
  add_executable(isaac_course_demo tests/model/course_demo.cpp)
  target_link_libraries(isaac_course_demo PRIVATE isaac_viewmodel isaac_model)
  add_test(NAME deterministic_course_demo COMMAND isaac_course_demo)
  add_test(NAME architecture_boundaries
    COMMAND ${CMAKE_COMMAND} -DPROJECT_ROOT=${CMAKE_SOURCE_DIR}
            -P ${CMAKE_SOURCE_DIR}/tests/architecture/check_architecture.cmake)
  add_executable(isaac_resource_tests tests/resource/test_resource.cpp)
  target_link_libraries(isaac_resource_tests PRIVATE isaac_resource)
  target_compile_definitions(isaac_resource_tests PRIVATE ISAAC_SOURCE_DIR="${CMAKE_SOURCE_DIR}")
  add_test(NAME resource_cache_and_missing COMMAND isaac_resource_tests)
  add_test(NAME asset_provenance
    COMMAND ${CMAKE_COMMAND} -DPROJECT_ROOT=${CMAKE_SOURCE_DIR}
            -P ${CMAKE_SOURCE_DIR}/tests/assets/check_assets.cmake)
endif()
