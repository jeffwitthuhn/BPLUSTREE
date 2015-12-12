/**
	* @file BPlusTree.h
	* Header file for B+ tree
	* @authors Jeff Witthuhn, Ross Kostron, Subodh Bhattarai, Andrew Morris, Alex Gatzke

*/
#ifndef BPLUSTREE_H
#define BPLUSTREE_H
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>
#include <iomanip>
#include <cctype>
#include <cstdlib>
#include "Block.h"
#include "ReplacementSelection.h"
#include "KeyPos.h"
#include "merge.h"
#include <cmath>
#include <sstream>

using namespace std;

/**
  * /breif Main B+ tree creator class.
  *
  * Use Block class objects to manipulate record and its indexes
  * to form sequence set and index set of the tree.
  *
  * *****************CONTROL FILE****************************
  * Must be able to do following operations:
  *   - open the file initially to get input files 
  *      ->( 1st project task plus Component A of 2nd project )
  *   - Create a B+ tree
  *   - Search for a record using a given key
  *   - Insert records ( Bulk loading and individual records)
  *   - Delete records ( Rebuild tree to take care of deletion)
  *   - Refresh -> (don't know what this would entail)
  *   - Check consistency ( DEBUG functions to check the correctness)
  */
template <class keyType>
class BPlusTree {
   public:
      int keyNum; /**< the key number */
      int order; /**< the order of the tree */
   	  int rootRBN; /**< the root relative block number */
      string originalFileName; /**< original file name */
      string treeFileName; /**< the tree file name */

	  /**
		The default constructor
	*/
      BPlusTree();
	  /**
		a constructor for b+ tree
		\param _keyNum key number
		\param _originalFileName the original file name
		\param _treeFileName the tree file name
		\param _order the tree order
	*/
      BPlusTree(int _keyNum, string _originalFileName, string _treeFileName, int _order=6);
      
	  /**
		Open the file
		\param _treeFileName the tree file name
	*/
	  void open (string _treeFileName);
	  
	  /**
		the create function
		\param _keyNum key number
		\param _originalFileName the original file name
		\param _treeFileName the tree file name
		\param _order the tree order
		\return nothing
	*/
      void create (string _originalFileName, string _treeFileName, int _keyNum=3, int _order=6);
	  
	  /**
		close the file
	*/
      void close ();
	  
	  /**
		the insert function
		\param key the key
		\param recAddr the record address
		\return nothing
		\post the record is inserted
	*/
      void insert (const keyType key, const int recAddr);
	  
	  /**
		the remove function
		\param key the key
		\param recAddr the record address
		\return nothing
		\post the record is removed
	*/
      void remove (const keyType key, const int recAddr = -1);
	  
	  /**
		the search function
		\param key the key
		\param recAddr the record address
		\return nothing
		\post the tree is searched
	*/
      void search (const keyType key, const int recAddr = -1);
	  
	  /**
		refresh the tree and clear up space
		\return nothing
	*/
      void refresh ();
	  
	  /**
		check to make sure that the tree is consistent
		\return nothing
	*/
      void checkConsistency ();
	  
	  /**
		makes a new tree
		\return nothing
	*/
      void newTree();
	  
	  /**
		print a visulization of the tree to the terminal
		\param ostream output stream
		\return nothing
		\post a tree is output to terminal
	*/
      void Print (ostream &);
	  
	  /**
		print a visulization of the tree to the terminal
		\param ostream output stream
		\param nodeAddr the node address
		\param level the level of the tree
		\return nothing
		\post a tree is output to terminal
	*/
      void Print (ostream &, int nodeAddr, int level);

   protected:
      Block * findLeaf (const keyType key);
      Block * newBlock();
      Block * fetch(const int recaddr);
      int store (Block *);
      Block Root; /**< the root */
      int Height; /**< the height of the tree */
};
#endif