# Ensure the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Define the plugin's name
project(zmq_multi_plugin)

# Check if we are building with VCPKG or Conan, or without package managers
if(BUILDING_WITH_VCPKG)
    message(STATUS "Finding ZeroMQ with vcpkg")
elseif(BUILDING_WITH_CONAN)
    message(STATUS "Finding ZeroMQ with conan")
else()
    message(STATUS "Finding ZeroMQ without package managers")
    set(ZeroMQ_LIBS ${ZeroMQ_LIBRARIES})
endif()

# Try to find ZeroMQ
find_package(ZeroMQ QUIET)

if(ZeroMQ_FOUND)
    message(STATUS "[ZeroMQ] found")

    # Define Qt plugin
    add_definitions(${QT_DEFINITIONS})
    add_definitions(-DQT_PLUGIN)

    # Process UI file
    QT5_WRAP_UI(UI_SRC zmq_multi.ui)

    # Add the plugin's source files
    add_library(zmq_multi SHARED zmq_multi.cpp ${UI_SRC})

    # Link the plugin with Qt and PlotJuggler base libraries
    target_link_libraries(zmq_multi
        ${Qt5Widgets_LIBRARIES}  # Qt Widgets
        plotjuggler_base         # Link with PlotJuggler base libraries
    )

    # Link ZeroMQ depending on the package manager or custom setup
    if(BUILDING_WITH_VCPKG OR BUILDING_WITH_CONAN)
        target_link_libraries(zmq_multi libzmq-static)  # Link with static ZeroMQ library
    else()
        target_link_libraries(zmq_multi ${ZeroMQ_LIBRARIES})  # Link with system-installed ZeroMQ libraries
    endif()

    # Install the plugin
    install(TARGETS zmq_multi DESTINATION ${PJ_PLUGIN_INSTALL_DIRECTORY})

else()
    message(STATUS "[ZeroMQ] not found. Skipping plugin zmq_multi.")
endif()

