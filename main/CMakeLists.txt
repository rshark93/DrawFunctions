set(MAIN_TARGET_NAME ${PROJECT_NAME}Main)
set(srcPath ../src/)
set(incPath ../headers/)

set(SDL2_LIBS ../../../SDL2-2.0.16/lib/x64)
set(SDL_TTF_LIBS ../../../SDL2_ttf-2.20.0/lib/x64)

if(WIN32)
    set(SDL2_LIBRARIES ${SDL2_LIBS}/SDL2 ${SDL2_LIBS}/SDL2main)
    set(SDL_TTF_LIBRARIES ${SDL_TTF_LIBS}/SDL2_ttf)    
endif()

find_file(SDL2_INCLUDE_DIRS NAME SDL.h HINTS SDL2)
find_library(SDL2_LIBRARY NAME SDL2)

find_file(SDL_TTF_INCLUDE_DIRS NAME SDL_ttf.h HINTS SDL_TTF)
find_library(SDL_TTF_LIBRARY NAME SDL_TTF)

add_executable(${MAIN_TARGET_NAME} "main.cpp" "main.h"
    ${srcPath}plot_graph.cpp ${incPath}plot_graph.h
    ${srcPath}linked_list.cpp ${incPath}linked_list.h
    ${srcPath}calculate_function.cpp ${incPath}calculate_function.h
)

target_include_directories(${MAIN_TARGET_NAME} PRIVATE ${srcPath} ${incPath} 
    ${SDL2_INCLUDE_DIRS} ${SDL_TTF_INCLUDE_DIRS}
)

target_link_libraries(${MAIN_TARGET_NAME} ${SDL2_LIBRARIES} ${SDL_TTF_LIBRARIES})

message("-- Linking SDL")
add_library(SDL2 ${SDL2_LIBS}/SDL2.dll)
add_library(SDL2_ttf ${SDL_TTF_LIBS}/SDL2_ttf.dll)
set_target_properties(SDL2 SDL2_ttf PROPERTIES LINKER_LANGUAGE CPP)