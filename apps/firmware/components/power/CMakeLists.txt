# Power component CMakeLists.txt
idf_component_register(SRCS "src/power.c"
                     INCLUDE_DIRS "include"
                     REQUIRES firmware_hal)