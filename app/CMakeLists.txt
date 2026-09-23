# include sub directories
include_directories(${PROJECT_SOURCE_DIR}/include/base)
include_directories(${PROJECT_SOURCE_DIR}/include/core)
include_directories(${PROJECT_SOURCE_DIR}/include/config)
include_directories(${PROJECT_SOURCE_DIR}/include/log)
include_directories(${PROJECT_SOURCE_DIR}/include/network)
include_directories(${PROJECT_SOURCE_DIR}/include/utility)
include_directories(${PROJECT_SOURCE_DIR}/include/thirdparty)
include_directories(${PROJECT_SOURCE_DIR}/include/thirdparty/rapidjson)
include_directories(${PROJECT_SOURCE_DIR}/include/thirdparty/mosquitto)

# add compile tree
add_subdirectory(config)
add_subdirectory(log)
add_subdirectory(network)
add_subdirectory(utility)

# all source files
file(GLOB_RECURSE ALL_SOURCE "*.cpp" "*.c")

# find_library(CURL
#   NAMES curl
#   PATHS ${PROJECT_SOURCE_DIR}/lib
# )
# set(ARM OFF) 

if(${ARM})
   link_directories(${PROJECT_SOURCE_DIR}/lib/arm/libcurl)
   link_directories(${PROJECT_SOURCE_DIR}/lib/arm/openssl)
elseif(${APPLE})
   link_directories(${PROJECT_SOURCE_DIR}/lib/macos/libcurl)
   link_directories(${PROJECT_SOURCE_DIR}/lib/macos/openssl)
   link_directories(${PROJECT_SOURCE_DIR}/lib/macos/mosquitto)
else()
   link_directories(${PROJECT_SOURCE_DIR}/lib/linux/libcurl)
   link_directories(${PROJECT_SOURCE_DIR}/lib/linux/openssl)
endif()


# create the executable
set(EXECUTABLE_NAME OtaUpgrader)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/output)

# execute file 
add_executable(${EXECUTABLE_NAME} ${ALL_SOURCE})

target_link_libraries(${EXECUTABLE_NAME} pthread)
target_link_libraries(${EXECUTABLE_NAME} curl)
target_link_libraries(${EXECUTABLE_NAME} ssl)
target_link_libraries(${EXECUTABLE_NAME} crypto)
if(NOT APPLE)
    target_link_libraries(${EXECUTABLE_NAME} rt)
endif()
target_link_libraries(${EXECUTABLE_NAME} mosquitto)
