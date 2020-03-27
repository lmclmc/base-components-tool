cmake_minimum_required(VERSION 3.2)

aux_source_directory(./thread/ BASE_FILES)
aux_source_directory(./threadpool/ BASE_FILES)
aux_source_directory(./timer/ BASE_FILES)
aux_source_directory(./yqueue/ BASE_FILES)
aux_source_directory(./single/ BASE_FILES)

add_library(core SHARED ${BASE_FILES})

target_link_libraries(core pthread)
