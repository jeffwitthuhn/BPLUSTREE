
/**
    * @file fileOperations.h
    * file operations for the B+ tree
    * @authors Jeff Witthuhn, Ross Kostron, Subodh Bhattarai, Andrew Morris, Alex Gatzke
    * extract keys and positions from record file to key and position file. Sort the record file from the key and position file.

*/
#ifndef FILEOP
#define FILEOP
#include <string>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <math.h>
#include "Block.h"
    
using namespace std;

/**
	The getfield function gets a field of the header record
	\param field the field
	\param outfile the output stream
	\return the field in the header record
*/
int getField(string field, istream& outfile)
{
    int value = 0;
    int switchField = 0;

    //convert field to int
    if (field.compare("headerSize") == 0 || field.compare("HS") == 0)
        switchField = 1;
    else if (field.compare("order") == 0 || field.compare("OR") == 0)
        switchField = 2;
    else if (field.compare("key") == 0 || field.compare("KY") == 0)
        switchField = 3;
    else if (field.compare("firstRBN") == 0 || field.compare("FR") == 0)
        switchField = 4;
    else if (field.compare("lastRBN") == 0 || field.compare("LR") == 0)
        switchField = 5;
    else if (field.compare("rootRBN") == 0 || field.compare("RR") == 0)
        switchField = 6;
    else if (field.compare("RBNsize") == 0 || field.compare("RS") == 0)
        switchField = 7;
    else if (field.compare("fieldSize") == 0 || field.compare("FS") == 0)
        switchField = 8;
    else if (field.compare("keySize") == 0 || field.compare("KS") == 0)
        switchField = 9;
    else if (field.compare("blockSize") == 0 || field.compare("BS") == 0)
        switchField = 10;


    switch (switchField)
    {
    case 1:
        outfile.seekg(3);
        outfile >> value;
        break;
    case 2:
        outfile.seekg(12);
        outfile >> value;
        break;
    case 3:
        outfile.seekg(21);
        outfile >> value;
        break;
    case 4:
        outfile.seekg(30);
        outfile >> value;
        break;
    case 5:
        outfile.seekg(39);
        outfile >> value;
        break;
    case 6:
        outfile.seekg(48);
        outfile >> value;
        break;
    case 7:
        outfile.seekg(57);
        outfile >> value;
        break;
    case 8:
        outfile.seekg(66);
        outfile >> value;
        break;
    case 9:
        outfile.seekg(75);
        outfile >> value;
        break;
    case 10:
        outfile.seekg(84);
        outfile >> value;
        break;
    }
    return value;
}
/**
	The setfield function sets fields of the header record
	\param field the field
	\param value the value
	\param outfile the output stream
	\return nothing
	\post the fields of the header record are set
*/
void setField(string field, int value, ostream& outfile)
{
    int switchField = 0;

    //convert field to int
    if (field.compare("headerSize") == 0 || field.compare("HS") == 0)
        switchField = 1;
    else if (field.compare("order") == 0 || field.compare("OR") == 0)
        switchField = 2;
    else if (field.compare("key") == 0 || field.compare("KY") == 0)
        switchField = 3;
    else if (field.compare("firstRBN") == 0 || field.compare("FR") == 0)
        switchField = 4;
    else if (field.compare("lastRBN") == 0 || field.compare("LR") == 0)
        switchField = 5;
    else if (field.compare("rootRBN") == 0 || field.compare("RR") == 0)
        switchField = 6;
    else if (field.compare("RBNsize") == 0 || field.compare("RS") == 0)
        switchField = 7;
    else if (field.compare("fieldSize") == 0 || field.compare("FS") == 0)
        switchField = 8;
    else if (field.compare("keySize") == 0 || field.compare("KS") == 0)
        switchField = 9;
    else if (field.compare("blockSize") == 0 || field.compare("BS") == 0)
        switchField = 10;


    switch (switchField) {
        case 1:
            outfile.seekp(3);
            outfile << setfill('0') << setw(5) << value;
            break;
        case 2:
            outfile.seekp(12);
            outfile << setfill('0') << setw(5) << value;
            break;
        case 3:
            outfile.seekp(21);
            outfile << setfill('0') << setw(5) << value;
            break;
        case 4:
            outfile.seekp(30);
            outfile << setfill('0') << setw(5) << value;
            break;
        case 5:
            outfile.seekp(39);
            outfile << setfill('0') << setw(5) << value;
            break;
        case 6:
            outfile.seekp(48);
            outfile << setfill('0') << setw(5) << value;
            break;
        case 7:
            outfile.seekp(57);
            outfile << setfill('0') << setw(5) << value;
            break;
        case 8:
            outfile.seekp(66);
            outfile << setfill('0') << setw(5) << value;
            break;
        case 9:
            outfile.seekp(75);
            outfile << setfill('0') << setw(5) << value;
            break;
        case 10:
            outfile.seekp(84);
            outfile << setfill('0') << setw(5) << value;
            break;
    }
}

/**
	The extract key function
	\param inputfile the input file
	\param outputfile the output file
	\param Key the key
	\return nothing
	\post the keys are extracted
*/
void extractKeys(string inputfile,string outputfile, int Key){
	int NAMESIZE=80;

    char InputFileName[NAMESIZE + 1];
    char OutputFileName[NAMESIZE + 1];
    ifstream infile;
    ofstream outfile;
    streampos position;
    int ID1, ID2;
    string Last, First;
    strncpy(InputFileName, inputfile.c_str(), sizeof(InputFileName));
	InputFileName[sizeof(InputFileName) - 1] = 0;
	strncpy(OutputFileName, outputfile.c_str(), sizeof(OutputFileName));
	OutputFileName[sizeof(OutputFileName) - 1] = 0;
    infile.open(InputFileName);
    outfile.open(OutputFileName);
    cout<<"file read, inputfile/outputfile:"<<InputFileName<<" "<<OutputFileName<<endl;
    //Process File
    position = infile.tellg();

    infile >> Last >> First >> ID1 >> ID2;
    cout<<"last, first, id1, id2:"<<Last<<" "<<First<<" "<<ID1<<" "<<ID2;
    // While !eof file
    int ReadabilityCounter = 0;
    while ( !infile.eof() ) {

        cout << position << " " << Last << " " << First;
        cout << " " << ID1 << " " << ID2 << endl;

        //if first letter of last name == '*', skip
        //else output "Position Key "
        if (Last.at(0) == '*') {
            cout << "Deleted record, did not output" << endl;
            cout << position << " " << Last << " " << First;
            cout << " " << ID1 << " " << ID2 << endl;
        } else {

            outfile << position; //Regardless of case, we need the position.
            switch (Key){
                case 1: { //Last Name Key
                outfile << " " << Last << " ";
                    break;
                }
                case 2: { // First name Key
                outfile << " " << First << " ";
                    break;
                }
                case 3: { // ID1 Key
                outfile << " " << ID1 << " ";
                    break;
                }
                case 4: { //ID2 Key
                outfile << " " << ID2 << " ";
                    break;
                }

            }
            //Simply creates line breaks for every 10 records
            //for readability                       
            ++ReadabilityCounter;                   
            if ( (ReadabilityCounter % 10) == 0) {  
                outfile << endl;                    
            }                                       
        }


        //get new record;
        //get current position
        infile.ignore(3, '\n');
        position = infile.tellg();
        //Read in line, assign values
        infile >> Last >> First >> ID1 >> ID2;


    }
}

/**
	The rewrite record from key function
	\param originalrecordfile the original record file
	\param keyfile the key file
	\return nothing
	\post records are rewrote from keys
*/
void rewriteRecordFromKey(string originalrecordfile, string keyfile){
	int NAMESIZE=80;
    char RecordName[NAMESIZE + 1];
    strncpy(RecordName, originalrecordfile.c_str(), sizeof(RecordName));
	RecordName[sizeof(RecordName) - 1] = 0;
    char KeyName[NAMESIZE + 1];
    strncpy(KeyName, keyfile.c_str(), sizeof(KeyName));
	KeyName[sizeof(KeyName) - 1] = 0;
    char renameTemp[NAMESIZE + 1];
    strncpy(renameTemp, ("sorted"+originalrecordfile).c_str(), sizeof(renameTemp));
    char TempName[] = "temp.dat";


    int oldRecordPos;

    string garbage; // The key value does not matter, we know the positions
                    // of the records in the file. Upon building B+ tree, extracting
                    // the key values will be trivial.

    fstream RecordFile;
    fstream KeyFile;
    fstream tempFile;

    Record record;

    //Open the files needed for the rewriting operations
    RecordFile.open(RecordName, std::fstream::in | std::fstream::out);
    KeyFile.open(KeyName, std::fstream::in);
    tempFile.open(TempName, std::fstream::out);

    //Initialize the first read from the KeyFile.
    KeyFile >> oldRecordPos >> garbage;
    RecordFile.seekp(oldRecordPos);

    RecordFile >> record.Last >> record.First >> record.ID1 >> record.ID2;

    while ( !KeyFile.eof()) {
    	//cout<<"5";

        tempFile << record.Last << " " << record.First << " " << record.ID1 << " " << record.ID2 << " |\n";
        cout<<"hi";
        KeyFile >> oldRecordPos >> garbage;
        cout<<oldRecordPos<<" "<<garbage;
        RecordFile.seekp(oldRecordPos);
        RecordFile >> record.Last >> record.First >> record.ID1 >> record.ID2;
    }

    RecordFile.close();
    KeyFile.close();
    tempFile.close();
    cout<<"\nrename: "<<TempName<<"\nTo: "<<renameTemp<<endl;
    rename(TempName, renameTemp);

}

/**
	Initialize the sequence set
	\param recordFileName the record file
	\param treeFileName the tree file
	\param _order the order
	\param _key the key
	\return nothing
	\post the sequence set is initialized
*/
void initSequenceSet(string recordFileName, string treeFileName,int _order,int _key){
    Block::initBlockClass(_order,_key);
    cout<<"initialized block class (order, key), ("<<(Block::order)<<", "<<(Block::key)<<")\n";
    string inputString;
    int inputInt;
    int i = 0;
    bool markedLast=false;

    Record record;
    ifstream infile(recordFileName);
    fstream outfile(treeFileName, std::fstream::in | std::fstream::out | std::fstream::trunc );

    cout<<"writing to file: "<<treeFileName<<endl;
    cout<<"reading from file: "<<recordFileName<<endl;


 	//This will be used for now, in the future, the block below 
    int headerSize = 89;
    int order = Block::order;
    int key = Block::key;
    int firstRBN = 0;
    int lastRBN = 0;
    int rootRBN = 0;
    int RBNsize = 5;
    int fieldSize = 10;
    int keySize = 10;
    int blockSize = Block::blockSize;
    outfile << "HS " << setfill('0') << setw(5) << headerSize
            << " OR " << setfill('0') << setw(5) << order
            << " KY " << setfill('0') << setw(5) << key
            << " FR " << setfill('0') << setw(5) << firstRBN
            << " LR " << setfill('0') << setw(5) << lastRBN
            << " RR " << setfill('0') << setw(5) << rootRBN
            << " RS " << setfill('0') << setw(5) << RBNsize
            << " FS " << setfill('0') << setw(5) << fieldSize
            << " KS " << setfill('0') << setw(5) << keySize
            << " BS " << setfill('0') << setw(5) << blockSize;


    while(!infile.eof())
    {
        //block counter
         Block block(false, true, i);
         int recordCount=0;
        //for each space in the block, put a record in
        //or 3/4 or whatever
        for (float j = 0; j < Block::initialFill; j++)
        {
            infile >> inputString;
            record.Last = inputString;

            infile >> inputString;
            record.First = inputString;

            infile >> inputInt;
            record.ID1 = inputInt;

            infile >> inputInt;
            record.ID2 = inputInt;
            if (infile.eof())
                break;
            block.putRecord(record);
            recordCount++;

            //eat vertical bar
            infile >> inputString;

            //check for end of file
           
        }

        if (infile.eof()){
            block.setRight(-9999);
            cout<<"marked last\n";
            block.writeBlock(cout);
            cout<<"i:"<<i<<" \n";
            if (block.getUsed()>0){
                markedLast=true;
            }

        }
        else
            block.setRight(i+1);

        if (i == 0)
            block.setLeft(-9999);
        else
            block.setLeft(i-1);

        block.setParent(-9999);
         //cout<<"recordCount "<<recordCount<<endl;
         //cout<<"order/2 "<<order/2<<endl;
        // cout<<"i "<<i<<endl;
        if(recordCount<order/2){
            Block previous;
           // cout<<"recordCount < order/2 at pass: "<<i<<endl;

         while(recordCount>0){
                recordCount= recordCount-1;   
                int prevRRN = block.getLeft();
                previous.readBlock(prevRRN,outfile);
                previous.putRecord(block.getRecord(recordCount));
                if(prevRRN == block.getLeft())
                    previous.setRight(-9999);

                previous.writeBlock(outfile);

                cout<<recordCount<<endl;
            }
          }
        else{
           // cout<<"recordCount >= order/2 at pass: "<<i<<endl;
            block.writeBlock(outfile);
            cout<<"writing block\n\n\n";
        }
        //block.writeBlock(cout);
        i++;
    }
    if(!markedLast){
        cout<<"***mark last";
        Block previous; 
        previous.readBlock(i-2,outfile);
        previous.setRight(-9999);
        previous.writeBlock(outfile);
        previous.writeBlock(cout);
    }
    infile.close();
    outfile.close();
}

/**
	The find key function
	\param rightPointer the right pointer
	\param file the file
	\return the key
*/
string findKey(int rightPointer, iostream& file) {

    Block kid;

    kid.readBlock(rightPointer, file);

    while (!kid.leaf()){
        kid.readBlock( kid.getChild(0), file );
    }
    return kid.getSmallestKey();


}
/**
 * @long TO INITIALIZE TREE
 * 1. Read over all of the populated data blocks
 * 2. get a count of the amount of blocks, determine how many internal nodes needed.
 *      -How to calculate?
 * 3. for each level, besides root, populate the nodes.
 * 4. create the root
 * 5. Update header file with root location.
 *
 * preconditions :
 * @post A B+ tree file has been created
 * @post Header information is known/retrievable.
 * @post the sequence set has been built, and links between nodes is established
 *       via left and right RBNs.
 *
 * @post : A B+ Tree will be initialized, and the root location value in the header has been update.

	\param treeFileName the tree file
	\return nothing
*/
void buildFromSS(string treeFileName){
    cout << "Initializing" << endl;
    fstream file(treeFileName, std::fstream::in | std::fstream::out );
    int nextRBN = 0;
    Block blockStats;     //NOT USED FOR ALL BLOCKS!!!! A container in case we need it.
    int header_size = blockStats.headerSize;        //header_size will be passed in from the header.
    int block_size = blockStats.blockSize; 
    int fillpercent=(int) ceil((blockStats.order -1) * 0.75);
    int minfill=(int) ceil(Block::order/2);
    int recs_per_node = (fillpercent>minfill?fillpercent:minfill);
    int levels = 0;
    int Key;   
    int num_of_leaves = 0;      //Number of leaves
    int extra_children=0;
    int num_of_parents = 0;     //Number of parents to generate for a given level

    file.seekg(header_size);
    Block temp(false, true, 0);
    cout << "reading first block.. " << endl;
    temp.readBlock(nextRBN, file);
    temp.writeBlock(cout);
    cout<<temp.getRight();
    ++nextRBN;
    ++num_of_leaves;
    int RBN_to_right = temp.getRight();
    cout << "RBN to the right = " << RBN_to_right << endl;
    while (RBN_to_right > 0) {

        //() << RBN_to_right << endl;
        cout<<"RBN_to_right ="<<RBN_to_right<< endl;
        cout << "nextRBN =" << nextRBN << endl;
        temp.readBlock(nextRBN, file);
        //temp.writeBlock(cout);
        RBN_to_right = temp.getRight();
        ++nextRBN;
        ++num_of_leaves;
    }
    setField("firstRBN",0,file);
    setField("lastRBN",nextRBN-1,file);
    cout<<"num leaves:"<<num_of_leaves<<endl;
    cout << "There are [" << num_of_leaves << "] leaves." << endl;
    cout << "streampos is now :" << file.tellg() << endl;
    cout << ((file.eof())? "EOF is tripped. dammit" :
                            "EOF is reset! Continue") << endl;
    if (file.eof()) {
        cout << "EOF has not been reset... Eff word.";
        return;
    }
    // 2. get a count of the amount of blocks,
    // determine how many internal nodes needed.

    // Get the records per node, this is how many leaves
    // I can assign to a single interior node.

    cout << "Placing " << recs_per_node << " keys per node." << endl;
    // Below is just getting the ceiling of the integer division.
    // The the number of parents needed for a level.
    extra_children=num_of_leaves%recs_per_node;
    num_of_parents = num_of_leaves/recs_per_node;
    cout << "Should have " << num_of_parents << " parents above leaves." << endl;
    extra_children=num_of_leaves%recs_per_node;
    num_of_parents = num_of_leaves/recs_per_node;
    Block the_root;
    the_root.setRoot(true);

    if(num_of_leaves==1){
        //leaf is a root
        the_root.readBlock(0,file);
        the_root.setRoot(true);
        the_root.writeBlock(file);
        setField("rootRBN", 0, file);
    }
    else if ((num_of_parents == 1||num_of_parents==0)&&recs_per_node+extra_children<=Block::order) {
        Block the_root;
        the_root.setRoot(true);
        the_root.setLeaf(false);

        cout<<"need to make 1 parent"<<endl;
        cout<<"need to add "<<extra_children<<" extra children"<<endl;
        
        levels ++;
        cout<<"writing "<<levels<< " levels above the leaves (Root)"<<endl;
        the_root.setRBN(nextRBN);
        the_root.setParent(-9999);
        for (int i = 0; i < num_of_leaves; i++) {
            Block existing_child;
            existing_child.readBlock(i,file );      //Retrieve Child record
            existing_child.setParent(nextRBN);      //Set Parent of child
            //Push RBN to parent
            the_root.putChild(i);
            if (i > 0) {
                 the_root.putKey(findKey( the_root.getChild(i) , file));
            }
            //update children with parents information
            existing_child.setParent(nextRBN);
            //write out child
            existing_child.writeBlock(file);

        }
        the_root.writeBlock(file);
        setField("rootRBN", the_root.getRBN(), file);


    }
    else {
        levels ++;
        cout <<"NEW LEVEL ==> "<<levels<<endl;
        cout<<"writing "<<levels<< " levels above the leaves"<<endl;
         
         // In this case, there will be at least one level of interior
         // nodes between the root node and the data node.
		 
         // First establish the lowest level of the tree above the leaves.
         // Then generalize the situation to populate the branch nodes
         // to the root. Finally create the root.

        int least_parentRBN = num_of_leaves;
        cout<<"least_parentRBN "<<least_parentRBN<<endl;
        cout<<"need to make "<<num_of_parents<<" parents"<<endl;
        cout<<"need to distribute "<<extra_children<<" extra children among them"<<endl;
        for (int i = 0; i < num_of_leaves;
             i += recs_per_node+ ((extra_children-- > 0)? 1:0 )) {
            //create a new interior node
            Block new_interior(false, false, nextRBN);
            cout << "New block created. RBN =  " << nextRBN << endl;
            cout << "Children are RBNs :";
            //for each child
            for (int j = 0; j < recs_per_node+ (extra_children>0?1:0)
                            && (i-0+j) < num_of_leaves; j++) {
                cout<<"looking at child: "<<j<<endl;
                cout<<"should make: "<<recs_per_node+ (extra_children>0?1:0)<<" children\n";

                //read in child
                Block existing_child;
                existing_child.readBlock(i + j, file);
                cout<< "" << j + i <<  ", ";
                existing_child.setParent(nextRBN);
                //Push RBN to parent
                new_interior.putChild(i + j);

                // Push Key to parent
                // the first and last children do not actually pass up their keys.
                // So for a order so for n children Only children 1 < child < n would pass the key up to
                // the parent.
                if (j > 0) {
                        new_interior.putKey(findKey( new_interior.getChild(j) , file));
                }
                //update children with parents information
                existing_child.setParent(nextRBN);
                //write out child
                cout << "CHILD Writing RBN" << existing_child.getRBN() << endl;
                existing_child.writeBlock(file);
                cout << endl;
                cout << "INT Writing RBN" << new_interior.getRBN() << endl;
                new_interior.writeBlock(file);
                //levels ABOVE leaf nodes.
                //write out interior node
            }
            cout<<"i:"<<i<<" numchildren: "<<num_of_leaves<<endl;
           ++nextRBN;   //Next available RBN for allocation

        }
        least_parentRBN=num_of_leaves;
        int num_of_children = nextRBN - num_of_leaves;
        extra_children=num_of_children%recs_per_node;
        num_of_parents=num_of_children/recs_per_node;
        //num_of_parents=(num_of_parents==0?num_of_parents+1:num_of_parents);
        cout<<"num_of_parents: "<<num_of_parents<<endl;
        cout<<"extra_children: "<<extra_children<<endl;
        cout<<"num_of_children: "<<num_of_children<<endl;
        cout<<"least_parentRBN: "<<least_parentRBN<<endl;
        while(!((num_of_parents == 1||num_of_parents==0)
                &&recs_per_node+extra_children<=Block::order)){
            levels ++;
            cout <<"NEW LEVEL ==> "<<levels<<endl;
            cout<<"writing "<<levels<< " levels above the leaves"<<endl;
            cout<<"need to make "<<num_of_parents<<" parents"<<endl;
            cout<<"need to distribute "<<extra_children<<" extra children among them"<<endl;
            for (int i = least_parentRBN; i < least_parentRBN + num_of_children;
                i += recs_per_node+ ((extra_children-- > 0)? 1:0 )) {
                Block new_interior(false, false, nextRBN);
                cout << "New block created. RBN =  " << nextRBN << endl;
                cout << "Children are RBNs :";
                for (int j = 0; j < recs_per_node+ (extra_children>0?1:0)
                            && (i - least_parentRBN +j) < num_of_children; j++){
                    cout<<"looking at child: "<<j<<endl;
                    cout<<"should make: "<<recs_per_node+ (extra_children>0?1:0)<<" children\n";

                    //read in child
                    Block existing_child;
                    existing_child.readBlock(i + j, file);
                    cout<< "" << j + i <<  ", ";
                    existing_child.setParent(nextRBN);
                    //Push RBN to parent
                    new_interior.putChild(i + j);

                    if (j > 0) {
                        new_interior.putKey(findKey( new_interior.getChild(j) , file));
                    }
                    //update children with parents information
                    existing_child.setParent(nextRBN);
                    //write out child
                    cout << "CHILD Writing RBN" << existing_child.getRBN() << endl;
                    existing_child.writeBlock(file);
                    cout << endl;
                    cout << "INT Writing RBN" << new_interior.getRBN() << endl;
                    new_interior.writeBlock(file);
                    //levels ABOVE leaf nodes.
                    //write out interior node

                }
                cout<<"i:"<<i<<" numchildren: "<<num_of_children<<endl;
                ++nextRBN;   //Next available RBN for allocation
            }
            least_parentRBN=num_of_children;
            int num_of_children = nextRBN - num_of_children;
            extra_children=num_of_children%recs_per_node;
            num_of_parents=num_of_children/recs_per_node;
         }
        //only the root is left
        assert(((num_of_parents == 1||num_of_parents==0)
            &&recs_per_node+extra_children<=Block::order)&&"if this fails idk");
        Block the_root;
        the_root.setRoot(true);
        the_root.setLeaf(false);
        cout<<"need to make 1 parent"<<endl;
        cout<<"need to add "<<extra_children<<" extra children"<<endl;
        levels ++;
        cout <<"NEW FINAL LEVEL ==> "<<levels<<endl;
        cout<<"writing "<<levels<< " levels above the leaves"<<endl;
        the_root.setRBN(nextRBN);
        the_root.setParent(-9999);
        for (int i = least_parentRBN; i < least_parentRBN + num_of_children; i++) {
            Block existing_child;
            
            cout<<1;
            existing_child.readBlock(i,file );      //Retrieve Child record
            existing_child.setParent(nextRBN);      //Set Parent of child
            //Push RBN to parent
            cout<<2;
            if (i > least_parentRBN) {
                the_root.putKey(existing_child.getSmallestKey());
            }
            cout<<3;
            the_root.putChild(i);
            //update children with parents information
            cout<<4;
            existing_child.setParent(nextRBN);
            //write out child
            existing_child.writeBlock(file);

        }
        the_root.writeBlock(file);
        setField("rootRBN", the_root.getRBN(), file);
        


    }
}

#endif
