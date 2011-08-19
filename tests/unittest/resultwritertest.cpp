/*
    ResultWriter classes tests
    Copyright (C) 2011  Rafael Durán Castañeda <rafadurancastaneda@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <gtest/gtest.h>

// ResultWriter includes
#include "resultwriter.h"
#include "blgajsonrw.h"

using ::testing::TestWithParam;
using ::testing::Values;


typedef ResultWriter* CreateRWFunc();

template <int rwNumber, bool tofile>
ResultWriter* CreateRW() {
  return  ResultWriter::getResultWriter(rwNumber, tofile, "test.json");
}


class RWTest : public TestWithParam<CreateRWFunc*> {
 public:
  virtual ~RWTest() { delete rw_; }
  virtual void SetUp() { 
    rw_ = (*GetParam())();
  }
  virtual void TearDown() {
    //I don't need deleting ff_ since ResultWriter already does
    rw_ = NULL;
  }

 protected:
  ResultWriter* rw_;
};


TEST_P(RWTest, Prueba) {
    EXPECT_EQ(0, 1);
}

// factory functions:
INSTANTIATE_TEST_CASE_P(
    BlgaJRW,
    RWTest,
    Values(&CreateRW<0, true>));
