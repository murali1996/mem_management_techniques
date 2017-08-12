// Reading from file pageReferences.txt and writing to data.dat
// Libraries required
#include <iostream>
#include <cmath>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib> 
using namespace std;

ofstream outfile;

// Functions Declaration
void fifo(int);
void lru(int);
void nfu(int);
int file_c(int,int);

// Globally defined variables
int	n=1000; // Number of page queries
int MAX_RANGE=100; // Maximum value of a query

int main(){
	int status;
	status = file_c(n,MAX_RANGE); // Create a data file: simulating input
	if(status!=1) {cout<<"ERROR in creating input file."<<endl; return 0;}

	outfile.open("data.dat");
	int select;
	cout << "Type 1 for FIFO, 2 for LRU, 3 for NFU: " << endl;
	cin >> select;	

	int pageFrames=100; // Edit the number of pageFrames here
    while(pageFrames>0){
		switch(select){
			case 1:
				fifo(pageFrames);
				break;
			case 2:
				lru(pageFrames);
				break;
			case 3:
				nfu(pageFrames);
				break;
			default:
				cout << "Error. Please press 1 or 2 or 3"<< endl;
				break;
		}
		cout << "pageFrames: " << pageFrames << endl;
		pageFrames=pageFrames-1;
	}
	
	// Plotting the graph
	outfile.close();	
	FILE *gnuplot = popen("gnuplot -persist","w");
	fprintf(gnuplot,"plot 'data.dat' with linespoints\n");
	fclose(gnuplot);
	return 0;
}

int file_c(int n,int MAX_RANGE){
	ofstream myfile;
	myfile.open ("pageReferences.txt");
	srand((unsigned)time(0)); 
    int num[n],i=0;
    for (i;i<n;i++){
	    num[i] = (rand()%MAX_RANGE)+1; 
    }
    for (i=0;i<n;i++){
	    myfile << num[i] << "\n";
    }
    myfile.close();
    return 1; // Returns 1 on successful termination
} 

void fifo(int pageFrames){
	
	int temp,i;
	int num[pageFrames]={0}; 
	int hit,hitCount=0;
	int pageReference,pageReference_n=0;

	// File to read Page Refernces
	ifstream infile("pageReferences.txt");
	while (infile >> pageReference){
		pageReference_n=pageReference_n+1;	
		hit=0; 
		for (temp=0;temp<pageFrames;temp++){
			if(num[temp]==pageReference) {
				hit=1;
				break;
			}
		}
		if(hit==1) 
			hitCount=hitCount+1;
		else{
			for(temp=pageFrames-2;temp>=0;temp--)
				num[temp+1]=num[temp];
			num[0]=pageReference;
		}
	}
	outfile << pageFrames << "\t" << pageReference_n-hitCount << "\n";
}


void lru(int pageFrames){
	int temp,i;
	int num[pageFrames]={0}; 
	int hit=0,hitCount=0;
	int pageReference,pageReference_n=0;
	
	ifstream infile("pageReferences.txt");
	while (infile >> pageReference){	
		pageReference_n=pageReference_n+1;
		hit=0;
		for (temp=0;temp<pageFrames;temp++){
			if(num[temp]==pageReference) {
				hit=1;
				break;
			}
		}

		//Put the page in the list
		if (hit==1){
			hitCount=hitCount+1;
			for(i=temp-1;i>=0;i--)
				num[i+1]=num[i];
			num[0]=pageReference;
		}
		else {
			for(i=pageFrames-2;i>=0;i--)
				num[i+1]=num[i];
			num[0]=pageReference;
		}
	}
	outfile << pageFrames << "\t" << pageReference_n-hitCount << "\n";
}


void nfu(int pageFrames){

	int counter[n]={0};
	int i,min=0,min_pageFrames,index;
	int num[pageFrames]={0}; 			  // The page Frames
	int hit=0,hitCount=0,flag;
	int pageReference,pageReference_n=0;
	
	// File to read Page Refernces from
	ifstream infile("pageReferences.txt");
	
	while (infile >> pageReference){
		cout << "Page Refrence: " << pageReference << endl;
		pageReference_n=pageReference_n+1;	
		hit=0;
		for (i=0;i<pageFrames;i++){
			if(num[i]==pageReference) {
				hit=1;
				break;
			}
		}
		counter[pageReference-1]=counter[pageReference-1]+1;
		/*for (i=0;i<10;i++)
			cout << "C: " << counter[i] << "\t";
		cout << endl; */
		if(hit==1) 
			hitCount=hitCount+1;
		else{
			// Make sure that no num[] is Zero
			flag=0;
			for (i=0;i<pageFrames;i++){
				if(num[i]==0){
					flag=1;
					break;
				}
			}
			// cout << "FLAG: " << flag << endl;
			if(flag==1){
				for(i=pageFrames-2;i>=0;i--)
					num[i+1]=num[i];
				num[0]=pageReference;
			}
			// Replacement. Tie-Breaker: FIFO
			else if (flag==0){
				min=1000;
				for(i=0;i<pageFrames;i++){
					index=num[i]-1;
					if(counter[index]<=min){  
						min=counter[index];
						min_pageFrames=i;
					}
				}
				for(i=min_pageFrames;i>0;i--)
					num[i]=num[i-1];
				num[0]=pageReference;
			}	
		}
		/*for(i=0;i<5;i++)
			cout << "Num: " << num[i]<< "\t";
		cout << endl << endl;*/
	}
	outfile << pageFrames << "\t" << pageReference_n-hitCount << "\n";
}
