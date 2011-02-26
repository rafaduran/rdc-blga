###############################################################################
#    CMakeLists.txt for CMake project rdc-blga                                #
#    Copyright (C) 2011  rafadurancastaneda@gmail.com                         #
#                                                                             #
#    This program is free software: you can redistribute it and/or modify     #
#    it under the terms of the GNU General Public License as published by     #
#    the Free Software Foundation, either version 3 of the License, or        #
#    (at your option) any later version.                                      #
#                                                                             #
#    This program is distributed in the hope that it will be useful,          #
#    but WITHOUT ANY WARRANTY; without even the implied warranty of           #
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            #
#    GNU General Public License for more details.                             #
#                                                                             #
#    You should have received a copy of the GNU General Public License        #
#   along with this program.  If not, see <http://www.gnu.org/licenses/>.     #
###############################################################################


cmake_minimum_required(VERSION 2.8.0)

PROJECT(rdc-blga)

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

SET(PEAK_NUMBER_DIR ${FLEX_DIR}/peakNumber)

SET(POP_DIS_DIR ${FLEX_DIR}/popDistribution)

SET(PRO_RES_DIR ${FLEX_DIR}/processResultFiles)

SET(FLEX_TARGETS PeakCounter P1PopDis P2PopDis HumpPopDis ProcessToWxMaxima)
  
set(RANDOM ./src/carlosFitnessFunctions/Random.cpp)

FLEX_TARGET(PeakCounterScan ${PEAK_NUMBER_DIR}/PeakCounter.flex
                            ${FLEX_DIR}/PeakCounter.cpp)

FLEX_TARGET(P1PopDisScan ${POP_DIS_DIR}/P1PopDis.flex 
                         ${FLEX_DIR}/P1PopDis.cpp)

FLEX_TARGET(P2PopDisScan ${POP_DIS_DIR}/P2PopDis.flex 
                         ${FLEX_DIR}/P2PopDis.cpp)

FLEX_TARGET(HumpPopDisScan ${POP_DIS_DIR}/HumpPopDis.flex 
                           ${FLEX_DIR}/HumpPopDis.cpp)

FLEX_TARGET(ProP1P2ResScan ${PRO_RES_DIR}/ProcessP1P2Results.flex
                           ${FLEX_DIR}/ProcessP1P2Results.cpp)
            
FLEX_TARGET(ProHumpResScan ${PRO_RES_DIR}/ProcessHumpResults.flex
                           ${FLEX_DIR}/ProcessHumpResults.cpp)

FLEX_TARGET(ProToMaximaScan ${PRO_RES_DIR}/ProcessToWxMaxima.flex
                            ${FLEX_DIR}/ProcessToWxMaxima.cpp)

ADD_EXECUTABLE(PeakCounter ${FLEX_PeakCounterScan_OUTPUTS})

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