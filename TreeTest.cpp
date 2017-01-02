/**
 * @author Yuhei Horibe
 * Tree structure tests
 *
 * 2016/12/25
 * */

#include "TreeTest.h"
#include <math.h>
#include <time.h>
#include <vector>
#include <iostream>

template <class STR_TYPE, class NODE>
bool DataStrTest<STR_TYPE, NODE>::GenerateAndRelease(TEST_TYPE type, unsigned int min, unsigned int max, unsigned int iteration)
{
	unsigned int	i;
	bool			bResult	= true;

	switch(type){
		case SEQ_NORMAL:
			for(i = 0; i < iteration; i++){
				if(SeqGen(min, max) == true){
					bResult	&= FindTest(min, max);
					m_DataStructure.release_all();
				}

				else{
					std::cerr << "Generation error" << std::endl;
					m_DataStructure.release_all();
					bResult	= false;
				}
			}
			break;

		case SEQ_REVERSE:
			for(i = 0; i < iteration; i++){
				if(SeqGen(max, min) == true){
					bResult	&= FindTest(min, max);
					m_DataStructure.release_all();
				}

				else{
					std::cerr << "Generation error" << std::endl;
					m_DataStructure.release_all();
					bResult	= false;
				}
			}
			break;

		case RANDOM:
			for(i = 0; i < iteration; i++){
				if(RandGen(min, max) == true){
					bResult	&= FindTest(min, max);
					m_DataStructure.release_all();
				}

				else{
					std::cerr << "Generation error" << std::endl;
					m_DataStructure.release_all();
					bResult	= false;
				}
			}
			break;

		default:
			std::cout << "Default executed." << std::endl;
			break;
	}
	return bResult;
}

//Generation and finding function testing
template <class STR_TYPE, class NODE>
bool DataStrTest<STR_TYPE, NODE>::FindTest(unsigned int min, unsigned int max)
{
	unsigned int	i;
	NODE*			tmp;
	bool			bResult	= true;
	unsigned int	key;

	if(max <= min)
		return false;

	srand((unsigned int)time(NULL));

	//Sequential access (increasing)
	for(i = min; i < max; i++){
		tmp	= m_DataStructure.find(i);

		if(tmp == NULL){
			std::cerr << "Key:" << i << " not found." << std::endl;
			bResult	&= false;
		}
	}

	//Sequential access (decreasing)
	for(i = (max - 1); i >= min; i--){
		tmp	= m_DataStructure.find(i);

		if(tmp == NULL){
			std::cerr << "Key:" << i << " not found." << std::endl;
			bResult	&= false;
		}
	}

	//Random access
	for(i = 0; i < 2 * (max - min); i++){
		key	= (rand() % (max - min)) + min;

		tmp	= m_DataStructure.find(key);
		if(tmp == NULL){
			std::cerr << "Key:" << i << " not found." << std::endl;
			bResult	&= false;
		}
	}

	return bResult;
}

//Sequential generation for Tree
template <class STR_TYPE, class NODE>
bool DataStrTest<STR_TYPE, NODE>::SeqGen(unsigned int boundary1, unsigned int boundary2)
{
	int			incr;
	int			i;
	NODE*		tmp;

	if(boundary1 > boundary2){
		incr	= -1;
	}

	else{
		incr	= 1;
	}

	for(i = (signed int)boundary1; i != (signed int)boundary2; i += incr){
		if(m_DataStructure.insert(i, abs(boundary2 - boundary1) - i) == false){
			std::cerr << "Insert i :" << i << " failed." << std::endl;
			return false;
		}

		//Ensure that the inserted node found in the tree
		tmp	= m_DataStructure.find(i);

		if(tmp == NULL){
			std::cerr << "Key i: " << i << " not found." << std::endl;
			return false;
		}

		else if(tmp->get_val() != (abs(boundary2 - boundary1) - i)){
			std::cerr << "Value doesn't match.Value is supposed to be " << abs(boundary2 - boundary1) - i << " but the node has " << tmp->get_val() << "." << std::endl;
		}
	}
	return true;
}

//Random generation
template <class STR_TYPE, class NODE>
bool	DataStrTest<STR_TYPE, NODE>::RandGen(unsigned int min, unsigned int max)
{
	if(min >= max){
		std::cerr << "Minimum value is larger than maximum value" << std::endl;
		return false;
	}

	std::vector<bool>	isGenerated(max - min, false);
	unsigned int		uint_val;
	NODE*				tmp;

	//Random seed
	srand((unsigned int)time(NULL));

	//Randomly generate every value in between min and max
	for(unsigned int i = 0; i < (max - min);){
		//If it is already generated.
		//Redo the generation.
		uint_val	= rand() % (max - min) + min;
		if(isGenerated[uint_val - min] == false){
			isGenerated[uint_val - min]	= true;
			i++;
			if(m_DataStructure.insert(uint_val, (max - min) - uint_val) == false){
				std::cerr << "Insert error. Key: " << uint_val << " failed." << std::endl;
				m_DataStructure.release_all();
				return false;
			}
		}
	}

	return true;
}

