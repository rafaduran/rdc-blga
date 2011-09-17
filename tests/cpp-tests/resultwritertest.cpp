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

std::vector<Param> get_params(void);
typedef ResultWriter<std::ofstream>* CreateRWFunc();

template <int rwNumber, int nVariables, class T>
ResultWriter<T>* CreateRW() {
    switch(nVariables){
        case 1:
            return  ResultWriter<T>::getResultWriter(rwNumber, 
                        "prueba_1.json", nVariables);
            break;
        case 5:
            return  ResultWriter<T>::getResultWriter(rwNumber, 
                        "prueba_5.json", nVariables);
            break;
    }
        
}


class RWTest : public TestWithParam<CreateRWFunc*> {
 public:
  virtual ~RWTest() { delete this->rw_; }
  virtual void SetUp() { 
    this->rw_ = (*GetParam())();
  }
  virtual void TearDown() {
    //I don't need deleting rw_ since ResultWriter already does
    this->rw_ = NULL;
  }

 protected:
  ResultWriter<std::ofstream>* rw_;
};


TEST_P(RWTest, GET_SET_Variables) {
    switch(rw_->getNVariables()){
        case 1:
        case 5:
            SUCCEED();
            break;
        default:
            FAIL();
    }
    rw_->setNVariables(3);
    ASSERT_EQ(3, rw_->getNVariables());
}

TEST_P(RWTest, GET_SET_Filename) {
    switch(rw_->getNVariables()){
        case 1:
            ASSERT_STREQ("prueba_1.json", rw_->getFilename());
            break;
        case 5:
            ASSERT_STREQ("prueba_5.json", rw_->getFilename());
            break;
        default:
            FAIL();
    }
    rw_->setFilename("prueba.json");
    ASSERT_STREQ("prueba.json", rw_->getFilename());
}

TEST_P(RWTest, WRITE) {

    int nVar = rw_->getNVariables();
    double* variables = new double[nVar];
    double fitness = 1.0;
    char* buffer;
    char *buffer_sample;
    int length, length_sample;
    
    for(int i = 0; i < nVar; i++){
        variables[i] = 1.0;
    }
    rw_->writeParams(get_params());
    rw_->startRun(0);
    rw_->startIteration(0);
    rw_->write(variables, fitness);
    rw_->write(variables, fitness, true);
    rw_->endIteration();
    rw_->startIteration(1);
    rw_->write(variables, fitness);
    rw_->write(variables, fitness, true);
    rw_->endIteration(true);
    rw_->endRun(false);
    rw_->startRun(1);
    rw_->startIteration(0);
    rw_->write(variables, fitness);
    rw_->write(variables, fitness, true);
    rw_->endIteration();
    rw_->startIteration(1);
    rw_->write(variables, fitness);
    rw_->write(variables, fitness, true);
    rw_->endIteration(true);
    rw_->endRun(true);
    
    SUCCEED();
}

// factory functions:
INSTANTIATE_TEST_CASE_P(
    BlgaJRW_1,
    RWTest,
    Values(&CreateRW<0, 1, std::ofstream>));

INSTANTIATE_TEST_CASE_P(
    BlgaJRW_5,
    RWTest,
    Values(&CreateRW<0, 5, std::ofstream>));

// Auxiliary function
std::vector<Param> get_params(void)
{
	std::vector<Param> params;
	for(int i=0; i < 5; i++){
		Param p;
		p.name = "hola";
		p.value = i;
		params.push_back(p);
	}
	return params;
}
