cmake_minimum_required(VERSION 3.10)
project(rotTest)

set(CMAKE_CXX_STANDARD 11)

configure_file(images/SlimeArcherSpriteSheet.png images/SlimeArcherSpriteSheet.png COPYONLY)
configure_file(images/World1.png images/World1.png COPYONLY)
configure_file(images/Arrow.png images/Arrow.png COPYONLY)
configure_file(images/Inventory.png images/Inventory.png COPYONLY)
configure_file(images/Stats.png images/Stats.png COPYONLY)
configure_file(images/Crystal_Crawler.png images/Crystal_Crawler.png COPYONLY)
configure_file(images/Oswald-Bold.ttf images/Oswald-Bold.ttf COPYONLY)
configure_file(images/gold.png images/gold.png COPYONLY)
configure_file(images/dBowShot.png images/dBowShot.png COPYONLY)

# Define sources and executable
set(EXECUTABLE_NAME "rotTest")

add_executable(${EXECUTABLE_NAME} main.cpp Projectile.cpp Projectile.h Character.cpp Character.h Enemy.cpp Enemy.h)

# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
#Find any version 2.X of SFML
#See the FindSFML.cmake file for additional details and instructions
find_package(SFML 2 REQUIRED network audio graphics window system)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endif()