add_executable(testomp OMPThreadTest.cpp)
target_link_libraries(testomp ${GTEST_BOTH_LIBRARIES})