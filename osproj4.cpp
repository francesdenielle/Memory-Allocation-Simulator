
#include <iostream>
#include <iomanip>
#include <vector>
#include <sstream>
#include <cmath>
#include <limits>
#include <cctype>
#include<bits/stdc++.h>

using namespace std;

//initializing functions
void Heading ();
int UserInput ();
void InitObj ();
void Processes (int selected);
void About ();
void Compaction (int process[], int block[], int blockCount, int processCount, int blockFilled[], int SC[], int withSC[], int bins[]);
void Coalesce (int process[], int block[], int blockCount, int processCount, int blockFilled[], int SC[], int withSC[], int bins[]);
void Clean ();
void Display();
void FirstFit (int block_size[], int total_blocks, int process_size[], int total_process, int SC[], int withSC[]);
void Simulation ();



//variables
int timeElapsed = 0;
int CH = 0;
string status = "";


int UserInput (){
	int selected = 0;
	string userSelected = "";

	while (selected == 0){
		cout<<"User Input: ";
		cin>>userSelected;
		
		transform(userSelected.begin(), userSelected.end(), userSelected.begin(), ::toupper);
		
		if (userSelected == "FIRST_FIT" || userSelected == "1")
			selected = 1;
			
		else if (userSelected == "ABOUT" || userSelected == "2")
			selected = 2;
			
			             		
		else if (userSelected == "EXIT" || userSelected == "3")
			selected = 3;
				
		else
			cout<<"Input not in selection. Please try again."<<endl<<endl;
	}
			
	return selected;
}

void Heading (){
	cout<<"\n================================================================================================="<<endl;
	cout<<"/////////////////////////////////////// OS LAB PROJECT //////////////////////////////////////////"<<endl;
	cout<<"================================================================================================="<<endl;
	cout<<"\n------ Menu -----"<<endl;
	cout<<"[1] First_Fit"<<endl;
	cout<<"[2] About"<<endl;
	cout<<"[3] Exit"<<endl;
	cout<<"-----------------"<<endl;
}

void About(){
	cout<<"\n================================================================================================="<<endl;
	cout<<"//////////////////////////////////////////// ABOUT //////////////////////////////////////////////"<<endl;
	cout<<"================================================================================================="<<endl;
	cout<<"\n------ GROUP MEMBERS -------"<<endl;
	cout<<"[1] Katrina Francesca Soo"<<endl;
	cout<<"[2] Frances Denielle Magno"<<endl;
	cout<<"\n--------- SOURCES ----------"<<endl;
	cout<<"[1] https://github.com/tody411/MemoryAllocationSimulator/blob/master/memory_allocaiton.py"<<endl;
	cout<<"[2] https://www.youtube.com/watch?v=qdkxXygc3rE"<<endl;
	cout<<"[3] https://www.youtube.com/watch?v=jgv6UrPhmFI\n"<<endl;
	
	Heading();
	Processes(UserInput());
}

void Processes(int selected){
	if (selected == 1)
		InitObj();
	else if (selected == 2)
		About();
	else if (selected == 3)
		cout<<"Exited";
}

void InitObj (){
	int blockCount = 0,
		processCount = 0,
		time = 0;
	double size = 0;
	
	
	cout<<"\n================================================================================================="<<endl;
	cout<<"////////////////////////////////////////// SIMULATION ///////////////////////////////////////////"<<endl;
	cout<<"================================================================================================="<<endl;
	
	//block
	cout<<"Enter number of blocks: ";
	cin>>blockCount;
	int block[blockCount];
	for (int i = 0; i < blockCount; i++){
		cout<<"Block ["<<i<<"] size: ";
		cin>>size;
		block[i] = size;
	}
	
	//process
	cout << "Enter quantity of Processes: ";
	cin >> processCount;
	int process[processCount];
	int SC[processCount];
	int withSC[blockCount];
	
	//setting SC's elements to 0
	for (int i = 0; i < processCount; i++){
		SC[i] = 0;
	}
	
	//setting withSC's elements to 0
	for (int i = 0; i < blockCount; i++){
		withSC[i] = 0;
	}
	
	for (int i = 0; i < processCount; i++){
		cout<<"\nProcess ["<<i<<"] size: ";
		cin>>size;
		process[i] = size;
		cout<<"Process ["<<i<<"] compaction time: ";
		cin>>time;
		SC[i] = time;
	}
	
	cout <<"\nEnter coalescing time (CH): ";
	cin>>CH;

	FirstFit (block, blockCount, process, processCount, SC, withSC);
	
}

void FirstFit (int block_size[], int total_blocks, int process_size[], int total_process, int SC[], int withSC[]) {
   int allocation[total_process];						//this is for the index of the block that has a process
   memset(allocation, -1, sizeof(allocation));			
   int allocatedBlocks[total_blocks];					//this is an array that would store the process size later
   int myBlockSize[total_blocks];						//this is will store all the block sizes later (for printing purposes)
   int bins[total_blocks];								//this will store the process sizes that will not be allocated
    
    //to initialize each array
	for(int i = 0; i<total_blocks; i++){
		allocatedBlocks[i] = 0;
    	myBlockSize[i] = block_size[i];
    	bins[i] = 0;
   	}
   
   //this loop will allocate every process to a block
   for (int i = 0; i < total_process; i++) {
      for (int j = 0; j < total_blocks; j++) {
         if (block_size[j] >= process_size[i]) {
            allocation[i] = j;
            block_size[j] -= process_size[i];
            break;
         }
      }
   }
   
   //prints the table which shows the results of the first fit process
   cout << "Process No.\tProcess Size\tBlock no."<<endl;
   for (int i = 0; i < total_process; i++) {
      cout << " " << i+1 << "\t\t" << process_size[i] << "\t\t";
		if (allocation[i] != -1){
	      	 cout << allocation[i] + 1;
	      	 allocatedBlocks[allocation[i]] = process_size[i];
	      	 withSC[allocation[i]] = SC[i];
		}
		else{
			//puts process size in index i into bins
	         bins[i] = process_size[i];
	         cout << "Not Allocated";
	  	}
         cout << endl;
   }
   	
   	cout << "\nPress enter to continue";
	cin.ignore();
   	Compaction(process_size, myBlockSize, total_blocks, total_process, allocatedBlocks, SC, withSC, bins);
   
}

void Compaction (int process[], int block[], int blockCount, int processCount, int blockFilled[], int SC[], int withSC[], int bins[]) {
	//my variables
	bool unfinished = true,
		 sorted = false,
		 timeUp = false;
	int counter = 0,
		blockCompaction = 0,
		checker = 0,
		binsCount = 0;
	status = "Compacting...";
	
	//this just adds 1 second to each element in the array of SCs
	for(int i = 0; i < blockCount; i++){
		if (withSC[i]>0)
			withSC[i]++;
	}
	
	//while loop that would process and show what happens in our compaction algorithm
	while (unfinished){
		counter = 0;
		sorted = true;
		//system("cls");
		
		//counts how many blocks are filled with processes
		for(int i = 0; i < blockCount; i++){
			if (blockFilled[i] > 0)
				blockCompaction++;
		}
		
		//checks if the blocks are magkadikit sa taas
		for(int i = 0; i < blockCount; i++){
			if (blockFilled[i] != 0){
				checker++;
			}else {
				break;

			}
		}
	
		//The while loop would finish if the number of blocks that are magkadikit sa taas are equal to
		//the number of blocks that are filled with processes
		if(checker == blockCompaction){
			status = "Finished Compaction";
			unfinished = false;
		}
		
		//Displays what is happening to our variables
		cout<<"======================================================"<<endl;
		cout<<"Time: \t"<<timeElapsed<<"\t|||\tStatus: "<<status<<endl;
		cout<<"======================================================"<<endl;
		cout<<"Process\t\tMemory\t\tTime Allotted"<<endl;
		for(int i = 0; i < processCount; i++){
			cout<<i<<"\t\t"<<process[i]<<"\t\t"<<SC[i]<<"\t"<<endl;
		}
		cout<<"======================================================"<<endl;
		
		cout<<"ID\t\tBlock Memory Size\tCompaction\t\ttime"<<endl;
		//decrements the timers in each process
		for(int i = 0; i < blockCount; i++){
			if (withSC[i]>0)
				withSC[i]--;
			cout<<i<<"\t\t"<<block[i]<<"\t\t\t"<<blockFilled[i]<<"\t\t\t"<<withSC[i]<<endl;
		}
		
		//this just switches the blocks, timers, and processes 
		int counter = 0;
		for (int i = 0; i < blockCount; i++){
			if(withSC[i]==0 && blockFilled[i] > 0){
				
				if(i != counter && blockFilled[i-1] == 0){
					int temp = blockFilled[i];
					blockFilled[i] = blockFilled[i-1];
					blockFilled[i-1] = temp;
					
					int blockTemp = block[i];
					block[i] = block[i-1];
					block[i-1] = blockTemp;
					
					int SCTemp = withSC[i];
					withSC[i] = withSC[i-1];
					withSC[i-1] = SCTemp;
				}
				
				if(blockFilled[i] != 0){
					counter++;
				}
			}
		}
		
		for (int i = 0; i < blockCount; i++){
			if (withSC[i] == 0){
				timeUp = true;
			}
		}
		
		checker = 0;
		blockCompaction = 0;
		timeElapsed++;
		//cout << "\nPress enter to continue";
		//cin.ignore();
	}
	
	for(int i = 0; i < blockCount; i++){
		if (bins[i]!=0){
			binsCount++;
		}
	}
	
	Coalesce (process, block,  blockCount,  processCount,  blockFilled, SC,  withSC,  bins);

} 

void Coalesce (int process[], int block[], int blockCount, int processCount, int blockFilled[], int SC[], int withSC[], int bins[]) {
	//declaration of variables
	int binCount = 0;
	int blocksToMergeCount = 0;
	int counter = 0;
	int coalesceCount = 0;
	int prevNumber = -1;
	int prevNumber2 = -1;
	int lastcounter = 0;
	int newBlockCount = 0;
	int temp = 0;
	int myi=0;
	int newCounter = 0;
	int coalesced = 0;
	int filledBlockCount = 0;
	int arrLength = 0;
	int delay = 0;
	bool coalescing = true;
	bool hasBin = true;
	status = "Coalescing...";
	
	//counts number of unallocated processes and the blocks to merge
	for(int i = 0; i < blockCount; i++){
		if (bins[i]!=0){
			binCount++;
		}
		if (blockFilled[i] == 0){
			blocksToMergeCount++;
		} else {
			filledBlockCount++;
		}
	}
	
	//signifies that there is no unallocated processes
	if (binCount <= 0)
		hasBin = false;
	
	//declaring variables
	newBlockCount = blocksToMergeCount;
	int newBlock[newBlockCount];
	int coalesceBlockSize[blocksToMergeCount];
	int newBins[binCount];
	int newAllocations[newBlockCount];
	
	//setting some of my arrays' elements to 0
	for(int i = 0; i<blocksToMergeCount; i++){
		coalesceBlockSize[i]=0;
		newAllocations[i]=0;
	}
	
	//setting newBins' array elements to 0
	for(int i = 0; i<binCount; i++){
		newBins[i]=0;
	}
	
	//transfers only the blocks that are unused to coalesceBlockSize
	for (int h = 0; h < blocksToMergeCount; h++){
		for (int i = 0; i < blockCount; i++){
			if (i > prevNumber && blockFilled[i] == 0){
				coalesceBlockSize[h]=block[i];
				prevNumber = i;
				break;
			}
		}
	}
	
	//transfers only the values in bins (unallocated) that are not 0 to newBins
	for (int i = 0; i<binCount; i++){
		for (int j = 0; j<blockCount; j++){
			if (i > prevNumber2 && bins[j]!=0){
				newBins[i] = bins[j];
				prevNumber2 = i;
				break;
			}
		}
	}
	
	temp = coalesceBlockSize[newBlockCount-1];
	
	//loop that adds up all the unused blocks from bottop to top
	while (coalescing){
		timeElapsed++;
		
		//setting the array's elements to 0
		for(int i = 0; i<newBlockCount; i++){
			newBlock[i] = 0;
		}
		
		//transfers block sizes to newBlock
		for(int i = 0; i < newBlockCount; i++){
			newBlock[i] = coalesceBlockSize[i];
		}
		
		//simulates the adding up of blocks (depending on the condition)
		if (binCount > 0 && CH <= 0){
			temp += coalesceBlockSize[newBlockCount-1];
		} else if (CH <= 0){
			temp += coalesceBlockSize[newBlockCount-1];
		}
		
		//signifies that a process has been allocated to a added up block
		if (binCount>0 && CH <=0) {
			for (int i = 0; i < newBlockCount; i++){
				if (newBins[newCounter] < temp) {
					coalesced = newBins[newCounter];
					binCount--;
					break;
				}
			}	
		}
		
		//---------------------------------the display---------------------------------------//
		cout<<"\n========================================================"<<endl;
		cout<<"Time: \t"<<timeElapsed<<"\t|||\tStatus: "<<status<<endl;
		cout<<"========================================================"<<endl;
		cout<<"Process\t\tNot Allotted\t\tTime Allotted: "<<CH<<endl;
		for(int i = 0; i < binCount; i++){
			cout<<i<<"\t\t"<<newBins[i]<<"\t\t\t"<<CH<<endl;
		}
		cout<<"========================================================"<<endl;
	
		cout<<"ID\t\tBlocks Left\tAllocated"<<endl;
		//decrements the timers in each process
		for(int i = 0; i < newBlockCount-1; i++){
			cout<<i<<"\t\t"<<newBlock[i]<<"\t\t"<<newAllocations[i]<<endl;
			myi=i;
		}
		cout<<myi<<"\t\t"<<temp<<"\t\t"<<coalesced<<endl;  
		//------------------------------------------------------------------------------------//
		
		//to end loop
		if((hasBin && binCount == 0) || newBlockCount <= 0){
			coalescing = false;
			status = "Finished Coalescing";
		}
		
		//CH timer
		if (CH > 0)
			CH--;
			
		//to help with the adding up simulation
		if (CH <= 0)
			newBlockCount--;
		counter++;
		delay = timeElapsed - 1;
	}
	int lastcoalesced[blockCount];
	for (int i = 0; i < blockCount; i++){
		lastcoalesced[i]=0;
	}
	for (int i = 0; i < blockCount; i++){
		if (i<filledBlockCount){
			lastcoalesced[i+1]= block[i];
			arrLength++;
		} else if (newBlock[lastcounter] < newBlockCount) {
			lastcoalesced[i+1]=newBlock[lastcounter];
			lastcounter++;
			arrLength++;
		} else {
			lastcoalesced[i+1] = temp;
			arrLength++;
		}
	}
	//--------------------------------------------------------------------------//
	status = "Completed";
	cout<<"\n======================================================"<<endl;
		cout<<"Time: \t"<<timeElapsed<<"\t|||\tStatus: "<<status<<endl;
	cout<<"======================================================"<<endl;
	cout<<"First Fit Compacted and Coalesced "<<endl;
	if (hasBin){
		for (int i = 1; i < arrLength-2; i++){
			cout<<lastcoalesced[i]<<endl;
		}
	} else {
		for (int i = 1; i < arrLength-1; i++){
			cout<<lastcoalesced[i]<<endl;
		}
	}
	
	timeElapsed++;
}

void Simulation(){
	InitObj();
}

int main() {
	char answer = ' ';
	
	do{
		Heading();
		Processes(UserInput());
		
		cout<<"Repeat? [y/n]: ";
		cin>>answer;
	}while (answer = 'y');
	
	return 0;
	
}