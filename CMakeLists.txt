cmake_minimum_required(VERSION 2.8)

project(ecl)

set_property(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS FALSE)

if(NOT TOP_DIR)
    set(TOP_DIR ${CMAKE_CURRENT_SOURCE_DIR})
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${TOP_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${TOP_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${TOP_DIR}/lib)

set(WARNINGS "-Wall -Wextra -pedantic -Wswitch -Wnon-virtual-dtor")
set(SECURE   "-U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=2 -ftrapv -Wformat-security -fPIE")
set(CXXFLAGS "-std=c++11 -fPIC -fno-rtti -fno-exceptions -fvisibility=hidden ${SECURE} ${OPTIMIZATION} ${DEBUG}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXXFLAGS} ${WARNINGS} ${DEFINES}")

set(ECL_INCLUDE      ${TOP_DIR}/include)

set(ECL_TESTS_DIR    ${TOP_DIR}/tests)

include_directories(${ECL_INCLUDE})

get_filename_component(POSTFIX ${CMAKE_CXX_COMPILER} NAME)

add_executable(bitfield_${POSTFIX}                 ${ECL_TESTS_DIR}/test_bitfield.cpp          )
add_executable(cb_${POSTFIX}                       ${ECL_TESTS_DIR}/test_cb.cpp                )
add_executable(command_processor_${POSTFIX}        ${ECL_TESTS_DIR}/test_command_processor.cpp )
add_executable(fsm_${POSTFIX}                      ${ECL_TESTS_DIR}/test_fsm.cpp               )
add_executable(json_${POSTFIX}                     ${ECL_TESTS_DIR}/test_json.cpp              )
add_executable(scope_guard_${POSTFIX}              ${ECL_TESTS_DIR}/test_scope_guard.cpp       )
add_executable(singleton_${POSTFIX}                ${ECL_TESTS_DIR}/test_singleton.cpp         )
add_executable(static_map_${POSTFIX}               ${ECL_TESTS_DIR}/test_static_map.cpp        )
add_executable(str_const_${POSTFIX}                ${ECL_TESTS_DIR}/test_str_const.cpp         )
add_executable(stream_${POSTFIX}                   ${ECL_TESTS_DIR}/test_stream.cpp            )
add_executable(tree_${POSTFIX}                     ${ECL_TESTS_DIR}/test_tree.cpp              )
add_executable(list_${POSTFIX}                     ${ECL_TESTS_DIR}/test_list.cpp              )

set_target_properties(bitfield_${POSTFIX}          PROPERTIES LINK_FLAGS " -Wl,-z,relro,-z,now -Wl,-Map=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/bitfield_${POSTFIX}.map"          )
set_target_properties(cb_${POSTFIX}                PROPERTIES LINK_FLAGS " -Wl,-z,relro,-z,now -Wl,-Map=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cb_${POSTFIX}.map"                )
set_target_properties(command_processor_${POSTFIX} PROPERTIES LINK_FLAGS " -Wl,-z,relro,-z,now -Wl,-Map=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/command_processor_${POSTFIX}.map" )
set_target_properties(fsm_${POSTFIX}               PROPERTIES LINK_FLAGS " -Wl,-z,relro,-z,now -Wl,-Map=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/fsm_${POSTFIX}.map"               )
set_target_properties(json_${POSTFIX}              PROPERTIES LINK_FLAGS " -Wl,-z,relro,-z,now -Wl,-Map=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/json_${POSTFIX}.map"              )
set_target_properties(scope_guard_${POSTFIX}       PROPERTIES LINK_FLAGS " -Wl,-z,relro,-z,now -Wl,-Map=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/scope_guard_${POSTFIX}.map"       )
set_target_properties(singleton_${POSTFIX}         PROPERTIES LINK_FLAGS " -Wl,-z,relro,-z,now -Wl,-Map=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/singleton_${POSTFIX}.map"         )
set_target_properties(static_map_${POSTFIX}        PROPERTIES LINK_FLAGS " -Wl,-z,relro,-z,now -Wl,-Map=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/static_map_${POSTFIX}.map"        )
set_target_properties(str_const_${POSTFIX}         PROPERTIES LINK_FLAGS " -Wl,-z,relro,-z,now -Wl,-Map=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/str_const_${POSTFIX}.map"         )
set_target_properties(stream_${POSTFIX}            PROPERTIES LINK_FLAGS " -Wl,-z,relro,-z,now -Wl,-Map=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/stream_${POSTFIX}.map"            )
set_target_properties(tree_${POSTFIX}              PROPERTIES LINK_FLAGS " -Wl,-z,relro,-z,now -Wl,-Map=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tree_${POSTFIX}.map"              )
set_target_properties(list_${POSTFIX}              PROPERTIES LINK_FLAGS " -Wl,-z,relro,-z,now -Wl,-Map=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/list_${POSTFIX}.map"              )

list(APPEND MAP_FILES
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/bitfield_${POSTFIX}.map
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cb_${POSTFIX}.map
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/command_processor_${POSTFIX}.map
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/fsm_${POSTFIX}.map
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/json_${POSTFIX}.map
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/scope_guard_${POSTFIX}.map
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/singleton_${POSTFIX}.map
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/static_map_${POSTFIX}.map
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/str_const_${POSTFIX}.map
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/stream_${POSTFIX}.map
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tree_${POSTFIX}.map
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/list_${POSTFIX}.map
)

set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${MAP_FILES}")

add_custom_target(build-time-make-directory ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

find_package(Doxygen)

if (DOXYGEN_FOUND)

set(DOXYGEN_INPUT doxygen.config)
set(DOXYGEN_OUTPUT ${TOP_DIR}/doc)

add_custom_command(
  OUTPUT ${DOXYGEN_OUTPUT}
  COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation..."
  COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_INPUT}
  COMMAND ${CMAKE_COMMAND} -E echo "Done."
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  DEPENDS ${DOXYGEN_INPUT}
  )

add_custom_target(apidoc DEPENDS ${DOXYGEN_OUTPUT})

add_custom_target(apidoc_forced
  COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation..."
  COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_INPUT}
  COMMAND ${CMAKE_COMMAND} -E echo "Done."
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

endif (DOXYGEN_FOUND)
