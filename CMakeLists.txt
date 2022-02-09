cmake_minimum_required(VERSION 3.0)
project(Chat-based-on-MQTT-protocol C)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set (CMAKE_C_STANDARD 99)

include_directories(include)

add_executable(CHAT app/main.c)
add_executable(PUB app/mqtt_pub.c)
add_executable(SUB app/mqtt_sub.c)

target_link_libraries(CHAT
  pthread
  mosquitto
)

target_link_libraries(PUB
  pthread
  mosquitto
)

target_link_libraries(SUB
  pthread
  mosquitto
)