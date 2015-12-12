/**
	* @file BPlusTree.cpp
	* implementation file for B+ tree
	* @authors Jeff Witthuhn, Ross Kostron, Subodh Bhattarai, Andrew Morris, Alex Gatzke

*/

#include "BPlusTree.h"
#include "fileOperations.h"

template <class keyType>
BPlusTree<keyType>::BPlusTree(){}
template <class keyType>
BPlusTree<keyType>::BPlusTree(int _keyNum, string _originalFileName, string _treeFileName, int _order){
	order =_order;
	keyNum=_keyNum;
	originalFileName=_originalFileName;
	treeFileName=_treeFileName;
}
template <class keyType>
void BPlusTree<keyType>::search(string parameterKey)
{
    int order = 0;
	int key = 0;
	int rootRBN = 0;	
	
	string mySearchKey = parameterKey;
	string searchKey = 0;
	int elementsUsed = 0;
	int pointer = 0;
	Record tempRecord;
	
	/* update tree variables */
	order = BPlusTree.getField("order");
	key = BPlusTree.getField("key");
	rootRBN = BPlusTree.getField("rootRBN");
	
	/* load root block into memory */
	Block block(rootRBN, file);
	block.readBlock(rootRBN, file);
	
	/* update block variables */		
	elementsUsed = block.getUsed();
	
	/* while at an interior node */
	while (!block.isLeaf())
	{
		/* locate appropriate searchKey value */
		int i = 0;
		for (i = 0; i < elementsUsed; i++)
		{
			searchKey = block.getKey[i];
			if (mySearchKey.compare(searchKey) < 0)//CHANGE TO STRING COMPARE
				break;
		}
		
		/* get child RBN associated with appropriate searchKey value */
		pointer = block.getChild[i];
	
		/* read child block (new current block) into memory */
		block.readBlock(pointer, file);
		
		/* update elementsUsed */
		elementsUsed = block.getUsed()
	}
	
	int i = 0;
	for (i = 0; i < elementsUsed - 1; i++)
	{		
		tempRecord = block.getChild[i];		
		
		if (key == 1)
		{
			/* key is Last */
			searchKey = tempRecord.Last;			
		}
		else if (key == 2)
		{
			/* key is First */
			searchKey = tempRecord.First;
		}
		else if (key == 3)
		{
			/* key is ID1 */
			searchKey = tempRecord.ID1;
		}
		else if (key == 4)
		{
			/* key is ID2 */
			searchKey = tempRecord.ID2;
		}
		
		if (searchKey > mySearchKey)
			break;
	}
	
	if (searchKey.compare(mySearchKey) == 0)
	{
		cout << "Record found: " << tempRecord.Last ", " << tempRecord.First
			<< " " << tempRecord.ID1 << " " << tempRecord.ID2 << endl;
	}
	else
	{
		cout << "Sorry, no record with that key exists in the tree" << endl;
	}
	
	return 0;
}



template <class keyType>
void BPlusTree<keyType>::Print(ostream& outfile){
   fstream file(treeFileName, std::fstream::in | std::fstream::out);
   Block root_block;
   root_block.readBlock(rootRBN, file);
   outfile<<"root block:"<<endl;
   //root_block.writeBlock(outfile);
   Record r;
   if(root_block.leaf()){
      outfile<<"LEAF BLOCKS\n";
   for(int i=0; i<root_block.getUsed(); i++){
         r=root_block.getRecord(i);
         outfile <<"|"<<setfill(' ')<<setw(10)<<r.Last<<" | ";
      }
      outfile<<endl;
   for(int i=0; i<root_block.getUsed(); i++){
         r=root_block.getRecord(i);
         outfile <<"|"<<setfill(' ')<<setw(10)<<r.First<<" | ";
      }
      outfile<<endl;
   for(int i=0; i<root_block.getUsed(); i++){
         r=root_block.getRecord(i);
         outfile <<"|"<<setfill('0')<<setw(10)<<r.ID1<<" | ";
      }
      outfile<<endl;
   for(int i=0; i<root_block.getUsed(); i++){
         r=root_block.getRecord(i);
         outfile <<"|"<<setfill('0')<<setw(10)<<r.ID2<<" | ";
      }
      outfile<<endl;
   }

}
template <class keyType>
void BPlusTree<keyType>::open(string _treeFileName) {
   treeFileName = _treeFileName;
   ifstream infile(treeFileName);
   keyNum = getField("KY",infile);
   rootRBN = getField("RR",infile);
   order = getField("OR",infile);
}


template <class keyType>
void BPlusTree<keyType>::create(string _originalFileName, string _treeFileName, int _keyNum, int _order) {
	order =_order;
	keyNum=_keyNum;
	originalFileName=_originalFileName;
	treeFileName=_treeFileName;
   extractKeys(originalFileName,"tmp_extracted_"+originalFileName, keyNum);
   int memory=15;///number of items allowed in memory for replacement selection
   bool ascending=true;
   ifstream inputFile("tmp_extracted_"+originalFileName);
   ofstream otemp("tmp_runs_"+originalFileName, std::ios_base::binary | std::ios_base::out |std::ios_base::trunc );
   if(keyNum==1||keyNum==2){
   ReplacementSelection<KeyPos<string> > sortedRuns(memory,inputFile,otemp,ascending);
   otemp.close();
   MergeFile<KeyPos<string> >("tmp_sortedRuns_"+originalFileName,"tmp_runs_"+originalFileName,ascending);
   }
   else{
   ReplacementSelection<KeyPos<int> > sortedRuns(memory,inputFile,otemp,ascending);
   otemp.close();
   MergeFile<KeyPos<int> >("tmp_sortedRuns_"+originalFileName,"tmp_runs_"+originalFileName,ascending);
   }
   rewriteRecordFromKey(originalFileName, "tmp_sortedRuns_"+originalFileName);
   initSequenceSet ("sorted"+originalFileName,treeFileName,order,keyNum); 
   buildFromSS(treeFileName);
}

template <class keyType>
void BPlusTree<keyType>::close(){

}

template <class keyType>
void BPlusTree<keyType>::insert(const keyType key, const int recAddr) {
   
}
/*
    addRecord() is called from insertRecord(). It uses splitBlock() to
	handle overfull blocks.
*/

int addRecord(Record record, int RBN, fstream& file)
{
	Record tempRecord;
	int key = 0;
	int elementsUsed = 0;
	
	int searchKey = 0;
	int mySearchKey = 0;

	/* load block into memory */
	Block block(RBN, file);
	block.readBlock(RBN, file);
	
	/*set block variables */
	key = BPlusTree.getField("key");
	elementsUsed = block.getUsed();
	
	/* get position at which to insert new record */
	/* this is done by iterative comparison */
	int i = 0;
	for (i = 0; i < elementsUsed; i++)
	{
		tempRecord = block.getChild[i];
		if (key == 3)
		{ //case is ID1
			searchKey = tempRecord.ID1;
			mySearchKey = record.ID1;
			
			if (searchKey > mySearchKey)
				break;
		}
		else if (key == 4)
		{ //case is ID2
			searchKey = tempRecord.ID2;
			mySearchKey = record.ID2;
			
			if (searchKey > mySearchKey)
				break;
		}
	}
	
	/* put child in appropriate place in block */
	if (i == elementsUsed)
		block.putRecord(record);
	else
		block.putRecordAt(record, i);
		
	/* if block is now overfull, time to split */
	block.readBlock(RBN, file);
	elementsUsed = block.getUsed();
	
	if (elementsUsed > (order - 1))
		splitBlock(RBN, file);
		
	return 0;	
}

/****************************************************************/
/*                      SPLIT BLOCK                             */
/****************************************************************/

int splitBlock(int rel_bl_num, filestream& filename) {
    // Handle 4 cases:
    //  i. Root
    //      a. Root is the only node in tree
    //      b. Otherwise
    //  ii. Leaf
    //  iii. Internal node
    
    // Local objects
    Block overfullBlock(rel_bl_num, filename);
    BPlusTree tree_object;
    Record rec;
    
    // Number of elements in the block
    int elements     = 0;
    elements         = overfullBlock.getUsed();
    getLeftOverfull  = overfullBlock.getLeft();
    getRightOverfull = overfullBlock.getRight();
    getRBNOverfull   = overfullBlock.getRBN();
    
    // Block order
    int order = 0;
    order = tree_object.getField("order");
    
    // Midpoint of block
    int splitted;             // Split the block into two
    splitted = cut(elements);
    
    // Local Rel block number
    int rbn;
    
    if (overfullBlock.root()) { // When root
        
        // ROOT: HANDLE BOTH CASES
        if (!overfullBlock.getParent() && elements == (order - 1)) {
            // Root is the only node here
            
            /*****************************************************************/
            // Allocate new leaf and move half the block's elements to new block
            /*****************************************************************/
            // Split it into two nodes and push the root up from the bottom
            Block newBlock(false, true, rel_bl_num); // Root Block (parent)
            Block newBlock1(); // Leaf
            Block newBlock2(); // Leaf
            
            newBlock1 = overfullBlock.putRecordAt(rec, 0, splitted);
            newBlock2 = overfullBlock.putRecordAt(rec, (splitted+1), elements);
            
            /*****************************************************************/
            /* Insert the new leaf's smallest key and address into the parent*/
            /*****************************************************************/
            // Point the first pointer (left) of the root block (newBlock) to
            // point to newBlock1
            newBlock.setParent(rel_bl_num);
            newBlock1.setLeaf(true);
            newBlock2.setLeaf(true);
            int _newBlock1_RBN = newBlock1.getRBN();
            int _newBlock2_RBN = newBlock2.getRBN();
            setLeft(_newBlock1_RBN);
            setRight(_newBlock2_RBN);
            
            // Copy the first key of the newBlock2 and pop up to the first key
            // of the root block (newBlock)
            newBlock.putKey(newBlock2.KeyFromRecord(rec));
            
            // Kill the overfull block
            overfullBlock.killBlock();
            
        } else {
            // Other cases
            //  Check if the minimum node is at least 2
            if (elements == 1) {
                // Boundary testing
                // Exit out of this
                cout << "Problem with the logic! This shouldn't happen!\n";
                break;
            }
            
            // Check if the root's pointers all have children and
            // if the root if overfull again
            if (overfullBlock.getUsed() == order) {
                /*****************************************************************/
                // Allocate new leaf and move half the block's elements to new block
                /*****************************************************************/
                
                Block newBlock(false, true, rel_bl_num); // Root (parent)
                Block newBlock1(); // Internal
                Block newBlock2(); // Internal
                
                /*****************************************************************/
                /* Insert the new leaf's smallest key and address into the parent*/
                /*****************************************************************/
                // Point the first pointer (left) of the root block (newBlock) to
                // point to newBlock1
                newBlock.setParent(rel_bl_num);
                newBlock1.setLeaf(false);
                newBlock2.setLeaf(false);
                int _newBlock1_RBN = newBlock1.getRBN();
                int _newBlock2_RBN = newBlock2.getRBN();
                setLeft(_newBlock1_RBN);
                setRight(_newBlock2_RBN);
                putChild(_newBlock1_RBN);
                putChild(_newBlock2_RBN);
                
                // Copy the first key of the newBlock2 and pop up to the first key
                // of the root block (newBlock)
                newBlock.putKey(newBlock2.KeyFromRecord(rec));
                
                // Kill the overfull block
                overfullBlock.killBlock();
                
            } else {
                /*****************************************************************/
                // Allocate new leaf and move half the block's elements to new block
                /*****************************************************************/
                
                Block newBlock(false, true, rel_bl_num); // Root (parent)
                Block newBlock1(); // Leaf
                Block newBlock2(); // Leaf
                
                newBlock1 = overfullBlock.putRecordAt(rec, 0, splitted);
                newBlock2 = overfullBlock.putRecordAt(rec, (splitted+1), elements);
                
                /*****************************************************************/
                /* Insert the new leaf's smallest key and address into the parent*/
                /*****************************************************************/
                // Point the first pointer (left) of the root block (newBlock) to
                // point to newBlock1
                newBlock.setParent(rel_bl_num);
                newBlock1.setLeaf(true);
                newBlock2.setLeaf(true);
                int _newBlock1_RBN = newBlock1.getRBN();
                int _newBlock2_RBN = newBlock2.getRBN();
                setLeft(_newBlock1_RBN);
                setRight(_newBlock2_RBN);
                
                // Copy the first key of the newBlock2 and pop up to the first key
                // of the root block (newBlock)
                newBlock.putKey(newBlock2.getKey());
                
                // Kill the overfull block
                overfullBlock.killBlock();
                
            }
            
        } // END of ROOT CASES
        
    } else if (overfullBlock.leaf()) { // leaf
        // LEAF BLOCK
        // The block is a leaf
        
        /*****************************************************************/
        // Allocate new leaf and move half the block's elements to new block
        /*****************************************************************/
        Block newBlock(false, false, rel_bl_num); // Internal node (parent)
        Block newBlock1(); // Leaf
        Block newBlock2(); // Leaf
        
        newBlock1 = overfullBlock.putRecordAt(rec, 0, splitted);
        newBlock2 = overfullBlock.putRecordAt(rec, (splitted+1), elements);
        
        /*****************************************************************/
        /* Insert the new leaf's smallest key and address into the parent*/
        /*****************************************************************/
        // Point the first pointer (left) of the leaf block (newBlock) to
        // point to newBlock1
        newBlock.setParent(rel_bl_num);
        newBlock1.setLeaf(true);
        newBlock2.setLeaf(true);
        int _newBlock1_RBN = newBlock1.getRBN();
        int _newBlock2_RBN = newBlock2.getRBN();
        setLeft(_newBlock1_RBN);
        setRight(_newBlock2_RBN);
        
        // Copy the first key of the newBlock2 and pop up to the first key
        // of the leaf block (newBlock)
        newBlock.putKey(newBlock2.KeyFromRecord(rec));
        
        // Kill the overfull block
        overfullBlock.killBlock();
        
        // Check if the parent is full
        // If yes, split it too
            // add the middle key to the parent node
        // Repeat until a parent if found that need not split.
        if (newBlock.getParent()) {
            rbn = newBlock.getRBN();
            splitBlock(rbn, filename);
        }
        // END OF LEAF CASES
        
    } else {    // internal node
        // INTERNAL BLOCK
        /*****************************************************************/
        // Allocate new leaf and move half the block's elements to new block
        /*****************************************************************/
        Block newBlock(false, false, rel_bl_num); // Internal node (parent)
        Block newBlock1(); // Int node
        Block newBlock2(); // Int node
        
        //newBlock1 = overfullBlock.putRecordAt(rec, 0, splitted);
        //newBlock2 = overfullBlock.putRecordAt(rec, (splitted+1), elements);
        
        /*****************************************************************/
        /* Insert the new leaf's smallest key and address into the parent*/
        /*****************************************************************/
        // Point the first pointer (left) of the internal node block (newBlock) to
        // point to newBlock1
        newBlock.setParent(rel_bl_num);
        newBlock1.setLeaf(false);
        newBlock2.setLeaf(false)
        int _newBlock1_RBN = newBlock1.getRBN();
        int _newBlock2_RBN = newBlock2.getRBN();
        setLeft(_newBlock1_RBN);
        setRight(_newBlock2_RBN);
        putChild(_newBlock1_RBN);
        putChild(_newBlock2_RBN);
        
        // Copy the first key of the newBlock2 and pop up to the first key
        // of the internal node block (newBlock)
        newBlock.putKey(newBlock2.getKey(0));
        
        // Kill the overfull block
        overfullBlock.killBlock();
        
        // Check if the parent is full
        // If yes, split it too
        // add the middle key to the parent node
        // Repeat until a parent if found that need not split.
        if (newBlock.getParent()) {
            rbn = newBlock.getRBN();
            splitBlock(rbn, filename);
        }
        // END OF INTERNAL BLOCK
    }
    
    return 0;
}

int cut (int length) {
    if (length % 2 == 0) {
        return length/2;
    } else {
        return ((length/2) + 1);
    }
}

/*
    removeRecord() is called from deleteRecord(). It uses mergeBlock() to
	handle underfull blocks.
*/

int removeRecord(Record record, int RBN, fstream& file)
{
	Record tempRecord;
	int key = 0;
	int elementsUsed = 0;
	int order = 0;
	int searchKey = 0;
	int mySearchKey = 0;

	/* load block into memory */
	Block block(RBN, file);
	block.readBlock(RBN, file);
	
	/*set block variables */
	key = BPlusTree.getField("key");
	order = BPlusTree.getField("order");
	elementsUsed = block.getUsed();
	
	/* get position at which to remove record */
	/* this is done by iterative comparison */
	int i = 0;
	for (i = 0; i < elementsUsed; i++)
	{
		tempRecord = block.getChild[i];
		if (key == 3)
		{ //case is ID1
			searchKey = tempRecord.ID1;
			mySearchKey = record.ID1;
			
			if (searchKey == mySearchKey)
			{
				/* we have located the record to delete */
				break;
			}			
			else if (searchKey > mySearchKey)
			{
				/* This element is not found in the tree */
				return 0;
			}
		}
		else if (key == 4)
		{ //case is ID2
			searchKey = tempRecord.ID2;
			mySearchKey = record.ID2;
			
			if (searchKey == mySearchKey)
			{
				/* we have located the record to delete */
				break;
			}
			else if (searchKey > mySearchKey)
			{
				/* This element is not found in the tree */
				return 0;
			}
		}
	}
	
	/* remove child from appropriate place in block */
	/* extra if check: in case the loop is completed, and mySearchKey > searchKey */
	if (searchKey == mySearchKey)
	{
		block.removeRecordAt(i);
	}
	
	/* reload block */
	/* if block is now underfull, time to merge */
	block.readBlock(RBN, file);
	elementsUsed = block.getUsed();
	
	/* calculate min number of records */
	float floatOrder = (float)order;
	float floatElementsUsed = (float)elementsUsed;
	
	if (floatElementsUsed < ((ceil(floatElementsUsed / 2)) - 1))
		mergeBlock(RBN, file);
		
	return 1;	
}

/*
    This is nearly identical to insertRecord, except it calls
	removeRecord() instead of addRecord(). They could be combined,
	and sent a parameter, bool isInsert, or isDelete.
*/

/*
	This pseudo-code is for (key == Last || key == First),
	not ID1 or ID2. This must be also implemented using strings.
*/

/*
	"-->" signifies "calls":
	insertRecord() --> addRecord() --> split()
	deleteRecord() --> removeRecord() --> merge()
	
	insertRecord() and deleteRecord() will locate the block in which
		to insert or delete, then call add/removeRecord. It may send 
		add/removeRecord() a request to add record to a full block.
	addRecord() and removeRecord() will be sent the RBN and handle
		inserting the record into the block. They will utilize split/merge
		in the case of an overfull leaf block OR interior block. (2 functions?)
	split() and merge() will be recursive functions to ensure no blocks are
		overfull or underfull
*/

int deleteRecord(Record record, fstream& file)
{
	int order = 0;
	int key = 0;
	int rootRBN = 0;
	int searchKey = 0;
	int pointer = 0;
	int mySearchKey = 0;
	int RBN = 0;
	
	/* update tree variables */
	order = BPlusTree.getField("order");
	key = BPlusTree.getField("key");
	rootRBN = BPlusTree.getField("rootRBN");
	
	switch (key)
	{
		case 1: mySearchKey = record.Last; break;
		case 2: mySearchKey = record.First; break;
		case 3: mySearchKey = record.ID1; break;
		case 4: mySearchKey = record.ID2; break;
	}
	
	/* load root block into memory */
	Block block(rootRBN, file);
	block.readBlock(rootRBN, file);
	
	/* update block variables */		
	elementsUsed = block.getUsed();
	
	while (!block.isLeaf())
	{
		/* get appropriate searchKey value */
		int i = 0;
		for (i = 0; i < elementsUsed; i++)
		{
			searchKey = block.getKey[i];
			if (searchKey > mySearchKey)
				break;
		}
		
		/* get child RBN associated with appropriate searchKey value */
		pointer = block.getChild[i];
	
		/* read child block (new current block) into memory */
		block.readBlock(pointer, file);
	}
	RBN = block.getRBN();
	removeRecord(record, RBN, file);
	
	return 0;
}


/*
    FUNCTION NOT WORKING, THIS IS SORT OF PSEUDO-CODE!

	This function is called from removeRecord().
	It is sent the RBN of an underfull block.
	Its task is to fix the tree so no blocks are underfull.
*/

int mergeBlock(int RBN, fstream& file)
{
	Record movingRecord;
	Record nextRecord;
	int key = 0;
	int order = 0;
	int left = 0;
	int right = 0;
	int parent = 0;
	bool right = false;
	bool left = false;
	
	oldSearchKey = 0;
	newSearchKey = 0;

	/* set tree variables */
	key = BPlusTree.getField("key");
	order = BPlusTree.getField("order");
	
	/* load block into memory */
	Block block(RBN, file);
	block.readBlock(RBN, file);
	
	/*set block variables */
	elementsUsed = block.getUsed();
	parent = block.getParent();
	
	
	if (block.isLeaf())
	{
		left = block.getLeft();
		right = block.getRight();
		
		/* if we can only use values from one side */
		if (left == -9999 || right == -9999)
		{
			if (left == -9999)
			{
				Block rightBlock(right, file);
				rightBlock.readBlock(right, file);
				float floatUsed = rightBlock.getUsed();
				
				/* if right block has more than minimum number of records */
				if (floatUsed > ((ceil(order / 2) - 1)))
				{
					/* move a record from right block to current block */
					movingRecord = rightBlock.getRecord(0);
					rightBlock.removeRecordAt(0);
					block.putRecordAt(movingRecord, elementsUsed);
				
					/* get next record in right block to copy in the key into parent */
					nextRecord = rightBlock.getRecord(0);
				
					/* get search keys to update keys in interior node */
					if (key == 3)
					{
						oldSearchKey = movingRecord.ID1;
						newSearchKey = nextRecord.ID1;
					}
					else if (key == 4)
					{
						oldSearchKey = movingRecord.ID2;
						newSearchKey = nextRecord.ID2;
					}	
							
					/* if blocks have same parent */
					if (block.getParent() == rightBlock.getParent())
					{
						/* get parent block */
						Block parentBlock(block.getParent(), file);
						parentBlock.readBlock(block.getParent(), file);
						
						int j = 0;
						for (j = 0; j < order -1; j++)
						{
							if (parentBlock.getKey(i) == oldSearchKey)
							{
								parentBlock.setKey(newSearchKey, i);
								break;
							}
						}
					}					
				}
				/* else right block has min number of records, merge */
				else
				{
					/* delete current block and move its parts to right block */
					int j = 0;
				
					/* move all records in block to right block */
					while (block.getUsed() > 0)
					{
						movingRecord = block.getRecord(block.getUsed() - 1);
						block.removeRecordAt(block.getUsed() - 1);
						rightBlock.putRecordAt(movingRecord, 0);
					}
					block.killBlock();
				}
			}
			else if (right == -9999)
			{
				Block leftBlock(left, file);
				leftBlock.readBlock(left, file);
				float floatUsed = leftBlock.getUsed();
				
				/* if left block has more than minimum number of records */
				if (floatUsed > ((ceil(order / 2) - 1)))
				{
					/* move record from left block to current block */
					movingRecord = leftBlock.getRecord(leftBlock.getUsed() - 1);
					leftBlock.removeRecordAt(leftBlock.getUsed() - 1);
					block.putRecordAt(movingRecord, 0);
				
					/* get next record in left block to copy the key into parent */
					nextRecord = leftBlock.getRecord(leftBlock.getUsed() - 1);
				
					/* get search keys to update keys in interior node */
					if (key == 3)
					{
						oldSearchKey = movingRecord.ID1;
						newSearchKey = nextRecord.ID1;
					}
					else if (key == 4)
					{
						oldSearchKey = movingRecord.ID2;
						newSearchKey = nextRecord.ID2;
					}			
					
					/* if blocks have same parent */
					if (block.getParent() == leftBlock.getParent())
					{
						/* get parent block */
						Block parentBlock(block.getParent(), file);
						parentBlock.readBlock(block.getParent(), file);
						
						int j = 0;
						for (j = 0; j < order -1; j++)
						{
							if (parentBlock.getKey(i) == oldSearchKey)
							{
								char *intStr = itoa(newSearchKey);
								string str = string(intStr);
								
								parentBlock.setKey(newSearchKey, i);
								break;
							}
						}
					}
				}
				/* else left block has min number of records, merge */
				else
				{
					/* delete current block and move its parts to left block */
					int j = 0;
				
					/* move all records in block to left block */
					while (block.getUsed() > 0)
					{
						movingRecord = block.getRecord(block.getUsed() - 1);
						block.removeRecordAt(block.getUsed() - 1);
						leftBlock.putRecord(movingRecord);
					}
					block.killBlock();
				}
			}
		}
		
		else
		{
			Block leftBlock(left, file);
			Block rightBlock(right, file);
			leftBlock.readBlock(left, file);
			rightBlock.readBlock(right,file);
			
			/* if both left and right blocks have minimum values */
			if (leftBlock.getUsed() == elementsUsed+1 && rightBlock.getUsed() == elementsUsed+1)
			{
				/* delete current block and move its parts to left block */
				int j = 0;
				
				/* move all records in block to leftBlock */
				while (block.getUsed() > 0)
				{
					movingRecord = block.getRecord(0);
					block.removeRecordAt(0);
					leftBlock.putRecord(movingRecord);
				}
				block.killBlock();
			}
			/* if left block has less values */
			else if (leftBlock.getUsed() < rightBlock.getUsed())
			{
				/* move record from right block to current block */
				movingRecord = rightBlock.getRecord(0);
				rightBlock.removeRecordAt(0);
				block.putRecordAt(movingRecord, elementsUsed);
				
				/* get next record in right block to copy in the key into parent */
				nextRecord = rightBlock.getRecord(0);
				
				/* get search keys to update keys in interior node */
				if (key == 3)
				{
					oldSearchKey = movingRecord.ID1;
					newSearchKey = nextRecord.ID1;
				}
				else if (key == 4)
				{
					oldSearchKey = movingRecord.ID2;
					newSearchKey = nextRecord.ID2;
				}	
						
				/* if blocks have same parent */
				if (block.getParent() == rightBlock.getParent())
				{
					/* get parent block */
					Block parentBlock(block.getParent(), file);
					parentBlock.readBlock(block.getParent(), file);
					
					int j = 0;
					for (j = 0; j < order -1; j++)
					{
						if (parentBlock.getKey(i) == oldSearchKey)
						{
							parentBlock.setKey(newSearchKey, i);
							break;
						}
					}
				}					
			}
			/* else choose left (less or equal values) */
			else
			{
				/* move record from left block to current block */
				movingRecord = leftBlock.getRecord(leftBlock.getUsed() - 1);
				leftBlock.removeRecordAt(leftBlock.getUsed() - 1);
				block.putRecordAt(movingRecord, 0);
				
				/* get next record in left block to copy the key into parent */
				nextRecord = leftBlock.getRecord(leftBlock.getUsed() - 1);
				
				/* get search keys to update keys in interior node */
				if (key == 3)
				{
					oldSearchKey = movingRecord.ID1;
					newSearchKey = nextRecord.ID1;
				}
				else if (key == 4)
				{
					oldSearchKey = movingRecord.ID2;
					newSearchKey = nextRecord.ID2;
				}			
				
				/* if blocks have same parent */
				if (block.getParent() == leftBlock.getParent())
				{
					/* get parent block */
					Block parentBlock(block.getParent(), file);
					parentBlock.readBlock(block.getParent(), file);
					
					int j = 0;
					for (j = 0; j < order -1; j++)
					{
						if (parentBlock.getKey(i) == oldSearchKey)
						{
							parentBlock.setKey(newSearchKey, i);
							break;
						}
					}
				}
			}
		}
	}
	else //block is interior block
	{
		Block parentBlock(parent, file);
		parentBlock.readBlock(parent, file);
		
		Block rightBlock;
		Block leftBlock;
		
		int i = 0;
		for (i = 0; i < order; i++)
		{
			if (parentBlock.getChild[i] == RBN)
			{
				break;
			}
		}
		//i now holds the index of the block to be merged
		
		/* if there is a left/right neighbor, link and load */
		int parentUsed = parentBlock.getUsed();
		if (i == getUsed() - 1)
		{
			rightBlock.read(parentBlock.getChild[i+1], file);
			right = parentBlock.getChild[i+1];
		}			
		else
			right = -9999;
			
		if (i == 0)
		{
			leftBlock.read(parentBlock.getChild[i-1], file);
			left = parentBlock.getchild[i-1];
		}	
		else
			left = -9999;		
	
		/* if we can only use values from one side */
		if (left == -9999 || right == -9999)
		{
			/* if left node is null */
			if (left == -9999)
			{
				float floatUsed = rightBlock.getUsed() + 1;
				
				/* if right block has more than minimum number of keys */
				if (floatUsed > (ceil(order / 2)))
				{
					/* move a child and key from right block to current block */
					movingRBN = rightBlock.getChildAt(0);
					rightBlock.removeChildAt(0);
					
					movingKey = rightBlock.getKey(0);
					rightBlock.removeKeyAt(0);
					
					block.putChild(movingRBN);
					block.putKey(movingKey);
					
					/* get search keys to update keys in interior node */
					int childRBN = block.getChildAt(getUsed());
					Block childBlock(childRBN, file);
					childBlock.read(childRBN, file);					
					
					if (childBlock.isLeaf())
					{
						nextRecord = childBlock.getRecordAt(0);
		
						if (key == 3)
						{
							oldSearchKey = movingKey;
							newSearchKey = nextRecord.ID1;
						}
						else if (key == 4)
						{
							oldSearchKey = movingKey;
							newSearchKey = nextRecord.ID2;
						}
						
						int j = 0;
						for (int j = 0; j < order - 1; j++)
						{
							if (block.getKey(j) == oldSearchKey)
							{
								block.setKey(newSearchKey, j);
								break;
							}
						}
					}
					else
					//childBlock is not a leaf
					{
						oldSearchKey = movingKey;
						newSearchKey = childBlock.getKey(0);
						
						int j = 0;
						for (int j = 0; j < order - 1; j++)
						{
							if (block.getKey(j) == oldSearchKey;
							{
								block.setKey(newSearchKey, j);
								break;
							}
						}
					}										
				}			
				/* else right block has min number of keys, merge */
				else
				{
					/* delete right block and move its parts to current block */
					
					movingRBN = rightBlock.getChildAt(0);
						
					Block rightChildBlock(movingRBN, file);
					rightChildBlock.readBlock(movingRBN, file);
					
					/* get new search key */
					if (rightChildBlock.isLeaf())
					{
						if (key == 3)
							newSearchKey = (rightChildBlock.getRecordAt(0)).ID1;
						else if (key ==4)
							newSearchKey = (rightChildBlock.getRecordAt(0)).ID2;
					}
					else
					{
						newSearchKey = rightChildBlock.getKey(0);
					}
						
					block.putKey(newSearchKey);
				
					/* move all records in block to right block */
					while (rightBlock.getUsed() > 0)
					{
						movingRBN = rightBlock.getChildAt(0);					
						rightBlock.removeChildAt(0);
					
						movingKey = rightBlock.getKey(0);
						rightBlock.removeKeyAt(0);
					
						block.putChild(movingRBN);
						block.putKey(movingKey);
					}
					
					movingRBN = rightBlock.getChildAt(0);
					rightBlock.removeChildAt(0);
					block.putChild(movingRBN);
					
					rightBlock.killBlock();
				}
			}
			
			/* else if right node is null */
			else if (right == -9999)
			{
				float floatUsed = leftBlock.getUsed() + 1;
				
				/* if left block has more than minimum number of keys */
				if (floatUsed > (ceil(order / 2)))
				{
					Block childBlock(block.getChild(0), file);	
					childBlock.readBlock(block.getChild(0), file);					
					oldSearchKey = childBlock.findKey(block.getChild(0), file);
					
					/* move a child and key from left block to current block */
					movingRBN = leftBlock.getChildAt(leftBlock.getUsed());
					leftBlock.removeChildAt(leftBlock.getUsed());
					
					leftBlock.removeKeyAt(leftBlock.getUsed() - 1);
					
					block.putChildAt(movingRBN, 0);
					block.putKeyAt(oldSearchKey, 0);
					
					childBlock.readBlock(block.getChild(0),file);
					newSearchKey = childBlock.findKey(childBlock, file);
					
					/* update keys */
					Block parentBlock(block.getParent(), file);
					parentBlock.readBlock(block.getParent(), file);
					
					int k = 0;
					for (int k = 0; k < order - 1; k++)
					{
						if (parentBlock.getKey(k) == oldSearchKey)
						{
							parentBlock.setKey(newSearchKey, k);
							break;
						}
					}						
				}
				/* else left block has min number of keys, merge */
				else
				{
					/* delete left block and move its parts to current block */
					
					movingRBN = leftBlock.getChildAt(leftBlock.getUsed());
						
					Block leftChildBlock(movingRBN, file);
					leftChildBlock.readBlock(movingRBN, file);
					
					
					newSearchKey = leftChildBlock.findKey(leftChildBlock, file);
					oldSearchKey = block.findKey(block, file);
					
					leftBlock.removeChildAt(leftBlock.getUsed());
					block.putChildAt(movingRBN, 0);
					
					block.putKeyAt(oldSearchKey, 0);
					
					int k = 0;
					for (k = 0; k < order - 1; k++)
					{
						if (parentBlock.getKey(k) == oldSearchKey)
						{
							parentBlock.setKey(newSearchKey, k);
						}
					}
					
					/* move all records in left block to current block */
					while (leftBlock.getUsed() > 0)
					{
						movingRBN = leftBlock.getChildAt(leftBlock.getUsed() - 1);					
						leftBlock.removeChildAt(leftBlock.getUsed() - 1);
					
						movingKey = leftBlock.getKey(getUsed() - 1);
						leftBlock.removeKeyAt(getUsed() - 1);
					
						block.putChildAt(movingRBN, 0);
						block.putKeyAt(movingKey, 0);
					}
					
					leftBlock.killBlock();
				}
			}
		}
		else
		{
			/* if both left and right blocks have minimum values */
			if (leftBlock.getUsed() == elementsUsed+1 && rightBlock.getUsed() == elementsUsed+1)
			{
				/* delete right block and move its parts to current block */
					
				Block rightChildBlock(movingRBN, file);
				rightChildBlock.readBlock(movingRBN, file);
				
				newSearchKey = rightChildBlock.findKey(rightChildBlock, file);
			
				/* move all records in block to right block */
				while (rightBlock.getUsed() > 0)
				{
					movingRBN = rightBlock.getChildAt(0);
					newSearchKey = block.findKey(rightBlock.getChildAt(0), file);
					rightBlock.removeChildAt(0);
				
					movingKey = rightBlock.getKey(0);
					rightBlock.removeKeyAt(0);
				
					block.putChild(movingRBN);
					block.putKey(newSearchKey);
				}
				
				movingRBN = rightBlock.getChildAt(0);
				rightBlock.removeChildAt(0);
				block.putChild(movingRBN);
				
				rightBlock.killBlock();
			}
			/* else if right block has more records than left, choose right */
			else if(leftBlock.getUsed() < rightBlock.getUsed())
			{
				/* move a child and key from right block to current block */
				movingRBN = rightBlock.getChildAt(0);
				rightBlock.removeChildAt(0);
				
				movingKey = rightBlock.getKey(0);
				rightBlock.removeKeyAt(0);
				
				block.putChild(movingRBN);
				block.putKey(movingKey);
				
				/* get search keys to update keys in interior node */
				int childRBN = block.getChildAt(getUsed());
				Block childBlock(childRBN, file);
				childBlock.read(childRBN, file);					
				
				if (childBlock.isLeaf())
				{
					nextRecord = childBlock.getRecordAt(0);
	
					if (key == 3)
					{
						oldSearchKey = movingKey;
						newSearchKey = nextRecord.ID1;
					}
					else if (key == 4)
					{
						oldSearchKey = movingKey;
						newSearchKey = nextRecord.ID2;
					}
					
					int j = 0;
					for (int j = 0; j < order - 1; j++)
					{
						if (block.getKey(j) == oldSearchKey)
						{
							block.setKey(newSearchKey, j);
							break;
						}
					}
				}
				else
				//childBlock is not a leaf
				{
					oldSearchKey = movingKey;
					newSearchKey = childBlock.getKey(0);
					
					int j = 0;
					for (int j = 0; j < order - 1; j++)
					{
						if (block.getKey(j) == oldSearchKey;
						{
							block.setKey(newSearchKey, j);
							break;
						}
					}
				}
			}
			/* else choose left (less or equal values) */
			else
			{
				/* move a child and key from left block to current block */
				movingRBN = leftBlock.getChildAt(leftBlock.getUsed());
				leftBlock.removeChildAt(leftBlock.getUsed());
				
				leftBlock.removeKeyAt(leftBlock.getUsed() - 1);
				
				block.putChildAt(movingRBN, 0);
				block.putKeyAt(oldSearchKey, 0);
				
				childBlock.readBlock(block.getChild(0),file);
				newSearchKey = childBlock.findKey(childBlock, file);
				
				/* update keys */
				Block parentBlock(block.getParent(), file);
				parentBlock.readBlock(block.getParent(), file);
				
				int k = 0;
				for (int k = 0; k < order - 1; k++)
				{
					if (parentBlock.getKey(k) == oldSearchKey)
					{
						parentBlock.setKey(newSearchKey, k);
						break;
					}
				}
			}
		}
	}
}


/*
    This pseudo-code is for (key == Last || key == First),
	not ID1 or ID2. This must be implemented using strings.
*/

/*
	"-->" signifies "calls":
	insertRecord() --> addRecord() --> split()
	deleteRecord() --> removeRecord() --> merge()
	
	insertRecord() and deleteRecord() will locate the block in which
		to insert or delete, then call add/removeRecord. It may send 
		add/removeRecord() a request to add record to a full block.
	addRecord() and removeRecord() will be sent the RBN and handle
		inserting the record into the block. They will utilize split/merge
		in the case of an overfull leaf block OR interior block. (2 functions?)
	split() and merge() will be recursive functions to ensure no blocks are
		overfull or underfull
*/

int insertRecord(Record record, fstream& file)
{
	int order = 0;
	int key = 0;
	int rootRBN = 0;
	int searchKey = 0;
	int pointer = 0;
	int mySearchKey = 0;
	int RBN = 0;
	
	/* update tree variables */
	order = BPlusTree.getField("order");
	key = BPlusTree.getField("key");
	rootRBN = BPlusTree.getField("rootRBN");
	
	switch (key)
	{
		case 1: mySearchKey = record.Last; break;
		case 2: mySearchKey = record.First; break;
		case 3: mySearchKey = record.ID1; break;
		case 4: mySearchKey = record.ID2; break;
	}
	
	/* load root block into memory */
	Block block(rootRBN, file);
	block.readBlock(rootRBN, file);
	
	/* update block variables */		
	elementsUsed = block.getUsed();
	
	while (!block.isLeaf())
	{
		/* get appropriate searchKey value */
		int i = 0;
		for (i = 0; i < elementsUsed; i++)
		{
			searchKey = block.getKey[i];
			if (searchKey > mySearchKey)
				break;
		}
		
		/* get child RBN associated with appropriate searchKey value */
		pointer = block.getChild[i];
	
		/* read child block (new current block) into memory */
		block.readBlock(pointer, file);
	}
	RBN = block.getRBN();
	addRecord(record, RBN, file);
	
	return 0;
}
template class BPlusTree<string>;







