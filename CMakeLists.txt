cmake_minimum_required(VERSION 3.0)

project(SN_LOCALIZATION)

find_package(ROOT 6.16 CONFIG REQUIRED)

add_executable(readKin src/readKin.cpp)
target_link_libraries(readKin PUBLIC ROOT::Hist PUBLIC ROOT::Tree)

