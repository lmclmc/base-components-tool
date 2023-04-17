cmake_minimum_required(VERSION 3.2)

aux_source_directory(./thread/ BASE_FILES)
aux_source_directory(./threadpool/ BASE_FILES)
aux_source_directory(./timer/ BASE_FILES)
aux_source_directory(./yqueue/ BASE_FILES)
aux_source_directory(./single/ BASE_FILES)
aux_source_directory(./cmdline/ BASE_FILES)
aux_source_directory(./log/ BASE_FILES)

add_library(lmcbase SHARED ${BASE_FILES})

target_link_libraries(lmcbase pthread)

install(DIRECTORY ${PROJECT_BINARY_DIR}/src/liblmcbase.so DESTINATION 
                  ${PROJECT_BINARY_DIR}/install/lib/)
install(DIRECTORY single/ DESTINATION 
                  ${PROJECT_BINARY_DIR}/install/include/single/)
install(FILES log/log.h DESTINATION 
              ${PROJECT_BINARY_DIR}/install/include/log/)
install(FILES thread/lthread.h DESTINATION 
              ${PROJECT_BINARY_DIR}/install/include/thread/)
install(FILES threadpool/workqueue.h DESTINATION 
              ${PROJECT_BINARY_DIR}/install/include/threadpool/)
install(FILES timer/ltimer.h DESTINATION 
              ${PROJECT_BINARY_DIR}/install/include/timer/)
install(FILES cmdline/cmdline.h DESTINATION 
              ${PROJECT_BINARY_DIR}/install/include/cmdline/)
install(FILES type/type.h DESTINATION 
              ${PROJECT_BINARY_DIR}/install/include/type/)