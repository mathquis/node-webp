project(node-webp)
cmake_minimum_required(VERSION 3.9)
cmake_policy(SET CMP0042 NEW)
set (CMAKE_CXX_STANDARD 11)

add_subdirectory(libwebp)

execute_process(COMMAND node -p "require('path').dirname(require('@mapbox/node-pre-gyp').find(path.resolve(path.join(__dirname,'./package.json'))))"
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
		OUTPUT_VARIABLE OUTPUT_DIRECTORY
		)
string(REPLACE "\n" "" OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})
string(REPLACE "\"" "" OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})

message("Output path: ${OUTPUT_DIRECTORY}")

file(GLOB SOURCE_FILES
	"src/binding.cc"
)

include_directories(
	${CMAKE_JS_INC}
)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${CMAKE_JS_SRC})

set_target_properties(${PROJECT_NAME} PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}"
	LIBRARY_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}"
	RUNTIME_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}"
	LINK_FLAGS_RELEASE -s
)

target_link_libraries(
	${PROJECT_NAME}
	${CMAKE_JS_LIB}
	webp
)

target_include_directories(node-webp PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/libwebp/src/webp"
                          )

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")

# Include N-API wrappers
execute_process(COMMAND node -p "require('node-addon-api').include"
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
		OUTPUT_VARIABLE NODE_ADDON_API_DIR
		)
string(REPLACE "\n" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
string(REPLACE "\"" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${NODE_ADDON_API_DIR})
