#file(
# GLOB_RECURSE FINALPROJECT_INC CONFIGURE_DEPENDS
# RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
# *.h *.hpp
#)

#file(
# GLOB_RECURSE FINALPROJECT_SRC CONFIGURE_DEPENDS
# RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
# *.c *.cpp
#)

# Hello Eric Winebrenner, the above globbing include files that shouldn't be included in certain build systems, it broke on Cooper's machine
# - Gus
set(FINALPROJECT_SRC main.cpp)
set(FINALPROJECT_INC )

#Copies this final project's asset folder to bin when it is built
add_custom_target(copyAssetsFP ALL COMMAND ${CMAKE_COMMAND} -E copy_directory
${CMAKE_CURRENT_SOURCE_DIR}/assets/
${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/)

install(FILES ${FINALPROJECT_INC} DESTINATION include/finalproject)
add_executable(finalproject ${FINALPROJECT_SRC} ${FINALPROJECT_INC})
target_link_libraries(finalproject PUBLIC core IMGUI glm)
target_include_directories(finalproject PUBLIC ${CORE_INC_DIR} ${stb_INCLUDE_DIR})

#Trigger asset copy when finalproject is built
add_dependencies(finalproject copyAssetsFP)