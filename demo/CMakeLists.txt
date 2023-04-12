cmake_minimum_required(VERSION 3.2)

aux_source_directory(./threadpool/ THREADPOOL_FILES)
aux_source_directory(./timer/ TIMER_FILES)

add_executable(threadpool ${THREADPOOL_FILES})
add_executable(timer ${TIMER_FILES})

target_link_libraries(threadpool lmcbase)
target_link_libraries(timer lmcbase)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/demo/threadpool DESTINATION ${PROJECT_BINARY_DIR}/install/demo/)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/demo/timer DESTINATION ${PROJECT_BINARY_DIR}/install/demo/)
install(DIRECTORY ${PROJECT_BINARY_DIR}/demo/threadpool DESTINATION ${PROJECT_BINARY_DIR}/install/demo/threadpool/)
install(DIRECTORY ${PROJECT_BINARY_DIR}/demo/timer DESTINATION ${PROJECT_BINARY_DIR}/install/demo/timer/)
