set(MAIN_TARGET_NAME ${PROJECT_NAME}Main)
set(srcPath ../src/)
set(incPath ../headers/)

add_executable( ${MAIN_TARGET_NAME} 
    "main.cpp" "main.h"
    # ${srcPath}Some.cpp ${incPath}Some.h
)

target_include_directories(${MAIN_TARGET_NAME} PRIVATE srcPath incPath 
    # ${SOME_INCLUDE_DIRS}
)

target_link_libraries(${MAIN_TARGET_NAME} 
    # ${SOME_LIBRARIES}
)