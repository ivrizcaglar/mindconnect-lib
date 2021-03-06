#Set name of the test framework.
SET(TEST_FRAMEWORK unity_and_cmock)

#Set paths.
SET(MOCK_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/mock")
SET(TEST_CASE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
SET(TEST_RUNNER_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/testrunner")
SET(GENERATE_TEST_RUNNER_SCRIPT_PATH "mcl_core/test/lib/CMock/vendor/unity/auto/generate_test_runner.rb")
SET(TEST_TYPE "unit")

#Create mock and test runner directories.
FILE(MAKE_DIRECTORY ${MOCK_DIRECTORY})
FILE(MAKE_DIRECTORY ${TEST_RUNNER_DIRECTORY})

#Clean Mock Directory
FILE(GLOB MOCK_DIRECTORY_OLD_FILES "${MOCK_DIRECTORY}/*")
IF(MOCK_DIRECTORY_OLD_FILES)
	FILE(REMOVE ${MOCK_DIRECTORY_OLD_FILES})
ENDIF()

MACRO(MAKE_SUBDIRECTORY_LIST output_list parent_directory)
  FILE(GLOB items RELATIVE ${parent_directory} ${parent_directory}/*)
  SET(subdirectory_list "")
  FOREACH(item ${items})
    IF(IS_DIRECTORY ${parent_directory}/${item})
      LIST(APPEND subdirectory_list ${item})
    ENDIF()
  ENDFOREACH()
  SET(${output_list} ${subdirectory_list})
ENDMACRO()

#Create list of replaceable modules.
MAKE_SUBDIRECTORY_LIST(REPLACEABLE_MODULES "${MCL_CORE_CMAKE_ROOT_DIR}/src")

#Memory module is replaceable but must not be mocked.
SET(STANDARD_MEMORY_MODULE_PATH "${MCL_CORE_CMAKE_ROOT_DIR}/src/memory/standard")
SET(MEMORY_MODULE_INTERFACE_FILE "mcl_memory.h")
FILE(GLOB STANDARD_MEMORY_MODULE_SOURCE "${STANDARD_MEMORY_MODULE_PATH}/*.c") 

#Create mocks for headers in src folder.
SET(RUBY_CMD "ruby")
SET(RUBY_SCRIPT_PATH "mcl_core/test/lib/CMock/lib/cmock.rb")
SET(CMOCK_YML_FILE_INPUT "cmock.yml.in")
SET(CMOCK_YML_FILE "${CMAKE_CURRENT_BINARY_DIR}/cmock.yml")
SET(CMOCK_YML_INCLUDES "")
CONFIGURE_FILE(${CMOCK_YML_FILE_INPUT} ${CMOCK_YML_FILE})
FILE(GLOB HEADERS_TO_MOCK "${MCL_CORE_CMAKE_ROOT_DIR}/src/*.h")

#Remove utility headers from the list.
FOREACH(MCL_CORE_UTILITY_INTERNAL_HEADER ${MCL_CORE_UTILITY_INTERNAL_HEADERS})
	LIST(REMOVE_ITEM HEADERS_TO_MOCK ${MCL_CORE_UTILITY_INTERNAL_HEADER})
ENDFOREACH()

SET(MOCK_MCL_HTTP_CLIENT_H "${MCL_CORE_CMAKE_ROOT_DIR}/include/mcl_core/mcl_http_client.h")

EXECUTE_PROCESS(COMMAND ${RUBY_CMD} ${RUBY_SCRIPT_PATH} -o${CMOCK_YML_FILE} ${HEADERS_TO_MOCK}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    RESULT_VARIABLE ruby_result	
    OUTPUT_VARIABLE ruby_output)

#Configure yml and create mock for headers in include folder. The mocks header will also include the corresponding header in src folder (e.g. to include typedef/structs).
FILE(GLOB INCLUDE_HEADERS_TO_MOCK "${MCL_CORE_CMAKE_ROOT_DIR}/include/mcl_core/*.h")

#Remove utility headers from the list.
FOREACH(MCL_CORE_UTILITY_HEADER ${MCL_CORE_UTILITY_HEADERS})
	LIST(REMOVE_ITEM INCLUDE_HEADERS_TO_MOCK ${MCL_CORE_UTILITY_HEADER})
ENDFOREACH()

FOREACH(INCLUDE_HEADER_TO_MOCK ${INCLUDE_HEADERS_TO_MOCK})
    #only consider headers : include/mcl_core/mcl_*.h
    IF(INCLUDE_HEADER_TO_MOCK MATCHES "^.*mcl_[a-zA-Z_]*\\.h$")
		IF(INCLUDE_HEADER_TO_MOCK MATCHES "^.*${MEMORY_MODULE_INTERFACE_FILE}")
			CONTINUE()
		ENDIF()
        STRING(REGEX REPLACE "^(.*)mcl_" "" SOURCE_HEADER_TO_INCLUDE ${INCLUDE_HEADER_TO_MOCK})
        #if src/*.h doesnt exist dont include in mock-header
        IF(EXISTS "${MCL_CORE_CMAKE_ROOT_DIR}/src/${SOURCE_HEADER_TO_INCLUDE}")
            SET(CMOCK_YML_INCLUDES "[${SOURCE_HEADER_TO_INCLUDE}]")
        ELSE()
            SET(CMOCK_YML_INCLUDES "")
        ENDIF()
        #MESSAGE(STATUS "Creating mock for ${INCLUDE_HEADER_TO_MOCK} with include of ${SOURCE_HEADER_TO_INCLUDE}")
        CONFIGURE_FILE(${CMOCK_YML_FILE_INPUT} ${CMOCK_YML_FILE})
        EXECUTE_PROCESS(COMMAND ${RUBY_CMD} ${RUBY_SCRIPT_PATH} -o${CMOCK_YML_FILE} ${INCLUDE_HEADER_TO_MOCK}
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            RESULT_VARIABLE ruby_result	
            OUTPUT_VARIABLE ruby_output)
     ENDIF()
ENDFOREACH()

#Configure yml and create mock for http_client with type definition from libcurl implementation.
SET(CMOCK_YML_INCLUDES "[http_client_libcurl.h]")
CONFIGURE_FILE(${CMOCK_YML_FILE_INPUT} ${CMOCK_YML_FILE})
EXECUTE_PROCESS(COMMAND ${RUBY_CMD} ${RUBY_SCRIPT_PATH} -o${CMOCK_YML_FILE} ${MOCK_MCL_HTTP_CLIENT_H}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    RESULT_VARIABLE ruby_result	
    OUTPUT_VARIABLE ruby_output)
    
#Set test libraries
SET(TEST_LIBS ${MCL_CORE_LIBS} ${TEST_FRAMEWORK})

#Set mock sources
FILE(GLOB CORE_MOCK_SOURCES "${MOCK_DIRECTORY}/*.c")

#No dll.
ADD_DEFINITIONS(-DMCL_CORE_EXPORT=)

#Set mock library
SET(CORE_MOCK_LIB CORE_MOCK_SOURCES)
ADD_LIBRARY(${CORE_MOCK_LIB} ${CORE_MOCK_SOURCES})
TARGET_LINK_LIBRARIES(${CORE_MOCK_LIB} ${TEST_LIBS})
TARGET_INCLUDE_DIRECTORIES(${CORE_MOCK_LIB} PUBLIC ${MCL_CORE_INCLUDE_DIRECTORIES} ${MCL_CORE_CMAKE_ROOT_DIR}/include/mcl_core)

#PIC option is required in order to build CORE_MOCK_LIB with shared library target
SET_TARGET_PROPERTIES(${CORE_MOCK_LIB} PROPERTIES POSITION_INDEPENDENT_CODE ON)

#Append mock library to test libraries
LIST(APPEND TEST_LIBS ${CORE_MOCK_LIB})

#Loop over each unit test file.
FILE(GLOB UNIT_TEST_FILE_LIST RELATIVE "${TEST_CASE_DIRECTORY}" "${TEST_CASE_DIRECTORY}/*.c") 
FOREACH(UNIT_TEST_FILE ${UNIT_TEST_FILE_LIST})

    #Remove file extension from the testcase file
    STRING(REPLACE ".c" "" UNIT_TEST_FILE_NAME ${UNIT_TEST_FILE})
	
	#Remove "test_" from testcase filename
    STRING(REPLACE "test_" "" TESTCASE_NAME ${UNIT_TEST_FILE_NAME})
	
	SET(IS_REPLACEABLE FALSE)
	SET(IS_MEMORY_MODULE FALSE)
	
	FOREACH(REPLACEABLE_MODULE ${REPLACEABLE_MODULES})
		STRING(FIND ${TESTCASE_NAME} ${REPLACEABLE_MODULE} FIND_POSITION)
		IF(FIND_POSITION EQUAL 0)
			SET(IS_REPLACEABLE TRUE)
			IF(${REPLACEABLE_MODULE} EQUAL "memory")
				SET(IS_MEMORY_MODULE TRUE)
			ENDIF()
			STRING(REPLACE "${REPLACEABLE_MODULE}_" "" SELECTED_MODULE ${TESTCASE_NAME})
			SET(SELECTED_MODULE_SOURCE_PATH "${MCL_CORE_CMAKE_ROOT_DIR}/src/${REPLACEABLE_MODULE}/${SELECTED_MODULE}")
			BREAK()
		ENDIF()
	ENDFOREACH()
                 
    #Create test runner.
    SET(TEST_RUNNER_FILE "${UNIT_TEST_FILE_NAME}_runner.c")
    EXECUTE_PROCESS(COMMAND ${RUBY_CMD} ${GENERATE_TEST_RUNNER_SCRIPT_PATH} "${TEST_CASE_DIRECTORY}/${UNIT_TEST_FILE}" "${TEST_RUNNER_DIRECTORY}/${TEST_RUNNER_FILE}"
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        RESULT_VARIABLE ruby_result	
        OUTPUT_VARIABLE ruby_output)
    
    #Parse the include lines of the test file to 
    #determine which source file to compile, the original or the mock. 
    SET(ORIGINAL_SOURCES "")
	
    FILE(STRINGS "${TEST_CASE_DIRECTORY}/${UNIT_TEST_FILE}" INCLUDE_LINE_LIST REGEX "^#include*")
    FOREACH(INCLUDE_LINE ${INCLUDE_LINE_LIST})
        STRING(REPLACE "#include \"" "" INCLUDE_FILE_H_ ${INCLUDE_LINE})
        STRING(REGEX REPLACE "\".*" "" INCLUDE_FILE_H_ ${INCLUDE_FILE_H_}) 
        STRING(REGEX REPLACE ".*/" "" INCLUDE_FILE_H_ ${INCLUDE_FILE_H_})
        STRING(REPLACE ".h" ".c" INCLUDE_FILE_C_ ${INCLUDE_FILE_H_})
        STRING(REPLACE "mcl_" "" INCLUDE_FILE_C_ ${INCLUDE_FILE_C_})
        IF(EXISTS "${MCL_CORE_CMAKE_ROOT_DIR}/src/${INCLUDE_FILE_H_}")
            LIST(REMOVE_ITEM CORE_MOCK_SOURCES "${MOCK_DIRECTORY}/mock_${INCLUDE_FILE_C_}")
            IF(EXISTS "${MCL_CORE_CMAKE_ROOT_DIR}/src/${INCLUDE_FILE_C_}")
                LIST(APPEND ORIGINAL_SOURCES "${MCL_CORE_CMAKE_ROOT_DIR}/src/${INCLUDE_FILE_C_}")
            ENDIF() 
        ELSEIF(EXISTS "${MCL_CORE_CMAKE_ROOT_DIR}/include/mcl_core/${INCLUDE_FILE_H_}")
            LIST(REMOVE_ITEM CORE_MOCK_SOURCES "${MOCK_DIRECTORY}/mock_mcl_${INCLUDE_FILE_C_}")
            IF(EXISTS "${MCL_CORE_CMAKE_ROOT_DIR}/src/${INCLUDE_FILE_C_}")
                LIST(APPEND ORIGINAL_SOURCES "${MCL_CORE_CMAKE_ROOT_DIR}/src/${INCLUDE_FILE_C_}")
            ELSE()
                #Look for the source file in subfolders of standard replaceable modules.
                FOREACH(REPLACEABLE_MODULE ${REPLACEABLE_MODULES})
                    IF(EXISTS "${MCL_CORE_CMAKE_ROOT_DIR}/src/${REPLACEABLE_MODULE}/standard/${INCLUDE_FILE_C_}")
                        LIST(APPEND ORIGINAL_SOURCES "${MCL_CORE_CMAKE_ROOT_DIR}/src/${REPLACEABLE_MODULE}/standard/${INCLUDE_FILE_C_}")    
                        BREAK()
                    ENDIF()
                ENDFOREACH()
            ENDIF()        
        ENDIF()
    ENDFOREACH(INCLUDE_LINE) 
	
	IF(IS_REPLACEABLE)
		FILE(GLOB SELECTED_MODULE_SOURCE_FILES "${SELECTED_MODULE_SOURCE_PATH}/*.c") 
		LIST(APPEND ORIGINAL_SOURCES ${SELECTED_MODULE_SOURCE_FILES})
	ENDIF()
	
	IF(NOT IS_MEMORY_MODULE)
		LIST(APPEND ORIGINAL_SOURCES ${STANDARD_MEMORY_MODULE_SOURCE})
	ENDIF()
	
    #Set test source files.             
    SET(TEST_SOURCES ${TEST_CASE_DIRECTORY}/${UNIT_TEST_FILE} ${TEST_RUNNER_DIRECTORY}/${TEST_RUNNER_FILE})

    #Set the name of the test executable.   
    SET(UNIT_TEST_EXECUTABLE ${UNIT_TEST_FILE_NAME})
	
    #Create test executable.    
    ADD_EXECUTABLE(${UNIT_TEST_EXECUTABLE} $<TARGET_OBJECTS:cJSON> ${TEST_SOURCES} ${ORIGINAL_SOURCES})    
        
    #Link libraries to executable.
    TARGET_LINK_LIBRARIES(${UNIT_TEST_EXECUTABLE} ${TEST_LIBS})

    IF (POW_NEEDS_LIBM)
        TARGET_LINK_LIBRARIES(${UNIT_TEST_EXECUTABLE} m)
    ENDIF()

    #Include required directories
    TARGET_INCLUDE_DIRECTORIES(${UNIT_TEST_EXECUTABLE} PUBLIC ${MOCK_DIRECTORY})
    TARGET_INCLUDE_DIRECTORIES(${UNIT_TEST_EXECUTABLE} PUBLIC ${MCL_CORE_INCLUDE_DIRECTORIES})
	
	IF(IS_REPLACEABLE)
		TARGET_INCLUDE_DIRECTORIES(${UNIT_TEST_EXECUTABLE} PUBLIC ${SELECTED_MODULE_SOURCE_PATH})
	ENDIF()
	
	IF(NOT IS_MEMORY_MODULE)
		TARGET_INCLUDE_DIRECTORIES(${UNIT_TEST_EXECUTABLE} PUBLIC ${STANDARD_MEMORY_MODULE_PATH})
	ENDIF()
	
	SET_TARGET_PROPERTIES(${UNIT_TEST_EXECUTABLE} PROPERTIES
		FOLDER "${MCL_COMPONENT}/${TEST_TYPE}_tests")

    ADD_TEST(${UNIT_TEST_EXECUTABLE} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${UNIT_TEST_EXECUTABLE})
    
ENDFOREACH(UNIT_TEST_FILE) 
