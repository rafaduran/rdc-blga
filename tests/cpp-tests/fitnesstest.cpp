/*
    FitnessFunction classes tests
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
#include <cmath>

#include <gtest/gtest.h>

// Fitness includes
#include "Random.h"
#include "FitnessFunction.h"


using ::testing::TestWithParam;
using ::testing::Values;


typedef FitnessFunction* CreateFitnessFunc();

template <int functionNumber>
FitnessFunction* CreateFitness() {
  return  FitnessFunction::getFitnessFunction(functionNumber,
		  "/home/rdc/projects/rdc-blga/tests/");
}


class FitnessTest : public TestWithParam<CreateFitnessFunc*> {
 public:
  virtual ~FitnessTest() { delete ff_; }
  virtual void SetUp() { 
    ff_ = (*GetParam())();
    dim_ = ff_->getDim();
    ind1_ = new char[dim_];
    ind2_ = new char[dim_];
    error_ = 1 / pow(2, (dim_ / ff_->getNvariables()) -1);
  }
  virtual void TearDown() {
    //I don't need deleting ff_ since FitnessFunction already does
    ff_ = NULL;
    delete ind1_;
    delete ind2_;
  }

 protected:
  FitnessFunction* ff_;
  const char *aux;
  char *ind1_;
  char *ind2_;
  int dim_;
  double error_;
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
  char *binary = new char[dim_];
  for(int j = 0; j < 3; j++){
    for(int i = 0; i < dim_; i++ ){
      switch(j){
        case 0:
          ind1_[i] = 1;             // Gray   111111111111111111111111111111 
          ind2_[i] = 1 - (i % 2);   // Binary 101010101010101010101010101010
          break;
        case 1:
          ind1_[i] = 0;   // Gray   000000000000000000000000000000
          ind2_[i] = 0;   // Binary 000000000000000000000000000000
          break;
        case 2:
          ind1_[i] = 1;             // Gray   110111101111111101110111100011 
          ind2_[i] = 0;             // Binary 100101001010101001011010111101
          break;
      }
    }
    if(j == 2){
      ind1_[2] = ind1_[7] = ind1_[16] = ind1_[20] = ind1_[25] = ind1_[26] = \
        ind1_[27] = 0; 
      ind2_[0] = ind2_[3] = ind2_[5] = ind2_[8] = ind2_[10] = ind2_[12] =\
        ind2_[14] = ind2_[17] = ind2_[19] = ind2_[20] = ind2_[22] = ind2_[24] =\
        ind2_[25] = ind2_[26] = ind2_[27] = ind2_[29] = 1;
      
      if(ff_->getFunctionNumber() == 2){
        int var_dim = dim_ / ff_->getNvariables();
        for(int i = 1; i < (var_dim / ff_->getNvariables())- 1; i++ ){
          for(int k = i * var_dim; k < (i+1) * var_dim; k++ ){
            if( i < ceil(ff_->getNvariables() / 2)){
              ind2_[k] = 0;
              ind1_[k] = 0;
            } else {
              ind1_[k] = 1;
              ind2_[k] = 1 - (k % 2);
            }
          }
        }
      }
    }
    
    if(ff_->getFunctionNumber()!=2){
      ff_->inverseGray(ind1_, binary);
      for(int i = 0; i < dim_; i++ )
        ASSERT_EQ(int(binary[i]), int(ind2_[i]));
    } else {
      ff_->inverseGrayVector(ind1_, binary);
      for(int i = 0; i < dim_; i++ )
        ASSERT_EQ(int(binary[i]), int(ind2_[i]));
    }
  }
  delete binary;
}


TEST_P(FitnessTest, BinaryToDouble){
  //TODO: refactoring must be done here after issue #14 is done
  for(int i = 0; i < 3; i++ ){
    for(int j = 0; j < dim_; j++){
      switch(i){
        case 0:
          ind1_[j] = 0;
          break;
        case 1:
          ind1_[j] = 1;
          break;
        case 2:
          ind1_[j] = 1; //random->Randint(0,1);
          break;
      }
    }
    
    switch(i) {
      case 0:
        if(ff_->getFunctionNumber() != 2) {
          ASSERT_NEAR(ff_->binaryToDouble(ind1_),0.0, 1 / pow(2,dim_-1));
        } else {
          int nvariables = ff_->getNvariables();
          double *vector = new double[nvariables];
          ff_->binaryToDoubleVector(ind1_, vector);
          for(int k = 0; k < nvariables; k++){
            ASSERT_NEAR(vector[k], 0.0, 1 / pow(2,dim_-1));
          }
          delete vector;
        }
        break;
      case 1:
        if(ff_->getFunctionNumber() != 2) {
          ASSERT_NEAR(ff_->binaryToDouble(ind1_),1.0, 1 / pow(2,dim_-1));
        } else {
          int nvariables = ff_->getNvariables();
          double *vector = new double[nvariables];
          ff_->binaryToDoubleVector(ind1_, vector);
          for(int k = 0; k < nvariables; k++){
            ASSERT_NEAR(vector[k], 1.0, 1 / pow(2,dim_-1));
          }
          delete vector;
        }
        break;
      case 2:
        if(ff_->getFunctionNumber() != 2) {
          ind1_[0] = 0;
          ASSERT_NEAR(ff_->binaryToDouble(ind1_),0.5, 1 / pow(2,dim_-1));
        } else {
          int nvariables = ff_->getNvariables();
          for(int k = 0; k < nvariables; k++)
            ind1_[k* dim_ / nvariables] = 0;
          double *vector = new double[nvariables];
          ff_->binaryToDoubleVector(ind1_, vector);
          for(int k = 0; k < nvariables; k++){
            ASSERT_NEAR(vector[k], 0.5, 1 / pow(2, (dim_ / nvariables) -1));
          }
          delete vector;
        }
        break;  
    }
  }
}


TEST_P(FitnessTest, Distance) {
  for(int i = 0; i < dim_; i++){
    ind1_[i] = 0;
    ind2_[i] = 0;
  }
  
  ASSERT_EQ(ff_->distance(ind1_, ind2_), 0);
  Random *rdom = new Random();
  rdom->cambiaSemilla(987654321);
  for(int i = 0; i < dim_ / 2; i++)
    ind1_[rdom->Randint(0, dim_)] = 1;
  ASSERT_GT(ff_->distance(ind1_, ind2_), 0); 
  
}



// factory functions:
INSTANTIATE_TEST_CASE_P(
    P1_2_3,
    FitnessTest,
    Values(&CreateFitness<0>, &CreateFitness<1>, &CreateFitness<2>));
