# Communication component CMakeLists.txt
idf_component_register(SRCS "src/communication.c"
                     INCLUDE_DIRS "include"
                     REQUIRES firmware_hal common_types json)