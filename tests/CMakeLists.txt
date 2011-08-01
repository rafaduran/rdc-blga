###############################################################################
#    CMakeLists.txt used by CMake project blga-tests                          #
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

PROJECT(blga-tests)

include_directories (./gtest-1.6.0/include
                     ./unittest
                     ../src/carlosFitnessFunctions)

ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/gtest-1.6.0)

SET(FITNESS ../src/carlosFitnessFunctions/PeriodicEqual.cpp
            ../src/carlosFitnessFunctions/FitnessFunction.cpp
            ../src/carlosFitnessFunctions/HumpFunction.cpp
            ../src/carlosFitnessFunctions/PeriodicUnequal.cpp
   )

ADD_EXECUTABLE(blga-tests ${FITNESS} ./unittest/fitnesstest.cpp)

target_link_libraries(blga-tests gtest_main gtest)

ADD_TEST(unittest blga-tests)
