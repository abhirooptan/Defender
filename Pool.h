// ===============================================
// @file   pool.h
// @author kmurphy
// @brief  Specification of memory pool collection class 
//
// A pool collection is typicaly used to store a large number of 
// short lived objects where the number of simultaneous instances 
// is limited, eg bullets in a game.
// ===============================================

#ifndef POOL_H
#define POOL_H

#include <string>
#include <sstream>

#include "math.h"

template <class T> 
class Pool {

private:
	const int m_capacity;		// maximum number of concurrent instances
	int m_n;					// current number of instances
	T * m_pool;					// memory pool for instances

public:	

	Pool(int capacity) : 
		m_capacity(capacity), 			
		m_n(0), 						// initialy Pool is empty
		m_pool (new T[capacity]) { };	// allocate memory ONCE
		
	~Pool() { delete [] m_pool; }
	
	int capacity() const { return m_capacity; }
	int size() const { return m_n; }
	
	bool isFull() const { return (m_n==m_capacity); }    
	bool isEmpty() const { return (m_n==0); }

	T& operator[](int i) {
		ASSERT ((i>=0 && i<m_n), "Error in trying to access element at invalid index.");
		return *(m_pool + i);
	}
		
	const std::string toString() const {
		std::ostringstream outs;
		outs <<"(" <<m_n <<"/" <<m_capacity <<") [";
		for (int k=0; k<m_n; ++k) 
			outs <<(k? ", " : "") <<m_pool[k];
		outs <<"]";
		return outs.str();
	}

	T & allocate() {
		if (isFull()) {
			std::cerr << "Warning capacity of Pool exceeded." <<std::endl;
			return *m_pool;	// return the first (and possibly the oldest member)
		}
		
		m_n++;
		return *(m_pool+m_n-1);
	}

	void free (int i) {
		ASSERT ((i>=0 && i<m_n), "Error in trying to free element at invalid index.");
		
		m_pool[i] = m_pool[m_n-1];
		m_n--;
	}

	void freeAll() { m_n = 0; }

	friend std::ostream& operator<< (std::ostream& outputStream, const Pool<T>& p) {
		outputStream <<p.toString();
		return outputStream;
	}
};

#endif 


