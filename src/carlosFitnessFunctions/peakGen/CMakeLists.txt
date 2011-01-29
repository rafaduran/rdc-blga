PROJECT(peakgen)

include_directories (../main/src/carlosFitnessFunctions)

set(RANDOM ../main/src/carlosFitnessFunctions/Random.cpp)

ADD_EXECUTABLE(peakgen ${RANDOM} main.cpp)

