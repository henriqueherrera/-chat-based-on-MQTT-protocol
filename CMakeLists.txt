cmake_minimum_required(VERSION 3.0)
project(Chat-based-on-MQTT-protocol C)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set (CMAKE_C_STANDARD 99)

add_library(sources
  src/terminal.c
  src/mqtt-client.c
  src/global.c
)

include_directories(include)

add_executable(Chat app/chat.c)

target_link_libraries(Chat
  sources
  pthread
  paho-mqtt3as
)