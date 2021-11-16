set(utils_omp_STAT_SRCS
   OMPThreading.h
   OMPThreading.cpp
)

add_library(utils_omp STATIC ${utils_omp_STAT_SRCS})
set_property(TARGET utils_omp PROPERTY PROJECT_LABEL "z_utils_omp")
