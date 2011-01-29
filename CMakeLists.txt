cmake_minimum_required(VERSION 2.8.0)

PROJECT(main)

include_directories (./src/carlosFitnessFunctions) 
include_directories(./src/localSearchers)

set(SEARCHER ./src/localSearchers/Blga.cpp
             ./src/localSearchers/BlgaClearing.cpp
             ./src/localSearchers/BlgaClustering.cpp
             ./src/localSearchers/BlgaSharing.cpp
             ./src/localSearchers/ClusteredClearing.cpp
             ./src/localSearchers/LocalSearcher.cpp
  )

set(FITNESS ./src/carlosFitnessFunctions/FitnessFunction.cpp
            ./src/carlosFitnessFunctions/FitnessStopC.cpp
            ./src/carlosFitnessFunctions/HumpFunction.cpp
            ./src/carlosFitnessFunctions/NFEsStopC.cpp
            ./src/carlosFitnessFunctions/NRestartsSC.cpp
            ./src/carlosFitnessFunctions/PeriodicEqual.cpp
            ./src/carlosFitnessFunctions/PeriodicUnequal.cpp
            ./src/carlosFitnessFunctions/Random.cpp
            ./src/carlosFitnessFunctions/StopCondition.cpp         
  )
  
set(RANDOM ./src/carlosFitnessFunctions/Random.cpp)

ADD_EXECUTABLE(main ${FITNESS} ${SEARCHER} blgaMain.cpp)

ADD_EXECUTABLE(peakgen ${RANDOM} ./src/carlosFitnessFunctions/peakGen/peakGenMain.cpp)
