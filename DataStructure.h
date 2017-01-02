/**
 * @author Yuhei Horibe
 * structure Structure Test
 * 2016/12/25
 **/

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <iostream>
#include <fstream>

//Include Guard
#ifndef __TEST_TYPE__
#define	__TEST_TYPE__
enum TEST_TYPE{
	SEQ_NORMAL,
	SEQ_REVERSE,
	RANDOM
};
#endif

//structure generation test
template <class STR_TYPE, class NODE>
class DataStrTest{
private:
	STR_TYPE		m_DataStructure;

	//Log file
	std::ofstream	m_StatLog;
	std::ofstream	m_ErrLog;

	//Log enable
	bool			m_bStatLogEn;

	//Private method
	bool	FindTest(unsigned int min, unsigned int max);
	bool	SeqGen(unsigned int boundary1, unsigned int boundary2);
	bool	RandGen(unsigned int min, unsigned int max);


public:
	//Constructor
	DataStrTest();

	//Repeat generation and release for given times
	bool	GenerateAndRelease(TEST_TYPE type, unsigned int min, unsigned int max, unsigned int iteration);

	//Remove the node from the tree
	bool	GenerateAndRemove(TEST_TYPE type, unsigned int min, unsigned int max);
};

