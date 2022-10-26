
add_executable(dataset_3f2n ${CMAKE_CURRENT_SOURCE_DIR}/dataset_3f2n.cpp)
add_executable(dataset_exr ${CMAKE_CURRENT_SOURCE_DIR}/dataset_exr.cpp)


find_package(TFTN REQUIRED)
include_directories(${TFTN_INCLUDE_DIRS})


target_link_libraries(dataset_3f2n
        ${OpenCV_LIBS}
        ${EXRIO_LIBS}
        ${TFTN_LIBS}
        -lIlmImf        #OpenEXR用的
        )

target_link_libraries(dataset_exr
        ${OpenCV_LIBS}
        ${EXRIO_LIBS}
        ${TFTN_LIBS}
        -lIlmImf        #OpenEXR用的
        )