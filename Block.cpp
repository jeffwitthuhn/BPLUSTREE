/**
	@file Block.cpp
	Implementation file for the Block class
	@authors Jeff Witthuhn, Ross Kostron, Subodh Bhattarai, Andrew Morris, Alex Gatzke
	\details compile g++ -c Block.cpp -std=c++11
*/

#include "Block.h"
int Block::order=0;
int Block::recordSize=0;
int Block::headerSize=0;
int Block::key=0;
int Block::leafBlockSize=0;
int Block::interiorBlockSize=0;
int Block::difference=0;
int Block::blockSize=0;
int Block::initialFill=0;

Block::Block (int _RBN)
{//testing function
		RBN= _RBN;
		isLeaf=false;
		isRoot=false;
		numChildren=0;
		elementsUsed=0;
		parent=RBN;
		recordList.resize(0);
		keyList.resize(0);
		childrenList.resize(0);	
		putKey("1");
		putKey("2");
		putKey("3");
		putChild(1);
		putChild(2);
		putChild(3);
		putChild(4);

	}
Block::Block(bool _isRoot, bool _isLeaf, int _RBN)
{//for creating a new node
		isLeaf=_isLeaf;
		isRoot=_isRoot;
		RBN=_RBN;
		elementsUsed=0;
		numChildren=0;
		recordList.resize(0);
		keyList.resize(0);
		childrenList.resize(0);

	}
Block::Block()
{
		elementsUsed=0;
		numChildren=0;
		recordList.resize(0);
		keyList.resize(0);
		childrenList.resize(0);
}
string Block::getLargestKey()
{
		assert(elementsUsed>0&&("no elements in block with RBN:"));
		string largest;
		string temp;
		if(isLeaf){
			largest =KeyFromRecord(recordList[0]);
			for(int i=0; i<elementsUsed;i++){
				temp=KeyFromRecord(recordList[i]);
				if(temp>largest) 
					largest=temp;
			}
		}
		else{
			largest=keyList[0];
			for(int i=0; i<elementsUsed;i++){
				if(keyList[i]>largest){
					largest=keyList[i];
				}
			}
		}
	return largest;
}


int Block::getRBNat(int nth_RBN)
{
    return childrenList.at(nth_RBN);
}

string Block::getSmallestKey()
{	

	     //ANDREW ADDED THIS
        string temp;
	 if(isLeaf)   
	    switch (key) {

	        case 1:
	            return recordList.at(0).Last;
	        case 2:
	            return recordList.at(0).First;
	        case 3:
	            temp = to_string(recordList.at(0).ID1);//-std:c++11
	            return temp;
	        case 4:
	            temp = to_string(recordList.at(0).ID2);
	            return temp;
	    }
	    else {
	    	return getKey(0);
	    }
    return "NO KEY";
}

string Block::KeyFromRecord(Record r)
{
		switch(key){
			case 1: return (r.Last); break;
			case 2: return (r.First); break;
			case 3: return to_string(r.ID1); break;
			case 4: return to_string(r.ID2); break;
		}
    return "NO KEY...";
	}
void Block::writeBlock(ostream& out)
{//1 99, 
		out.seekp(89+RBN*blockSize);
		if(isLeaf){
			out<<"\n"
			<<(isRoot?1:0)<<" "<<(isLeaf?1:0)<<" "
			<<(elementsUsed<10?"0":"")<<elementsUsed<<" "
			<<setfill('0')<<setw(5)<<left<<" "
			<<setfill('0')<<setw(5)<<right<<" "
			<<setfill('0')<<setw(5)<<parent<<" ";
			for(int i=0; i<elementsUsed; i++){
				out
				<<setfill(' ')<<setw(10)<<recordList[i].Last<<" "
				<<setfill(' ')<<setw(10)<<recordList[i].First<<" "
				<<setfill('0')<<setw(10)<<recordList[i].ID1<<" "
				<<setfill('0')<<setw(10)<<recordList[i].ID2<<" | ";
			}
			for(int i=0; i<(Block::order-1-elementsUsed);i++){
				for(int j=0; j<(recordSize-3); j++)
					out<<'%';
				out<<" | ";
			}
			if(leafBlockSize<interiorBlockSize){
				for(int i=0; i<difference;i++){
					out<<"#";
				}
			}
			out<<"!";
		}
		else{
			assert(checkKeyRRN()&&"make sure an interior node has keys+1 children");
			out<<"\n"
			<<(isRoot?1:0)<<" "<<(isLeaf?1:0)<<" "
			<<(elementsUsed<10?"0":"")<<elementsUsed<<" "
			<<setfill('0')<<setw(5)<<parent<<" ";
			for(int i=0; i<elementsUsed; i++){
				out<<setfill(' ')<<setw(10)<<keyList[i]<<" ";
 			}
			for(int i=0; i<(Block::order-1-elementsUsed);i++){
				for(int j=0; j<10; j++)
					out<<'%';
				out<<" ";
			}
			out<<"| ";
			for(int i=0; i<numChildren;i++){
				out<<setfill('0')<<setw(5)<<childrenList[i]<<" ";
			}
			for(int i=0; i<Block::order-numChildren;i++){
				for(int j=0; j<5; j++)
					out<<'%';
				out<<" ";
			}
			if(leafBlockSize>interiorBlockSize){
				for(int i=0; i<difference;i++){
					out<<"#";
				}
			}
			out<< "!";
			

		}
	}
void Block::readBlock(int _RBN, iostream& file)
{
		RBN=_RBN; 
		recordList.resize(0);
		keyList.resize(0);
		childrenList.resize(0);
		file.seekg(89+RBN*blockSize);
		int _isRoot, _isLeaf;
		file>>_isRoot>>_isLeaf>>elementsUsed;
    /*cout << "root = " << _isRoot << endl;
    cout << "leaf = " << _isLeaf << endl;
    if (_isRoot == 0) {
        cout << "_isRoot ZEERO" << endl;
    }
    if (_isLeaf == 0) {
        cout << "_isLeaf ZEERO" << endl;
    }*/
		isRoot=( _isRoot == 0 ?false:true);
		isLeaf=( _isLeaf == 0 ?false:true);
    /*cout << ((isRoot)?"IS A ROOT" :"" ) << endl;
    cout << ((isLeaf)? "IS A LEAF":"" ) << endl;
    */

		if(isLeaf){
			Record _record;
			string bar;
			//read left
			file>>left;
			//read right
			file>>right;
			//read parent
			file>>parent;
			//for elements used
			//	read last, first, id1, id2, |
			for(int i=0; i<elementsUsed; i++){
				file>>_record.Last>>_record.First>>_record.ID1>>_record.ID2>>bar;
				recordList.push_back(_record);
			}
			//done
		}
		else{
			string _key; 
			int _child;
			//read parent
			file>>parent;
            cout << "read interior, parent = " << parent << endl;
			//for elements used
			//	read key
			for(int i=0; i<elementsUsed;i++){
				file>>_key;
				keyList.push_back(_key);
            }
			//for Block::order -1 -elements used
			//	eat %%%
			for(int i=0; i<Block::order-1-elementsUsed;i++)
				file>>_key;
			//eat |
			file>>_key;
			//for elements used +1
			// read child
			for(int i=0; i<elementsUsed +1;i++){
				file>>_child;
				childrenList.push_back(_child);
			}
			numChildren=elementsUsed+1;
			//done
			

		}

}
