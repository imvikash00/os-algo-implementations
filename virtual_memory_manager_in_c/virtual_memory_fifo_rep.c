#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define TABLE_ENTRIES 256              //NUmber of entries in pagetable

int pagetable[TABLE_ENTRIES];            //Creating the pagetable
int frametable[128];                     //Creating the frametable
int physicalmemorypointer = 0;           //will point where the next 256 byte will go in physical memory 
int bitmask = 0x00FF;                      //use to mask high order 16 bits
int tlb_remove_poi = 0;                 //will point to which tlb entry to remove
char physicalmemory[128*TABLE_ENTRIES];   //Physical memory to hold the contents read from Backing store
int tlb[16][2],numofaddresses=0;                                //Tlb   //no. of addresses read 
double pagefaults, pagefaultsrate, tlbhits =0;        


int tlb_contain_key(int pageno)   //check if tlb contains a pagenumber 
{
	int j;
	for(j=0;j<16;j++)
	{
		if(tlb[j][0]==pageno)
			return 1;
	}
	return 0;
}


void tlb_remove(int p)  //remove the entry pointed tlb_remove_poi
{
	tlb[tlb_remove_poi][0]=-1;	
}

int checktlb(int pageno)   //returns framenumber corresponding to given pagenumber
{
	int frameno,j;
	for(j=0;j<16;j++)
	{
		if(tlb[j][0]==pageno)
			frameno = tlb[j][1];
	}
	return frameno;
	
}

int tlbsize()    //returns tlb size i.e no. of valid tlb entries
{
	int count=0,j;
   for(j=0;j<16;j++)
	{
		if(tlb[j][0]!=-1)
			count++;
	}
	return count;
}

void addtotlb(int pageno , int frameno) //add pageno. and frame no. in tlb
{
	int j;
	for(j=0;j<16;j++)
	{
		if(tlb[j][0]==-1){
			tlb[j][1]=frameno;
			tlb[j][0]=pageno;
			break;
		}	
	}
	tlb_remove_poi=(tlb_remove_poi+1)%16; //update the tlb_remove poi in fifo manner
	

}

int fifo_victim()   //selects the victim page in lru fashion
{
	int i=0;
	for(i=0;i<128;i++)  //if any frame is invalid then use it
	{
		if(frametable[i]==-1){
			return i;
		}
	}
	int min=0;
	for(i=0;i<128;i++)   
	{
		if(frametable[i]<frametable[min])
			min=i;
	}
	for(i=0;i<256;i++) {  //invalidate the pagetable entry
		if(pagetable[i]==min){
			pagetable[i]=-1;
			break;
		}	
	}
	for(i=0;i<16;i++)
	{
		if(tlb[i][1]==min){     //invalidate the tlb entry
			tlb[i][0]=-1;
			break;
		}
	}
	return min;
}


int main()
{
	FILE *fp,*fw;
	char* filename = ("addresses.txt");         
	int i;
	char data[80];               //hold the value read from file addresses.txt
	
	fw=fopen("fifo_output.txt","w"); //output file
	
	/*Create pageTable and initialize each index with -1. This indicates that the entry in the pageTable is invalid.*/
    for(i = 0; i <(TABLE_ENTRIES); i++){
        pagetable[i] = -1; 
	}
	//invalidate all tlb entries
	for(i=0;i<16;i++)
		tlb[i][0]=-1;
	
	/*Create frameTable and  initialize each index starting at 0 and increasing by TABLE_ENTRIES_OR_SIZE at each iteration*/
    for(i = 0; i < (TABLE_ENTRIES); i++){
        frametable[i] = -1;
	}
	
	fp = fopen(filename, "r");
	if(fp == NULL)
	{
		printf("Could not open file %s", filename);
		return 1;
	}
	while(fgets(data, 80, fp)!=NULL)
	{
		/*Right-Shift virtual Address by 8 bits virtual address space256). Use bitMask to "mask" Least Significant 0xFF bits*/
		int no = atoi(data) & 0x0000FFFF, dub=no, pagenumber = no >> 8, offset=dub & bitmask, framenumber=0;
		
		if((!(tlb_contain_key(pagenumber)) && pagetable[pagenumber]==-1))  //page fault case
		{
			FILE *fb;
			fb = fopen("BACKING_STORE.bin","rb");
			int victim;
			victim=fifo_victim();  //select the victim page
			fseek(fb, ((pagenumber) * TABLE_ENTRIES), SEEK_SET);
			fread(physicalmemory+(victim)*256, 256, 1 , fb);   //read from backing store
			physicalmemorypointer += TABLE_ENTRIES;        
            pagetable[pagenumber] = victim;            //update the pagetable
			frametable[victim]=numofaddresses+1;     //update the frame table with new value
            framenumber = pagetable[pagenumber];        
			
			//Update TLB     
			if(tlbsize() <16){
                addtotlb(pagenumber,framenumber);
            }
            else{
                 tlb_remove(tlb_remove_poi);
				 addtotlb(pagenumber,framenumber);
			}
			pagefaults++;
			
			int physicaladdress =  (numofaddresses)*256 + (framenumber | offset);
			fprintf(fw,"Virtual Address: %d Physical Address: %d Value: %d\n", atoi(data),physicaladdress,physicalmemory[victim*256+offset]);
			numofaddresses++;
			fclose(fb);

		}
	else{
		
			if(tlb_contain_key(pagenumber)){
				//frametable[pagetable[pagenumber]]=numofaddresses+1;   //this update doesn't happen in fifo
				framenumber = checktlb(pagenumber);
				int physicaladdress =  (numofaddresses)*256 + offset;
				fprintf(fw,"Virtual Address: %d Physical Address: %d Value: %d\n", atoi(data),physicaladdress,physicalmemory[framenumber*256+offset]);
				tlbhits++;
			}
			else{
				//frametable[pagetable[pagenumber]]=numofaddresses+1;        //this update doesn't happen in fifo else everything is same in fifo
			//Take pageNumber as index and get Frame number from Page table
				framenumber = pagetable[pagenumber];
			//Take Frame number  "or" it with offset to get physical address	
				int physicaladdress =  (numofaddresses)*256 + (offset);
			    fprintf(fw,"Virtual Address: %d Physical Address: %d Value: %d\n", atoi(data),physicaladdress,physicalmemory[framenumber*256+offset]);
				if(tlbsize() <16){
                addtotlb(pagenumber,framenumber);
            }
            else{              
				tlb_remove(tlb_remove_poi);
				addtotlb(pagenumber,framenumber);
			}
			}	
			numofaddresses++;
		}
	}
	fclose(fp);
	fclose(fw);
	printf("Number of Page Faults: %f\n",pagefaults);
	printf("Number of tlb hits: %f\n",tlbhits);
	printf("Tlb hit rate:%f %%\n",(tlbhits/1000)*100);
	return 0;
}	
	
	
	
	
	
	
	
	
	
	
	
	
	
	