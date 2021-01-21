file(GLOB SRCS *.h *.cc *.cpp)
add_library(ooasm ${SRCS})
set_target_properties(ooasm PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(ooasm PUBLIC ${CMAKE_CURRENT_LIST_DIR})
