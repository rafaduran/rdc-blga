cmake_minimum_required(VERSION 2.8.0)

PROJECT(main)

# uninstall target
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P
   ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

find_package(FLEX)

include_directories (./src/carlosFitnessFunctions) 
include_directories(./src/localSearchers)
include_directories(${FLEX_DIR})

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

SET(FLEX_DIR ${CMAKE_CURRENT_SOURCE_DIR}/flex)

SET(peak_number_dir ${FLEX_DIR}/peakNumber)

SET(pop_dis_dir ${FLEX_DIR}/popDistribution)

SET(pro_res_dir ${FLEX_DIR}/processResultFiles)

SET(FLEX_TARGETS PeakNumP1 PeakNumP2 PeakNumHump P1PopDis P2PopDis HumpPopDis
   ProcessToWxMaxima)
  
set(RANDOM ./src/carlosFitnessFunctions/Random.cpp)

FLEX_TARGET(PeakNumberP1Scan ${peak_number_dir}/PeakNumP1.flex
                             ${FLEX_DIR}/PeakNumP1.cpp)

FLEX_TARGET(PeakNumberP2Scan ${peak_number_dir}/PeakNumP2.flex 
                             ${FLEX_DIR}/PeakNumP2.cpp)

FLEX_TARGET(PeakNumberHumpScan ${peak_number_dir}/PeakNumHump.flex
                               ${FLEX_DIR}/PeakNumHump.cpp)

FLEX_TARGET(P1PopDisScan ${pop_dis_dir}/P1PopDis.flex 
                         ${FLEX_DIR}/P1PopDis.cpp)

FLEX_TARGET(P2PopDisScan ${pop_dis_dir}/P2PopDis.flex 
                         ${FLEX_DIR}/P2PopDis.cpp)

FLEX_TARGET(HumpPopDisScan ${pop_dis_dir}/HumpPopDis.flex 
                           ${FLEX_DIR}/HumpPopDis.cpp)

FLEX_TARGET(ProP1P2ResScan ${pro_res_dir}/ProcessP1P2Results.flex
                           ${FLEX_DIR}/ProcessP1P2Results.cpp)
            
FLEX_TARGET(ProHumpResScan ${pro_res_dir}/ProcessHumpResults.flex
                           ${FLEX_DIR}/ProcessHumpResults.cpp)

FLEX_TARGET(ProToMaximaScan ${pro_res_dir}/ProcessToWxMaxima.flex
                            ${FLEX_DIR}/ProcessToWxMaxima.cpp)

ADD_EXECUTABLE(PeakNumP1 ${FLEX_PeakNumberP1Scan_OUTPUTS})

ADD_EXECUTABLE(PeakNumP2 ${FLEX_PeakNumberP2Scan_OUTPUTS})

ADD_EXECUTABLE(PeakNumHump ${FLEX_PeakNumberHumpScan_OUTPUTS})

ADD_EXECUTABLE(P1PopDis ${FLEX_P1PopDisScan_OUTPUTS})

ADD_EXECUTABLE(P2PopDis ${FLEX_P2PopDisScan_OUTPUTS})

ADD_EXECUTABLE(HumpPopDis ${FLEX_HumpPopDisScan_OUTPUTS})

ADD_EXECUTABLE(ProcessP1P2Results ${FLEX_ProP1P2ResScan_OUTPUTS})

ADD_EXECUTABLE(ProcessHumpResults ${FLEX_ProHumpResScan_OUTPUTS})

ADD_EXECUTABLE(ProcessToWxMaxima ${FLEX_ProToMaximaScan_OUTPUTS})

ADD_EXECUTABLE(blga ${FITNESS} ${SEARCHER} blgaMain.cpp)

ADD_EXECUTABLE(peakGen ${RANDOM} 
   ./src/carlosFitnessFunctions/peakGen/peakGenMain.cpp)

INSTALL(TARGETS blga DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/executables)

INSTALL(TARGETS peakGen DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/executables)

INSTALL(TARGETS ${FLEX_TARGETS}
   DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/executables/flex)