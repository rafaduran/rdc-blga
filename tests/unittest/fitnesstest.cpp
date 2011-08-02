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
#include "fitnesstest.h"
#include <math.h>

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
    _ind1 = new char[SIZE];
    _ind2 = new char[SIZE];
  }
  virtual void TearDown() {
    //I don't need deleting ff_ since FitnessFunction already does
    ff_ = NULL;
    delete _ind1;
    delete _ind2;
  }

 protected:
  FitnessFunction* ff_;
  const char *aux;
  char *_ind1;
  char *_ind2;
};

TEST_P(FitnessTest, GetDim) {
  
  if(ff_->getFunctionNumber() != 2) {
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

TEST_P(FitnessTest, FunctionNumber){
  switch(ff_->getFunctionNumber()){
    case 0:
      ASSERT_STREQ(ff_->getName().str().data(), "Periodic equal");
      break;
    case 1:
      ASSERT_STREQ(ff_->getName().str().data(), "Periodic unequal");
      break;
    case 2:
      ASSERT_STREQ(ff_->getName().str().data(), "Hump function");
      break;
  }
}

TEST_P(FitnessTest, Inversegray){
  /* Test values can be obtained from online gray/binary converter at:
     http://www.strw.leidenuniv.nl/~mathar/progs/gray.html */
  char *binary = new char[SIZE];
  for(int j = 0; j < 3; j++){
    for(int i = 0; i < SIZE; i++ ){
      switch(j){
        case 0:
          _ind1[i] = 1;             // Gray   111111111111111111111111111111 
          _ind2[i] = 1 - (i % 2);   // Binary 101010101010101010101010101010
          break;
        case 1:
          _ind1[i] = 0;   // Gray   000000000000000000000000000000
          _ind2[i] = 0;   // Binary 000000000000000000000000000000
          break;
        case 2:
          _ind1[i] = 1;             // Gray   110111101111111101110111100011 
          _ind2[i] = 0;             // Binary 100101001010101001011010111101
          break;
      }
    }
    if(j == 2){
      _ind1[2] = _ind1[7] = _ind1[16] = _ind1[20] = _ind1[25] = _ind1[26] = \
        _ind1[27] = 0; 
      _ind2[0] = _ind2[3] = _ind2[5] = _ind2[8] = _ind2[10] = _ind2[12] =\
        _ind2[14] = _ind2[17] = _ind2[19] = _ind2[20] = _ind2[22] = _ind2[24] =\
        _ind2[25] = _ind2[26] = _ind2[27] = _ind2[29] = 1;
    }

    if(ff_->getFunctionNumber()!=2){
      ff_->inverseGray(_ind1, binary); // 
      for(int i = 0; i < SIZE; i++ )
        ASSERT_EQ(int(binary[0]), int(_ind2[0]));
    } // TODO: test inverseGrayVector from Hump function   
  }
  delete binary;
}

/*TEST_P(FitnessTest, Distance) {
  ASSERT_EQ(ff_->distance(ff_->inverseGray(_ind1),
    ff_->inverseGray(_ind2)), );
  ASSERT_FALSE(ff_->distance(_ind1,_ind1));
  ASSERT_FALSE(ff_->distance(_ind2,_ind2));
  for(int i = 0; i < SIZE/2; i++)
    _ind1[i] = 1;
  ASSERT_NEAR(ff_->distance(_ind1, _ind2), 0.5, 1 / pow(2,SIZE-1));
}*/

// factory functions:
INSTANTIATE_TEST_CASE_P(
    P1_2_3,
    FitnessTest,
    Values(&CreateFitness<0>, &CreateFitness<1>, &CreateFitness<2>));
