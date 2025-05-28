# CMakeLists.txt inside Myprojects/oopL
cmake_minimum_required(VERSION 3.10)
project(ATM_System)

# REMOVE OR COMMENT OUT THIS LINE:
# set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/../vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

# Set C++ Standard to C++17 for Crow
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find required packages
find_package(Threads REQUIRED)

# Find ASIO from Homebrew installation
set(ASIO_INCLUDE_DIR "/opt/homebrew/include")
if(NOT EXISTS "${ASIO_INCLUDE_DIR}/asio.hpp")
    message(FATAL_ERROR "ASIO not found in Homebrew. Please install it using: brew install asio")
endif()

# Include directories for your project files, Crow, and ASIO
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Crow/include)
include_directories(${ASIO_INCLUDE_DIR})

# Add executable
add_executable(atm_server 
    main.cpp 
    Account.cpp 
    BankDatabase.cpp 
    Screen.cpp 
    Keypad.cpp 
    Transaction.cpp 
    BalanceInquiry.cpp 
    Withdrawal.cpp 
    Deposit.cpp 
    DepositSlot.cpp
)

# Link libraries
target_link_libraries(atm_server PRIVATE Threads::Threads)

# Define ASIO_STANDALONE to use standalone ASIO (not Boost.ASIO)
target_compile_definitions(atm_server PRIVATE ASIO_STANDALONE)

# --- Optional: For serving static files ---
file(GLOB_RECURSE STATIC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/public/*")
#add_custom_command(TARGET atm_web POST_BUILD
   # COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/public" "$<TARGET_FILE_DIR:atm_web>/public"
#)

# --- For serving static files ---
# Use standard Unix 'cp -R' command for more robust directory copying
# --- For serving static files ---
# Copy the HTML file to the build output directory
add_custom_command(TARGET atm_server POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:atm_server>"
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/atm_interface.html" "$<TARGET_FILE_DIR:atm_server>/"
    COMMENT "Copying static web file to build directory"
)