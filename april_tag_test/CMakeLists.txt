project(TestAprilTag)

add_executable(apriltag_opencv_test apriltag_opencv_test.cpp)

target_link_libraries(apriltag_opencv_test ${OpenCV_LIBS} apriltag)
