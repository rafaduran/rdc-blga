/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  <copyright holder> <email>

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
#include "FitnessFunction.h"

#define SIZE P1_P2_dimension

using ::testing::TestWithParam;
using ::testing::Values;


typedef FitnessFunction* CreateFitnessFunc();


template <int functionNumber>
FitnessFunction* CreateFitness() {
  return  FitnessFunction::getFitnessFunction(functionNumber);
}


class FitnessTest : public TestWithParam<CreateFitnessFunc*> {
 public:
  virtual ~FitnessTest() { delete ff_; }
  virtual void SetUp() { 
    ff_ = (*GetParam())(); 
    ind1 = new char[SIZE];
    ind2 = new char[SIZE];
    for(int i = 0; i < SIZE; i++){
      ind1[i] = 0;
      ind2[i] = 1;
    }
  }
  virtual void TearDown() {
    //I don't need deleting ff_ since FitnessFunction already does
    ff_ = NULL;
    delete ind1;
    delete ind2;
  }

 protected:
  FitnessFunction* ff_;
  char *ind1;
  char *ind2;
  
};

TEST_P(FitnessTest, GetDim) {
  
  if(strcmp(ff_->getName().str().data(), "Hump function")) {
    EXPECT_EQ(ff_->getDim(), P1_P2_dimension);  // Not Hump function
  }
  else {
    // Hump function
    EXPECT_EQ(ff_->getDim(), ff_->getNvariables() * P1_P2_dimension);
  }
}

TEST_P(FitnessTest, FitnessCompare) {
  ASSERT_LT(ff_->compare(0,1), 0);
  ASSERT_GT(ff_->compare(1,0), 0);
  ASSERT_EQ(ff_->compare(1,1), 0);
}

// factory functions:
INSTANTIATE_TEST_CASE_P(
    P1_2_3,
    FitnessTest,
    Values(&CreateFitness<0>, &CreateFitness<1>, &CreateFitness<2>));
