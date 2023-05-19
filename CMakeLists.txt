cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(Simulation)

find_package(Geant4 REQUIRED ui_all vis_all)

include(${Geant4_USE_FILE})

file(GLOB sources ${PROJECT_SOURCE_DIR}/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/*.hh)

add_executable(gen_Hyperon gen_Hyperon.cc ${sources} ${headers})
target_link_libraries(gen_Hyperon ${Geant4_LIBRARIES})

add_custom_target(Simulation DEPENDS gen_Hyperon)
