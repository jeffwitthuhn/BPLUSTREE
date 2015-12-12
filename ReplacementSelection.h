/**
	* @file ReplacementSelection.h
	* Header file for the replacement selection algorithm implementation
	* @authors Jeff Witthuhn, Ross Kostron, Subodh Bhattarai, Andrew Morris, Alex Gatzke

*/
#ifndef REPLACEMENT_SELECTION
#define REPLACEMENT_SELECTION
#include <vector> 
#include <iostream>
#include <fstream>
#include <functional>
#include <typeinfo>


using namespace std; 

/**
	\brief The ReplacementSelection class header.
	The header class for ReplacementSelection with all of the function prototypes and member definitions
*/
template <class obj>
class ReplacementSelection{
private: 

	bool activeLeft; /**< true => left (lower) indices are active */
	bool ascending;/**< true => sort in ascending order */
	int size; /**< size of the simulated memory, does not change during execution */
	int leftStart; /**< root index of left heap */
	int leftEnd; /**< final index in left heap */
	int rightStart; /**< root index of right heap */
	int rightEnd; /**< final index of right heap */
	vector<obj> heap; /**< the dual heap used in the algorithm */

	/**
		The heapify method
		\param leftSide indicates which side of the dual heap is to be heapified.
		\return nothing
	*/
	void heapify (bool leftSide);
	
	/**
		A helper function for heapify that sifts index up
		\param index the index to sift
		\param leftSide indicates which side of the dual heap is to be heapified.
		\return nothing
	*/
	void siftUp(int index, bool leftSide);
	
	/**
		A helper function for heapify that sifts index down
		\param index the index to sift
		\param leftSide indicates which side of the dual heap is to be heapified.
		\return nothing
	*/
	void siftDown (int index, bool leftSide);

	/**
		The left method will return the left index
		\param index indicates the index.
		\param leftSide indicates which side of the dual heap it belongs to.
		\return the left index
	*/
	int left(int index, bool leftSide);
	
	/**
		The right method will return the right index
		\param index indicates the index.
		\param leftSide indicates which side of the dual heap it belongs to.
		\return the right index
	*/
	int right(int index, bool leftSide);
	
	/**
		The parent method will return the parent index
		\param index indicates the index.
		\param leftSide indicates which side of the dual heap it belongs to.
		\return the parent index
	*/
	int parent(int index, bool leftSide);
	
	/**
		The swap method will swap 2 indicies
		\param index1 a index.
		\param index2 another index.
		\return nothing
	*/
	void swap(int index1, int index2);

	/**
		removes the top item in the heap and maintains the heap properties
		\return the top item in the heap
	*/
	obj pop();
	
	/**
		initializes the heap for the first part of the algorithm
		\param infile the input file stream
		\return an item
	*/
	obj fill(istream& infile);
	
	/**
		pushes entry into the current heap and maintains the heap property.
		\param entry the item to push
		\return nothing
	*/
	void current_heap_push(const obj entry);
	/**
		pushes entry into the pending heap and maintains the heap property.
		\param entry the item to push
		\return nothing
	*/
	void pending_heap_push (const obj entry);
	
	/**
		toggles which heap is the 'active' heap
		\return nothing
	*/
	void toggleActive();
	
	/**
		prints the heap to cout for monitoring purposes
		\return nothing
	*/
	void printHeap();

	/**
		the driving method for the algorithm.
		\param infile the input file stream
		\param outfile the output file stream
		\return nothing
	*/
	void sortPreMerge(istream& infile,ostream& outfile);

public: 
	/**
		\brief The ReplacementSelection constructor.
		The constructor for the ReplacementSelection class
		\param _size the size
		\param _infile the input file
		\param _outfile the output file
		\param _sort the sort order
	*/	
	ReplacementSelection(const int _size=10,  istream& _infile = new ifstream("input.txt"),
				ostream& _outfile = cout,  const bool _sort = true);
};
#endif