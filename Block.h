/**
	* @file Block.h
	* Header file for the Block class
	* @authors Jeff Witthuhn, Ross Kostron, Subodh Bhattarai, Andrew Morris, Alex Gatzke

*/
#ifndef BLOCKH
#define BLOCKH
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>
#include <iomanip>
using namespace std;

/** \struct Record
	\breif The record struct.

	A struct to hold details about a Record.
*/
struct Record {
	string Last; /**< Last name */
	string First; /**< First name */
	int ID1; /**< ID number 1 */
	int ID2; /**< ID number 2 */
};

/**
	\breif The Block class header.
	The header class for Block with all of the function prototypes and member definitions
*/
class Block{

	private:
	//Member Variables
	int RBN; /**< Relative block number */
	int elementsUsed; /**< Elements used */
	int numChildren; /**< number of children */
	int parent; /**< Parent RBH */

	//fields used if Block is a leaf
	int left; /**< sibling RBN for left*/
	int right; /** sibling RBN for right */
	vector<Record> recordList; /**< A vector of records, size = order, min = ceil (order /2)-1 */

	//fields used if Block is an interior node
	vector<string> keyList; /**< A vector of keys, size = order-1 (+1 extra, so order), min ceil(order/2)-1 */
	vector<int> childrenList; /**< A vector of children, size = order, min ceil(order/2) */
	

	public:
	
	//BEGIN STATIC VARIABLES
	//These are shared by all instances of the Block class.
	static int order; /**< the order of the tree, If changed during exe we must rebuild tree */
	static int recordSize; /**< the size of each record */
	static int headerSize; /**< the side of the header */
	static int key; /**< an integer 1, 2, 3 or 4 that represents the field in the Record struct that the B+ tree is indexed by. */
	static int leafBlockSize; /**< the block size of a leaf */
	static int interiorBlockSize; /**< the block size of an interior node */
	static int initialFill; /**< how full each block will be filled initialy */
	static int difference; /**< the difference in block sizes */
	static int blockSize; /**< the block size. determined by the order, and by the format which we have chosen for the elements to be stored in each block */
	
	/**
		The initialization method for a block
		\param _order order of the tree, default value of 6
		\param _key indicates which key to sory by, default value 3
		\return nothing
	*/
	static void initBlockClass(int _order=6, int _key=3){
		order = _order;
		key=_key;
		recordSize=46;
		headerSize=89;
		initialFill = (int)(order * .75);
		initialFill = (initialFill>(order/2)?initialFill:(order/2));
		leafBlockSize=(order-1)*recordSize+28;
		interiorBlockSize=14+(order-1)*6+order*11+4-5;
		difference = (interiorBlockSize>leafBlockSize?
									interiorBlockSize-leafBlockSize
									:leafBlockSize- interiorBlockSize);
		blockSize= (interiorBlockSize>leafBlockSize?
								interiorBlockSize:leafBlockSize);
	}
	//END STATIC VARIABLES
	
	bool isRoot; /**< is the block a root? */
	bool isLeaf; /**< is the block a leaf? */
	
	/**
		a constructor for Block that recieves the relative block number
		\param _RBN relative block number
	*/
	Block (int _RBN);
	
	/**
		a constructor for Block that recieves the relative block number and file
		\param _RBN relative block number
		\param file the file
	*/
	Block (int _RBN, iostream& file);
	
	/**
		a constructor for Block that recieves the relative block number and if is a leaf or root
		\param _RBN relative block number
		\param _isRoot is the block a root
		\param _isLeaf is the block a leaf
	*/
	Block(bool _isRoot, bool _isLeaf, int _RBN);//for creating new nodes
	
	/**
		The default constructor
	*/
	Block();
	
	/**
		Set Root
		\param _isRoot bool value for if block is root
		\return nothing
		\post isRoot is set
	*/
	void setRoot(bool _isRoot){ isRoot=_isRoot; }
	
	/**
		Set Leaf
		\param _isLeaf bool value for if block is leaf
		\return nothing
		\post isLeaf is set
	*/
	void setLeaf(bool _isLeaf){	isLeaf=_isLeaf; }
	
	/**
		Toggle Root
		\return nothing
		\post value of isRoot is inverted
	*/
	void toggleRoot(){ isRoot=!isRoot; }
	
	/**
		Toggle Leaf
		\return nothing
		\post value of isLeaf is inverted
	*/
	void toggleLeaf(){ isLeaf=!isLeaf; }

	/**
		The getter for isLeaf
		\return isLeaf
	*/
	bool leaf(){ return isLeaf; }
	/**
		The getter for isRoot
		\return isRoot
	*/
	bool root(){ return isRoot; }

	/**
		Set Left
		\param _leftRBN relative block number for left
		\return nothing
		\post left is set
	*/
	void setLeft(int _leftRBN){ left = _leftRBN; }
	
	/**
		Set Right
		\param _rightRBN relative block number for right
		\return nothing
		\post right is set
	*/
	void setRight(int _rightRBN){ right=_rightRBN; }
	
	/**
		Set Parent
		\param _parentRBN relative block number for parent
		\return nothing
		\post parent is set
	*/
	void setParent(int _parentRBN){ parent = _parentRBN; }
	
	/**
		Set Relative block number
		\param _RBN relative block number
		\return nothing
		\post RBN is set
	*/
	void setRBN(int _RBN){ RBN=_RBN; }
	
	/**
		Set Key
		\param newKey the value for the new key
		\param i the key index
		\return nothing
		\post keylist[i] is set
	*/
	void setKey(string newKey, int i){
		keyList[i]=newKey;
	}

	/**
		The getter for right
		\return right
	*/
	int getRight(){ return right; }
	
	/**
		The getter for left
		\return left
	*/
	int getLeft(){ return left; }
	
	/**
		The getter for parent
		\return parent
	*/
	int getParent(){ return parent; }
	
	/**
		The getter for RBN
		\return RBN
	*/
	int getRBN(){ return RBN; }
	
	/**
		The getter for elements used
		\return elementsUsed
	*/
	int getUsed(){ return elementsUsed; }
	
	/**
		The getter for RBN at specific index
		\param nth_RBN the nth RBN
		\return the RBN at index n
	*/
	int getRBNat(int nth_RBN);
	
	/**
		The method to put a record into the record list
		\param r the record
		\return nothing
		\post record is pushed into recordList and elements used incremented
	*/
	void putRecord(Record r){
		assert(isLeaf);
		recordList.push_back(r);
		elementsUsed++;
	}
	
	/**
		The method to put a record into the record list at a specific index
		\param r the record
		\param i the index
		\return nothing
		\post record is pushed into recordList and elements used incremented
	*/
	void putKeyAt(string _key, int i);
	void putChildAt(int _RBN, int i);
	void putRecordAt(Record r, int i){
		assert(isLeaf);
		recordList.push_back(r);
		//shift things around; 
		elementsUsed++;
		for(int j=i; j<elementsUsed-1; j++){
			recordList[j+1].Last=recordList[j].Last;
			recordList[j+1].Last=recordList[j].First;
			recordList[j+1].Last=recordList[j].ID1;
			recordList[j+1].Last=recordList[j].ID2;
		}
		recordList[i].Last=r.Last;
		recordList[i].Last=r.First;
		recordList[i].Last=r.ID1;
		recordList[i].Last=r.ID2;
	}
	
	/**
		The method to remove a record from a specific index
		\param i the index
		\return nothing
		\post record removed from the index
	*/
	
	void removeRecordAt(int i);
	void removeChildAt(int i);
	void removeKeyAt(int i);
	
	/**
		\breif The method to kill a block.
		remove block, update sibling and parent pointers, called af
	*/
	void killBlock();
	
	/**
		The method to put a key into the key list
		\param key the key
		\return nothing
		\post key is pushed into keyList and elements used incremented
	*/
	void putKey(string key){
		assert(!isLeaf);
		keyList.push_back(key);
		elementsUsed++;
	}
	
	/**
		The method to put a child into the children list
		\param childRBN the child
		\return nothing
		\post child is pushed into childrenList and number of chidren incremented
	*/
	void putChild(int childRBN){
		assert(!isLeaf);
		childrenList.push_back(childRBN);
		numChildren++;
	}
	
	/**
		The method to ensure that elements used is equal to number of children - 1
		\return true if valid
	*/
	bool checkKeyRRN(){
		return elementsUsed == (numChildren-1);
	}
	
	/**
		The method to get a record at specific index
		\param i the index
		\return the record at index i
	*/
	Record getRecord(int i){
		assert(isLeaf&&i<elementsUsed&&"i>=elementsUsed, Block::getRecord(i), trying to get a record that doesnt exist");
		return recordList.at(i);
	}
	
	/**
		The method to get a key at specific index
		\param i the index
		\return the key at index i
	*/
	string getKey(int i){
		assert(!isLeaf&&i<elementsUsed&&"i>=elementsUsed. Block::getKey(i), trying to get a key that doesnt exist.");
		return keyList.at(i);
	}
	
	/**
		The method to get a child at specific index
		\param i the index
		\return the child at index i
	*/
	int getChild(int i){
		assert(!isLeaf&&i<numChildren&&"i>=numChildren. Block::getChild(i),trying to get a child that does not exist.");
		return childrenList.at(i);

	}
	
	/**
		The method to get the largest key
		\return the largest key
	*/
	string getLargestKey();
	
	/**
		The method to get the smallest key
		\return the smallest key
	*/
	string getSmallestKey();
	
	/**
		The method to get the key from a record
		\param r the record
		\return the key
	*/
	string KeyFromRecord(Record r);
	
	/**
		The method to write a block to output
		\param out the output stream
		\return nothing
		\post the record is output
	*/
	void writeBlock(ostream& out);
	
	/**
		The method to read a block from input
		\param _RBN relative block number
		\param file the input file stream
		\return nothing
		\post the record is input
	*/
	void readBlock(int _RBN, iostream& file);
	
	/**
		another method to write a block to output
		\param output the output stream
		\param b the block to output
		\return output stream for chaining
		\post the record is output
	*/
	friend ostream &operator<<( ostream &output, const Block &b ){ 
	         //print the formatted block to output. 
	         
	         return output;            
	      }


};
#endif