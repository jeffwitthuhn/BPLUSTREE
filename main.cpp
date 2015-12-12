/**
	* @file BPlusTree.cpp
	* implementation file for B+ tree
	* @authors Jeff Witthuhn, Ross Kostron, Subodh Bhattarai, Andrew Morris, Alex Gatzke

*/
#include <iostream>
#include "BPlusTree.h"
#include <cctype>
#include <string>
using namespace std;
void printMenu(bool open){
	cout<<"\nO: open a designated database file\n";
	cout<<"N: create a new database file from designated record file\n";
	cout<<"   choose the key [1,2,3,4] and order (order > 2) for the\n";
	cout<<"   database.\n";
	cout<<"T: testopen on Records.txt, tree.txt\n";

	if(open){
		cout<<"P: print the B+ tree to terminal\n";
		cout<<"S: search for a record by designating a key in the open database\n";
		cout<<"R: remove an item designated by a key from the open database\n";
		cout<<"X: remake the currently opened database into a designated database file\n";
		cout<<"   choose an order and a key for the new database file.\n";
	}
}
int main(){
	int key=3;
	int order;
	bool open = false;
	string treeFileName;
	string originalFileName;
	//cout<<"enter key number:";
	//cin>>key;
	char input; 
	BPlusTree<string> tree;
	while(1){
		printMenu(open);
		cin>>input;
		input=tolower(input);
		switch(input){
			case 'o':
				cout<<"enter Tree File Name:";
				cin>>treeFileName;
				tree.open(treeFileName);
				open=true;
				break;
			case 'n':
				cout<<"enter data file name:";
				cin>>originalFileName;
				cout<<"enter tree file name:";
				cin>>treeFileName;
				cout<<"enter order (order > 2): ";
				cin>>order;
				cout<<"enter key [1,2,3,4]";
				cin>>key;
				tree.create(originalFileName,treeFileName,key,order);
				break;
			case 't': 
				originalFileName="Records.txt";
				treeFileName="tree.txt";
				tree.create(originalFileName, treeFileName);
				tree.open(treeFileName);
				open=true;
				break;
			case 'p':if(open) tree.Print(cout);
				break;
			case 's':
				break;
			case 'r':
				break;
			case 'x':
				break;


		}


	}

}