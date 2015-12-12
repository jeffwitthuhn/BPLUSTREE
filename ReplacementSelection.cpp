/**
	@file ReplacementSelection.cpp
	@Implementation of ReplacementSelection methods
	@authors Jeff Witthuhn, Ross Kostron, Subodh Bhattarai, Andrew Morris, Alex Gatzke

**/
	

#include "ReplacementSelection.h"
#include <string>
#include "KeyPos.h"
#include <iostream>

template <class obj>
ReplacementSelection<obj>::ReplacementSelection(const int _size, istream& _infile,
				ostream& _outfile,  const bool _sort)
{
		size=_size;
		ascending=_sort;
		activeLeft=true;

		leftStart=0;
		leftEnd=0;
		rightStart=size-1;
		rightEnd=size;
		heap.resize(size);
		sortPreMerge(_infile, _outfile);
		//cout<<"constructor\n";

}
template <class obj>
obj ReplacementSelection<obj>::fill(istream& infile)
{
	obj newelement; //The new element
	int i; //Counter 
	int newIndex; // The new index
	infile>>newelement; 
	for(i=0; i<size && !infile.eof(); i++){
		heap[i]=newelement;
		leftEnd=i;
		heapify(true);
		infile>>newelement;

	}
	//cout<<"filled\n";
	return newelement;
}

template <class obj>
void ReplacementSelection<obj>::sortPreMerge(std::istream& infile,std::ostream& outfile)
{
	vector<obj> currentList; //The current list
	obj newelement; //The new element
	obj lastElement; //The last element
	bool pendingFull=false; //is the pending heap full
	bool currentHeapEligible; //is the current heap eligible
	bool endOfFile=false; //are you at the end of file
	
	//while replacement selection vector is not full, heapify a new element
	newelement = fill(infile);
	//while there is a new element
	while(!endOfFile){
		//Pop the root of the current heap, adding it to the current list.
		if(infile.eof()){
			endOfFile=true;
		}
		lastElement=pop();
		currentList.push_back(lastElement);
		if(ascending){
			if(newelement>=lastElement){
				currentHeapEligible=true;
			}
			else{
				currentHeapEligible=false;
			}
		}
		else {
			if(newelement<=lastElement){
				currentHeapEligible=true;
			}
			else{
				currentHeapEligible=false;
			}
		}
		//If the new element is greater than or equal to the last element popped, push it onto the current heap
		if(currentHeapEligible){
					//cout<<"push current:"<<newelement<<endl;
					current_heap_push(newelement);
				}
		//If the new element is less than (for a min-heap) to the last element popped,
		else {
			//push it onto the pending heap
			//cout<<"push pending:"<<newelement<<endl;
			pending_heap_push(newelement);
			if(activeLeft){
				if(leftStart>leftEnd){
					pendingFull=true;
				}
				else{
				}
			}
			else {
				if(rightStart<rightEnd){
					pendingFull=true;
				}
				else {
				}
			}

			if(pendingFull){
				//cout<<"pending now active\n";
				for(int j=0; j<currentList.size(); j++)
					outfile<<currentList[j]<<" ";
				outfile<<" |";
				outfile<<"\n";
				currentList.resize(0);
				toggleActive();
				pendingFull=false;
			}
		}

		infile>>newelement;
	}
	if(activeLeft){
		while(leftStart<=leftEnd){
			currentList.push_back(pop());
			leftEnd--;

		}
	}
	else {
		while(rightStart>=rightEnd){
			currentList.push_back(pop());
			rightEnd++;

		}
	}
	//cout<<"finish off\n";
	for(int j=0; j<currentList.size(); j++)
		outfile<<currentList[j]<<" ";
	outfile<<" |";
	outfile<<"\n";
	currentList.resize(0);
	toggleActive();
	if(activeLeft){
		while(leftStart<=leftEnd){
			currentList.push_back(pop());
			leftEnd--;

		}
	}
	else {
		while(rightStart>=rightEnd){
			currentList.push_back(pop());
			rightEnd++;

		}
	}
	for(int j=0; j<currentList.size(); j++)
		outfile<<currentList[j]<<" ";
	outfile<<" |";
	outfile<<"\n";
	
}

template <class obj>
void ReplacementSelection<obj>::heapify(bool leftSide)
{
	int index; // The index
	if(leftSide){
		index=parent(leftEnd, leftSide);
		while(index>=leftStart){
			siftDown(index, leftSide);
			index-=1;
		}
	}
	else {
		index=parent(rightEnd,leftSide);
		while(index<=rightStart){
			siftDown(index,leftSide);
			index+=1;
		}
	}
}

template <class obj>
void ReplacementSelection<obj>::siftUp(int index, bool leftSide)
{
	int p=parent(index, leftSide); // The parent
	int toSwap = index; // index to swap
	if(leftSide&&ascending){
		if((p>=leftStart)&&(heap[index]<heap[p]))
			toSwap=p;
	}
	else if(leftSide){//&&!ascending
		if((p>=leftStart)&&(heap[index]>heap[p]))
			toSwap=p;
	}
	else if(ascending){//&&!left
		if((p<=rightStart)&&(heap[index]<heap[p]))
			toSwap=p;
	}
	else {// !ascending&&!left
		if((p<=rightStart)&&(heap[index]>heap[p]))
			toSwap=p;
	}
	if(toSwap!=index){
		swap(index,toSwap);
		siftUp(toSwap,leftSide);
	}
}

template <class obj>
void ReplacementSelection<obj>::siftDown(int index, bool leftSide)
{
	int l; // left index
	int r; // right index
	int toSwap; // index to swap
	l=left(index,leftSide);
	r=right(index,leftSide);
	if(leftSide&&ascending){
		if(l<=leftEnd&&heap[l]<heap[index])
			toSwap=l;
		else 
			toSwap=index;
		if(r<=leftEnd&&heap[r]<heap[toSwap])
			toSwap=r;
	}
	else if(leftSide){// &&!ascending
		if(l<=leftEnd&&heap[l]>heap[index])
			toSwap=l;
		else 
			toSwap=index;
		if(r<=leftEnd&&heap[r]>heap[toSwap])
			toSwap=r;
	}
	else if(ascending){// &&!left
		if(l>=rightEnd&&heap[l]<heap[index])
			toSwap=l;
		else 
			toSwap=index;
		if(r>=rightEnd&&heap[r]<heap[toSwap])
			toSwap=r;
	}
	else {// !ascending&&!left
		if(l>=rightEnd&&heap[l]>heap[index])
			toSwap=l;
		else 
			toSwap=index;
		if(r>=rightEnd&&heap[r]>heap[toSwap])
			toSwap=r;
	}
	if(toSwap!=index){
		swap(index,toSwap);
		siftDown(toSwap,leftSide);
	}
}

template <class obj>
obj ReplacementSelection<obj>::pop()
{
	obj out; //object to pop
	if(activeLeft){
		out=heap[leftStart];
		swap(leftStart,leftEnd);
		leftEnd--;
		siftDown(leftStart,activeLeft);
		leftEnd++;
	}
	else{
		out=heap[rightStart];
		swap(rightStart,rightEnd);
		rightEnd++;
		siftDown(rightStart, activeLeft);
		rightEnd--;
	}
	return out;
} 

template <class obj>
void ReplacementSelection<obj>::current_heap_push(obj entry)
{
	if(activeLeft){
		heap[leftEnd]=entry;
		siftUp(leftEnd,activeLeft);
	}
	else{
		heap[rightEnd]=entry;
		siftUp(rightEnd,activeLeft);
	}
}

template <class obj>
void ReplacementSelection<obj>::pending_heap_push(obj entry)
{
	if(activeLeft){
		leftEnd--;
		rightEnd--;
		heap[rightEnd]=entry;
		siftUp(rightEnd,!activeLeft);
	}
	else{
		leftEnd++;
		rightEnd++;
		heap[leftEnd]=entry;
		siftUp(leftEnd,!activeLeft);
	}
}

template <class obj>
void ReplacementSelection<obj>::toggleActive()
{
	activeLeft=!activeLeft;
}
template <class obj>
void ReplacementSelection<obj>::swap(int index1, int index2)
{
	obj temp; // temp object
	temp=heap[index1];
	heap[index1]=heap[index2];
	heap[index2]=temp;
}

template <class obj>
int ReplacementSelection<obj>::left(int index, bool leftSide)
{
	return (leftSide? 2*index +1 : rightStart-(rightStart-index)*2-1);
}
template <class obj>
int ReplacementSelection<obj>::right(int index, bool leftSide)
{
	return (leftSide? 2*index +2 : rightStart-(rightStart-index)*2-2);
}
template <class obj>
int ReplacementSelection<obj>::parent(int index, bool leftSide)
{
	return (leftSide? (index-1)/2 : (index-rightStart+1)/2+rightStart);
}

template <class obj>
void ReplacementSelection<obj>::printHeap()
{
		if(activeLeft){
			cout<<"active:("<<leftStart<<", "<<leftEnd<<") ";
			for(int i=leftStart; i<=leftEnd; i++)
				cout<<heap[i]<<" ";
			cout<<"  pending:("<<rightStart<<", "<<rightEnd<<") ";
			for(int i=rightStart; i>=rightEnd; i--)
				cout<<heap[i]<<" ";
			cout<<"\n";
			}
		else{
			cout<<"active:("<<rightStart<<", "<<rightEnd<<") ";
			for(int i=rightStart; i>=rightEnd; i--)
				cout<<heap[i]<<" ";
			cout<<"  pending:("<<leftStart<<", "<<leftEnd<<") ";
			for(int i=leftStart; i<=leftEnd; i++)
				cout<<heap[i]<<" ";
			cout<<"\n";
		}
}

/** 
	allows ints to be used with the class
*/
template class ReplacementSelection<int>;

/** 
	allows strings to be used with the class
*/
template class ReplacementSelection<string>;

/** 
	allows doubles to be used with the class
*/
template class ReplacementSelection<double>;

/** 
	allows floats to be used with the class
*/
template class ReplacementSelection<float>;

/** 
	allows keypos<string> to be used with the class
*/
template class ReplacementSelection<KeyPos<string> >;

/** 
	allows keypos<int> to be used with the class
*/
template class ReplacementSelection<KeyPos<int> >;