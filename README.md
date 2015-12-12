\mainpage User Manual

- a) Name of Program:
	+ SUPER B+ Tree that is not quite finished
- b) Purpose:
	+ Learn about B+ trees and increase programming skills
- c) Location on Computer
	+ Located in C:/Users/Database/
- d) Compile and Execute Instructions:
	+ run the make program on the directory then open run.exe
- e) Example Execution:
	+on execution:
	+O: open a designated database file
	+N: create a new database file from designated record file
   	+choose the key [1,2,3,4] and order (order > 2) for the
    +database.
    +T: testopen on Records.txt, tree.txt
    +=============================================================
    +O: open a designated database file
	+N: create a new database file from designated record file
    +choose the key [1,2,3,4] and order (order > 2) for the
    +database.
	+T: testopen on Records.txt, tree.txt
	+P: print the B+ tree to terminal
	+S: search for a record by designating a key in the open database
	+R: remove an item designated by a key from the open database
	+X: remake the currently opened database into a designated database file
   	+choose an order and a key for the new database file.
   	+=============================================================
   	+n
	+enter data file name:testing.txt
	+enter tree file name:testingTree.txt
	+enter order (order > 2): 4
	+enter key [1,2,3,4]1
	+file read, inputfile/outputfile:testing.txt tmp_extracted_testing.txt
	+last, first, id1, id2:Morris Andrew 12345 43210 Morris Andrew 12345 4321
	+032 Hendren Amanda 9876 5678
	+3259 Morris Jonathan 9999 88888
	+5988 Morris Andrew 12345 4321
	+88115 Hendren Amanda 9876 5678
	+115142 Morris Jonathan 9999 88888
	+142filled
	+push current:142 Morris
	+finish off
	+constructor
	+total Elements: 7
	+hi115 Hendrenhi142 Morrishi0 Morrishi88 Morrishi59 Morrishi142 Morrishi142 Morris
	+rename: temp.dat
	+To: sortedtesting.txt
	+initialized block class (order, key), (4, 1)
	+writing to file: testingTree.txt
	+reading from file: sortedtesting.txt
	+============================================================================
	+testing.txt
	+Morris Andrew 12345 4321 |
	+Hendren Amanda 9876 5678 |
	+Morris Jonathan 9999 88888 |
	+Morris Andrew 12345 4321 |
	+Hendren Amanda 9876 5678 |
	+Morris Jonathan 9999 88888 |

	+============================================================================
	+tmp_extracted_testing.txt
	+0 Morris 32 Hendren 59 Morris 88 Morris 115 Hendren 142 Morris 
	+============================================================================
	+tmp_runs_testing.txt
	+32 Hendren 115 Hendren 142 Morris 0 Morris 88 Morris 59 Morris 142 Morris  |
 	+|
	+============================================================================
	+tmp_sortedRuns_testing.txt
	+32 Hendren 115 Hendren 142 Morris 0 Morris 88 Morris 59 Morris 142 Morris 
	+============================================================================
	+sortedtesting.txt
	+Hendren Amanda 9876 5678 |
	+Hendren Amanda 9876 5678 |
	+Morris Jonathan 9999 88888 |
	+Morris Andrew 12345 4321 |
	+Morris Andrew 12345 4321 |
	+Morris Jonathan 9999 88888 |
	+Morris Jonathan 9999 88888 |
	+============================================================================
	+testDataTree.txt
	+HS 00089 OR 00004 KY 00001 FR 00000 LR 00001 RR 00003 RS 00005 FS 00010 KS 00010 BS 00166
	+0 1 03 -9999 00001 00002        ren     Amanda 0000009876 0000005678 |     endren     Amanda 
	+0000009876 0000005678 |     Morris   Jonathan 0000009999 0000088888 | !
	+0 1 04 00000 -9999 00002     Morris     Andrew 0000012345 0000004321 |       rris     Andrew 
	+0000012345 0000004321 |        ris   Jonathan 0000009999 0000088888 |  
	+0 0 01 00003     Morris %%%%%%%%%% %%%%%%%%%% | 00000 00001 %%%%% %%%%% 
	+###########################################################################################!
	+1 0 00 -9999 %%%%%%%%%% %%%%%%%%%% %%%%%%%%%% | 00002 %%%%% %%%%% %%%%% 
	###########################################################################################!
	+=============================================================================
- f) Known Errors:
	+ incomplete