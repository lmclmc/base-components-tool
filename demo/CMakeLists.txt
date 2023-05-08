cmake_minimum_required(VERSION 3.2)

add_executable(threadpool ./threadpool.cpp)
add_executable(timer ./timer.cpp)
add_executable(cmdline ./cmdline.cpp)

target_link_libraries(threadpool lmcbase)
target_link_libraries(timer lmcbase)
target_link_libraries(cmdline lmcbase)

install(FILES ${PROJECT_BINARY_DIR}/demo/threadpool PERMISSIONS OWNER_EXECUTE 
              OWNER_READ OWNER_WRITE GROUP_READ GROUP_WRITE WORLD_READ 
              WORLD_WRITE DESTINATION 
              ${PROJECT_BINARY_DIR}/install/demo/)
install(FILES ${PROJECT_BINARY_DIR}/demo/timer PERMISSIONS OWNER_EXECUTE 
              OWNER_READ OWNER_WRITE GROUP_READ GROUP_WRITE WORLD_READ 
              WORLD_WRITE DESTINATION ${PROJECT_BINARY_DIR}/install/demo/)
install(FILES ${PROJECT_BINARY_DIR}/demo/cmdline PERMISSIONS OWNER_EXECUTE 
              OWNER_READ OWNER_WRITE GROUP_READ GROUP_WRITE WORLD_READ 
              WORLD_WRITE DESTINATION 
              ${PROJECT_BINARY_DIR}/install/demo/)