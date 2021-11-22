add_executable(testomp OMPThreadTest.cpp)

target_link_libraries(testomp ${GTEST_BOTH_LIBRARIES})

add_executable(compareompfoxtools CompareOMPToFoxTools.cpp)
setTestProperties(compareompfoxtools ${FOX_LIBRARY})
target_link_libraries(compareompfoxtools ${GTEST_BOTH_LIBRARIES})