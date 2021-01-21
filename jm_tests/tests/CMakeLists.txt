file(GLOB TEST_SRCS *.cc *.cpp)
foreach (test_file IN ITEMS ${TEST_SRCS})
    get_filename_component(test_name ${test_file} NAME)
    set(test_target ${test_name}_target)
    add_executable(${test_target} ${test_file})
    target_link_libraries(${test_target} ooasm)
    add_test(${test_name} ${test_target})
endforeach ()

file(GLOB DNC_TEST_SRCS dnc/*.cc dnc/*.cpp)
foreach (test_file IN ITEMS ${DNC_TEST_SRCS})
    get_filename_component(test_name ${test_file} NAME)
    set(test_target ${test_name}_target)
    add_executable(${test_target} ${test_file})
    set_target_properties(${test_target} PROPERTIES
        EXCLUDE_FROM_ALL TRUE
        EXCLUDE_FROM_DEFAULT_BUILD TRUE)
    target_link_libraries(${test_target} ooasm)
    add_test(NAME ${test_name}
         COMMAND ${CMAKE_COMMAND} --build . --target ${test_target} --config $<CONFIGURATION>
         WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
    set_tests_properties(${test_name} PROPERTIES WILL_FAIL TRUE)
endforeach ()
