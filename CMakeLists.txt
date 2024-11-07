# Minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Project name
project(LinkChat)

# Specify C standard
set(CMAKE_C_STANDARD 11)

# Source files
set(SOURCES
        main.c
        menu.c
        p2p.c
        server.c
        login.c
        networkmanager.c
        button.c
        textbox.c
        textfield.c
        host.c
        host.h
        message_queue.c
        message_queue.h
        join.c
        join.h
        encrypter.c
        encrypter.h
        base58.c
        base58.c
        base58.h
        client_list_view.c
        client_list_view.h
        client_list_view.c   # Add base64.c here
)

# Include directories
include_directories(include)

# FetchContent module for automatically downloading dependencies
include(FetchContent)

# Fetch Raylib
FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.0  # Specify the version or branch of Raylib you want
)

# Fetch tiny-AES-c
FetchContent_Declare(
        tiny-aes-c
        GIT_REPOSITORY https://github.com/kokke/tiny-AES-c.git
        GIT_TAG master
)

# Build dependencies if not already available
FetchContent_MakeAvailable(raylib tiny-aes-c)

# Platform-specific settings
if (WIN32)
    # Windows settings
    set(PLATFORM_LIBS ws2_32)  # For socket programming on Windows
else()
    # Linux settings
    set(PLATFORM_LIBS GL m pthread dl rt X11)
endif()

# Add executable
add_executable(LinkChat ${SOURCES})

# Include directories for dependencies
target_include_directories(LinkChat PRIVATE
        ${raylib_SOURCE_DIR}/src
        ${tiny-aes-c_SOURCE_DIR}
)

# Add the tiny-AES-c source files
target_sources(LinkChat PRIVATE
        ${tiny-aes-c_SOURCE_DIR}/aes.c
)

# Link Raylib and platform-specific libraries
target_link_libraries(LinkChat raylib ${PLATFORM_LIBS})

# Compiler flags
target_compile_options(LinkChat PRIVATE -Wall -Wextra -pedantic)

# Clean target
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "LinkChat")
