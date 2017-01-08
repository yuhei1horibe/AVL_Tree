/**
 * @author Yuhei Horibe
 * Data structure tests
 *
 * 2016/12/25
 * */

#include "DataStructureTest.h"
#include <math.h>
#include <time.h>
#include <vector>

template <class STR_TYPE, class NODE>
DataStrTest<STR_TYPE, NODE>::DataStrTest()
{
	//Random seed
	//Don't generate random seed more than twice.
	srand((unsigned int)time(nullptr));

	m_bStatLogEn	= false;

	std::string	FileName("log/");

	FileName	+= __TIMESTAMP__;
	std::replace(FileName.begin(), FileName.end(), ' ', '_');
	system("if [ ! -d log ]; then \
				mkdir log; \
			fi");
	FileName	+= ".log";
	m_ErrLog.exceptions(std::ofstream::failbit | std::ofstream::badbit);
	try{
		m_ErrLog.open(FileName.c_str());
	}
	catch(const std::ofstream::failure& e){
		std::cerr << "Log file open error" << std::endl;
	}
}

template <class STR_TYPE, class NODE>
DataStrTest<STR_TYPE, NODE>::~DataStrTest()
{
	m_ErrLog.close();
}

template <class STR_TYPE, class NODE>
bool DataStrTest<STR_TYPE, NODE>::GenerateAndRelease(TEST_TYPE type, unsigned int min, unsigned int max, unsigned int iteration)
{
	unsigned int	i;
	bool			bResult	= true;

	switch(type){
		case SEQ_NORMAL:
			m_ErrLog << "Sequential generation (in order) test." << std::endl;
			for(i = 0; i < iteration; i++){
				m_ErrLog << "\tIteration:" << i << std::endl;
				if(SeqGen(min, max) == true){
					bResult	&= FindTest(min, max);
#ifdef DEBUG
					if(m_DataStructure.get_height() > ceil(log(abs(max - min)) / log(2))){
						m_ErrLog << "Height: " << m_DataStructure.get_height() << std::endl;
						m_ErrLog << "log n :" << log(abs(max - min)) / log(2) << std::endl;
					}
#endif

					m_DataStructure.release();
				}

				else{
					m_ErrLog << "Generation error" << std::endl;
					m_DataStructure.release();
					bResult	= false;
				}
			}
			break;

		case SEQ_REVERSE:
			m_ErrLog << "Sequential generation (reversed) test." << std::endl;
			for(i = 0; i < iteration; i++){
				m_ErrLog << "\tIteration:" << i << std::endl;
				if(SeqGen(max, min) == true){
					bResult	&= FindTest(min, max);
#ifdef DEBUG
					if(m_DataStructure.get_height() > ceil(log(abs(max - min)) / log(2))){
						m_ErrLog << "\t\tHeight: " << m_DataStructure.get_height() << std::endl;
						m_ErrLog << "\t\tlog n :" << log(abs(max - min)) / log(2) << std::endl;
					}
#endif
					m_DataStructure.release();
				}

				else{
					m_ErrLog << "\t\tGeneration error" << std::endl;
					m_DataStructure.release();
					bResult	= false;
				}
			}
			break;

		case RANDOM:
			m_ErrLog << "Random generation test." << std::endl;
			for(i = 0; i < iteration; i++){
				m_ErrLog << "\tIteration:" << i << std::endl;
				if(RandGen(min, max) == true){
					bResult	&= FindTest(min, max);
#ifdef DEBUG
					if(m_DataStructure.get_height() > 10 + ceil(log(abs(max - min)) / log(2))){
						m_ErrLog << "\t\tHeight: " << m_DataStructure.get_height() << std::endl;
						m_ErrLog << "\t\tlog n :" << log(abs(max - min)) / log(2) << std::endl;
					}
#endif
					m_DataStructure.release();
				}

				else{
					m_ErrLog << "\t\tGeneration error" << std::endl;
					m_DataStructure.release();
					bResult	= false;
				}
			}
			break;

		default:
			m_ErrLog << "\t\tTEST_TYPE error." << std::endl;
			break;
	}
	return bResult;
}

//Generation and finding function testing
template <class STR_TYPE, class NODE>
bool DataStrTest<STR_TYPE, NODE>::FindTest(unsigned int min, unsigned int max)
{
	unsigned int		i;
	NODE*				tmp;
	bool				bResult	= true;
	unsigned int		key;
	std::vector<bool>	isGenerated(max - min, false);

	if(max <= min)
		return false;

	//Sequential access (increasing)
	for(i = min; i < max; i++){
		tmp	= m_DataStructure.find(i);

		if(tmp == nullptr){
			m_ErrLog << "\t\tKey:" << i << " not found." << std::endl;
			bResult	&= false;
		}
	}

	//Sequential access (decreasing)
	for(i = (max - 1); i >= min; i--){
		tmp	= m_DataStructure.find(i);

		if(tmp == nullptr){
			m_ErrLog << "\t\tKey:" << i << " not found." << std::endl;
			bResult	&= false;
		}
	}

	//Random access
	//Test every single value in the structure
	for(i = 0; i < (max - min); ){
		key	= (rand() % (max - min)) + min;

		if(isGenerated[key - min] == false){
			isGenerated[key - min]	= true;
			tmp	= m_DataStructure.find(key);
			if(tmp == nullptr){
				m_ErrLog << "\t\tKey:" << i << " not found." << std::endl;
				bResult	&= false;
			}
			i++;
		}
	}

	return bResult;
}

//Sequential generation for structure
template <class STR_TYPE, class NODE>
bool DataStrTest<STR_TYPE, NODE>::SeqGen(unsigned int boundary1, unsigned int boundary2)
{
	int				incr;
	int				i;
	NODE*			tmp;
	unsigned int	begin;
	unsigned int	end;
	bool			bResult	= true;

	if(boundary1 > boundary2){
		begin	= boundary1 - 1;
		end		= boundary2 - 1;
		incr	= -1;
	}

	else{
		begin	= boundary1;
		end		= boundary2;
		incr	= 1;
	}

	//Ensure the size() function
	if(m_DataStructure.size() != 0){
		m_ErrLog << "\t\tStructure supposed to be empty, but size() returns value: " << m_DataStructure.size() << std::endl;
		bResult	= false;
	}

	for(i = (signed int)begin; i != (signed int)end; i += incr){
		if(m_DataStructure.insert(i, abs(boundary2 - boundary1) - i) == false){
			m_ErrLog << "\t\tInsert i :" << i << " failed." << std::endl;
			bResult		= false;
		}

		//Make sure that the size() function works properly.
		if(m_DataStructure.size() != (abs(begin - i) + 1)){
			m_ErrLog << "\t\tStructure's size() function error. size() returns " << m_DataStructure.size() << " but size must be" << abs(begin - i) + 1 << std::endl;
		}

		//Ensure that the inserted node found in the tree
		tmp	= m_DataStructure.find(i);

		if(tmp == nullptr){
			m_ErrLog << "\t\tKey i: " << i << " not found." << std::endl;
			bResult		= false;
		}

		else if(tmp->get_val() != (abs(boundary2 - boundary1) - i)){
			m_ErrLog << "\t\tValue doesn't match.Value is supposed to be " << abs(boundary2 - boundary1) - i << " but the node has " << tmp->get_val() << "." << std::endl;
		}
	}
	return bResult;
}

//Random generation
template <class STR_TYPE, class NODE>
bool	DataStrTest<STR_TYPE, NODE>::RandGen(unsigned int min, unsigned int max)
{
	if(min >= max){
		m_ErrLog << "\t\tMinimum value is larger than maximum value" << std::endl;
		return false;
	}

	bool	bResult	= true;

	//Ensure the size() function
	if(m_DataStructure.size() != 0){
		m_ErrLog << "\t\tStructure supposed to be empty, but size() returns value: " << m_DataStructure.size() << std::endl;
		bResult	= false;
	}

	std::vector<bool>	isGenerated(max - min, false);
	unsigned int		uint_val;

	//Randomly generate every value in between min and max
	for(unsigned int i = 0; i < (max - min);){
		//If it is already generated.
		//Redo the generation.
		uint_val	= rand() % (max - min) + min;
		if(isGenerated[uint_val - min] == false){
			isGenerated[uint_val - min]	= true;
			i++;
			if(m_DataStructure.insert(uint_val, (max - min) - uint_val) == false){
				m_ErrLog << "\t\tInsert error. Key: " << uint_val << " failed." << std::endl;
				m_DataStructure.release();
				bResult		= false;
			}

			//Make sure that the size() function works properly.
			if(m_DataStructure.size() != i){
				m_ErrLog << "\t\tStructure's size() function error. size() returns " << m_DataStructure.size() << " but size must be" << i << std::endl;
			}
		}
	}

	return bResult;
}

//Removal test
template <class STR_TYPE, class NODE>
bool DataStrTest<STR_TYPE, NODE>::GenerateAndRemove(TEST_TYPE type, unsigned int min, unsigned int max, unsigned int iteration)
{
	unsigned int	i;
	bool			bResult	= true;

	switch(type){
		case SEQ_NORMAL:
			m_ErrLog << "Sequential removal (in order) test." << std::endl;
			for(i = 0; i < iteration; i++){
				m_ErrLog << "\tIteration:" << i << std::endl;

				//Sequential generation
				m_ErrLog << "\tSequential generation." << std::endl;
				if(SeqGen(min, max) == true){
					bResult	&= FindTest(min, max);
#ifdef DEBUG
					if(m_DataStructure.get_height() > ceil(log(abs(max - min)) / log(2))){
						m_ErrLog << "Height: " << m_DataStructure.get_height() << std::endl;
						m_ErrLog << "log n :" << log(abs(max - min)) / log(2) << std::endl;
					}
#endif
					if(SeqRemoval(min, max) == false){
						bResult		= false;
					}

#ifdef DEBUG
					//Ensure that the Tree root is null
					if(m_DataStructure.isRootNull() == false){
						m_ErrLog << "After the removal of all node, Tree root is not nullptr." << std::endl;
						bResult		= false;
					}
#endif
				}

				else{
					m_ErrLog << "Generation error" << std::endl;
					m_DataStructure.release();
					bResult	= false;
				}

				//Sequential generation (reversed)
				m_ErrLog << "\tSequential generation (reversed)" << std::endl;
				if(SeqGen(max, min) == true){
					bResult	&= FindTest(min, max);
#ifdef DEBUG
					if(m_DataStructure.get_height() > ceil(log(abs(max - min)) / log(2))){
						m_ErrLog << "\t\tHeight: " << m_DataStructure.get_height() << std::endl;
						m_ErrLog << "\t\tlog n :" << log(abs(max - min)) / log(2) << std::endl;
					}
#endif
					if(SeqRemoval(min, max) == false){
						bResult		= false;
					}
#ifdef DEBUG
					//Ensure that the Tree root is null
					if(m_DataStructure.isRootNull() == false){
						m_ErrLog << "After the removal of all node, Tree root is not nullptr." << std::endl;
						bResult		= false;
					}
#endif
				}

				else{
					m_ErrLog << "\t\tGeneration error" << std::endl;
					m_DataStructure.release();
					bResult	= false;
				}

				//Random generation
				m_ErrLog << "\tRandom generation" << std::endl;
				if(RandGen(min, max) == true){
					bResult	&= FindTest(min, max);
#ifdef DEBUG
					if(m_DataStructure.get_height() > ceil(log(abs(max - min)) / log(2))){
						m_ErrLog << "\t\tHeight: " << m_DataStructure.get_height() << std::endl;
						m_ErrLog << "\t\tlog n :" << log(abs(max - min)) / log(2) << std::endl;
					}
#endif
					if(SeqRemoval(min, max) == false){
						bResult		= false;
					}
#ifdef DEBUG
					//Ensure that the Tree root is null
					if(m_DataStructure.isRootNull() == false){
						m_ErrLog << "After the removal of all node, Tree root is not nullptr." << std::endl;
						bResult		= false;
					}
#endif
				}

				else{
					m_ErrLog << "\t\tGeneration error" << std::endl;
					m_DataStructure.release();
					bResult	= false;
				}
			}
			break;

		case SEQ_REVERSE:
			m_ErrLog << "Sequential removal (reversed) test." << std::endl;
			for(i = 0; i < iteration; i++){
				m_ErrLog << "\tIteration:" << i << std::endl;

				//Sequential generation
				m_ErrLog << "\tSequential generation." << std::endl;
				if(SeqGen(min, max) == true){
					bResult	&= FindTest(min, max);
#ifdef DEBUG
					if(m_DataStructure.get_height() > ceil(log(abs(max - min)) / log(2))){
						m_ErrLog << "Height: " << m_DataStructure.get_height() << std::endl;
						m_ErrLog << "log n :" << log(abs(max - min)) / log(2) << std::endl;
					}
#endif
					if(SeqRemoval(max, min) == false){
						bResult		= false;
					}
#ifdef DEBUG
					//Ensure that the Tree root is null
					if(m_DataStructure.isRootNull() == false){
						m_ErrLog << "After the removal of all node, Tree root is not nullptr." << std::endl;
						bResult		= false;
					}
#endif
				}

				else{
					m_ErrLog << "Generation error" << std::endl;
					m_DataStructure.release();
					bResult	= false;
				}

				//Sequential generation (reversed)
				m_ErrLog << "\tSequential generation (reversed)" << std::endl;
				if(SeqGen(max, min) == true){
					bResult	&= FindTest(min, max);
#ifdef DEBUG
					if(m_DataStructure.get_height() > ceil(log(abs(max - min)) / log(2))){
						m_ErrLog << "\t\tHeight: " << m_DataStructure.get_height() << std::endl;
						m_ErrLog << "\t\tlog n :" << log(abs(max - min)) / log(2) << std::endl;
					}
#endif
					if(SeqRemoval(max, min) == false){
						bResult		= false;
					}
#ifdef DEBUG
					//Ensure that the Tree root is null
					if(m_DataStructure.isRootNull() == false){
						m_ErrLog << "After the removal of all node, Tree root is not nullptr." << std::endl;
						bResult		= false;
					}
#endif
				}

				else{
					m_ErrLog << "\t\tGeneration error" << std::endl;
					m_DataStructure.release();
					bResult	= false;
				}

				//Random generation
				m_ErrLog << "\tRandom generation" << std::endl;
				if(RandGen(min, max) == true){
					bResult	&= FindTest(min, max);
#ifdef DEBUG
					if(m_DataStructure.get_height() > ceil(log(abs(max - min)) / log(2))){
						m_ErrLog << "\t\tHeight: " << m_DataStructure.get_height() << std::endl;
						m_ErrLog << "\t\tlog n :" << log(abs(max - min)) / log(2) << std::endl;
					}
#endif
					if(SeqRemoval(max, min) == false){
						bResult		= false;
					}
#ifdef DEBUG
					//Ensure that the Tree root is null
					if(m_DataStructure.isRootNull() == false){
						m_ErrLog << "After the removal of all node, Tree root is not nullptr." << std::endl;
						bResult		= false;
					}
#endif
				}

				else{
					m_ErrLog << "\t\tGeneration error" << std::endl;
					m_DataStructure.release();
					bResult	= false;
				}
			}
			break;

		case RANDOM:
			m_ErrLog << "Random removal test." << std::endl;
			for(i = 0; i < iteration; i++){
				m_ErrLog << "\tIteration:" << i << std::endl;

				//Sequential generation
				m_ErrLog << "\tSequential generation." << std::endl;
				if(SeqGen(min, max) == true){
					bResult	&= FindTest(min, max);
#ifdef DEBUG
					if(m_DataStructure.get_height() > ceil(log(abs(max - min)) / log(2))){
						m_ErrLog << "Height: " << m_DataStructure.get_height() << std::endl;
						m_ErrLog << "log n :" << log(abs(max - min)) / log(2) << std::endl;
					}
#endif
					if(RandRemoval(min, max) == false){
						bResult		= false;
					}
#ifdef DEBUG
					//Ensure that the Tree root is null
					if(m_DataStructure.isRootNull() == false){
						m_ErrLog << "After the removal of all node, Tree root is not nullptr." << std::endl;
						bResult		= false;
					}
#endif
				}

				else{
					m_ErrLog << "Generation error" << std::endl;
					m_DataStructure.release();
					bResult	= false;
				}

				//Sequential generation (reversed)
				m_ErrLog << "\tSequential generation (reversed)" << std::endl;
				if(SeqGen(max, min) == true){
					bResult	&= FindTest(min, max);
#ifdef DEBUG
					if(m_DataStructure.get_height() > ceil(log(abs(max - min)) / log(2))){
						m_ErrLog << "\t\tHeight: " << m_DataStructure.get_height() << std::endl;
						m_ErrLog << "\t\tlog n :" << log(abs(max - min)) / log(2) << std::endl;
					}
#endif
					if(RandRemoval(min, max) == false){
						bResult		= false;
					}
#ifdef DEBUG
					//Ensure that the Tree root is null
					if(m_DataStructure.isRootNull() == false){
						m_ErrLog << "After the removal of all node, Tree root is not nullptr." << std::endl;
						bResult		= false;
					}
#endif
				}

				else{
					m_ErrLog << "\t\tGeneration error" << std::endl;
					m_DataStructure.release();
					bResult	= false;
				}

				//Random generation
				m_ErrLog << "\tRandom generation" << std::endl;
				if(RandGen(min, max) == true){
					bResult	&= FindTest(min, max);
#ifdef DEBUG
					if(m_DataStructure.get_height() > ceil(log(abs(max - min)) / log(2))){
						m_ErrLog << "\t\tHeight: " << m_DataStructure.get_height() << std::endl;
						m_ErrLog << "\t\tlog n :" << log(abs(max - min)) / log(2) << std::endl;
					}
#endif
					if(RandRemoval(min, max) == false){
						bResult		= false;
					}
				}

				else{
					m_ErrLog << "\t\tGeneration error" << std::endl;
					m_DataStructure.release();
					bResult	= false;
				}
			}
			break;

		default:
			m_ErrLog << "\t\tTEST_TYPE error." << std::endl;
			break;
	}
	return bResult;
}

//Sequential removal
template <class STR_TYPE, class NODE>
bool DataStrTest<STR_TYPE, NODE>::SeqRemoval(unsigned int boundary1, unsigned int boundary2)
{
	int				incr;
	int				i;
	int				j;
	unsigned int	itr	= 0;
	NODE*			tmp;
	unsigned int	begin;
	unsigned int	end;

	bool			bResult	= true;

	if(boundary1 > boundary2){
		begin	= boundary1 - 1;
		end		= boundary2 - 1;
		incr	= -1;
	}

	else{
		begin	= boundary1;
		end		= boundary2;
		incr	= 1;
	}

	//Check size function
	if(m_DataStructure.size() != abs(boundary2 - boundary1)){
		m_ErrLog << "\t\tsize(): " << m_DataStructure.size() << " doesn't match the actual size: " << abs(boundary2 - boundary1) << std::endl;
		bResult	= false;
	}

	for(i = (signed int)begin; i != (signed int)end; i += incr){
		m_DataStructure.remove(i);

		itr++;

		//Check the size() function works properly.
		if(m_DataStructure.size() != (abs(boundary2 - boundary1) - itr)){
			m_ErrLog << "\t\tsize(): " << m_DataStructure.size() << " doesn't match the actual size: " << abs(boundary2 - boundary2) - itr << std::endl;
			bResult	= false;
		}

		//Make sure that the removed node is not found in the tree.
		tmp	= m_DataStructure.find(i);

		if(tmp != nullptr){
			m_ErrLog << "\t\tKey: " << i << " removed but found in the tree. Removal failure." << std::endl;
			bResult	= false;
		}

		//Ensure that all of the remained node 
		//must be found
		for(j = (signed int)begin + (itr * incr); j != (signed int)end; j += incr){
			tmp	= m_DataStructure.find(j);

			//If it's not found
			if(tmp == nullptr){
				m_ErrLog	<< "\t\tKey i: " << j << "not found." << std::endl;
				bResult	= false;
			}

			else if(tmp->get_val() != (abs(boundary2 - boundary1) - j)){
				m_ErrLog << "\t\tValue doesn't match.Value is supposed to be " << abs(boundary2 - boundary1) - j << " but the node has " << tmp->get_val() << "." << std::endl;
				bResult	= false;
			}
			tmp	= nullptr;
		}
	}
	return bResult;
}

//Random removal
template <class STR_TYPE, class NODE>
bool DataStrTest<STR_TYPE, NODE>::RandRemoval(unsigned int min, unsigned int max)
{
	typedef			std::vector<bool>	bool_vec;

	unsigned int	i;
	unsigned int	j;
	unsigned int	key;
	NODE*			tmp;
	bool_vec		bRemoved(max - min, false);
	bool			bResult	= true;

	if(min > max)
		return false;

	//Check the size() function
	if(m_DataStructure.size() != (max - min)){
		m_ErrLog << "\t\tsize(): " << m_DataStructure.size() << " doesn't match the actual size: " << (max - min) << std::endl;
		bResult	= false;
	}

	for(i = min; i < max; i++){
		//Generate the key remained in the tree.
		while(1){
			key	= rand() % (max - min);
			if(bRemoved[key] == false){
				bRemoved[key]	= true;

				m_DataStructure.remove(key + min);

				//Make sure that the node has key removed.
				tmp	= m_DataStructure.find(key + min);
				if(tmp != nullptr){
					m_ErrLog << "\t\tKey: " << key + min << " removed but found in the tree. Removal failure." << std::endl;
					bResult	= false;
				}
				break;
			}
		}

		//Ensure that all of the remained node 
		//must be found
		for(j = min; j < max; j++){
			if(bRemoved[j - min] == false){
				tmp	= m_DataStructure.find(j);

				//If it's not found
				if(tmp == nullptr){
					m_ErrLog	<< "\t\tKey: " << j << "not found." << std::endl;
					bResult	= false;
				}

				else if(tmp->get_val() != ((max - min) - j)){
					m_ErrLog << "\t\tValue doesn't match.Value is supposed to be " << (max - min) - j << " but the node has " << tmp->get_val() << "." << std::endl;
					bResult	= false;
				}
			}
		}
	}
	return bResult;
}

#ifdef DEBUG
template <class KEY, class VAL>
bool AVL_Tree<KEY, VAL>::isRootNull()
{
	return m_lpTreeTop == NULL;
}
#endif

