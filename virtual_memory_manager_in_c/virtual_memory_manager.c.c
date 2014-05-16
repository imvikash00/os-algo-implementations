#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define TABLE_ENTRIES 256                //NUmber of entries in pagetable

int pagetable[TABLE_ENTRIES];            //Creating the pagetable
int frametable[TABLE_ENTRIES];           //Creating the frametable
int physicalmemorypointer = 0;           //will point where the next 256 byte will go in physical memory 
int bitmask = 0x00FF;                   //use to mask high order 16 bits
int tlb_remove_poi = 0;                  //will point to which tlb entry to remove
char physicalmemory[TABLE_ENTRIES*TABLE_ENTRIES];       //Physical memory to hold the contents read from Backing store
int tlb[16][2],numofaddresses=0;                                //Tlb   //no. of addresses read 
double pagefaults, pagefaultsrate, tlbhits =0;        


int tlb_contain_key(int pageno)           //check if tlb contains a pagenumber 
{
	int j;
	for(j=0;j<16;j++)
	{
		if(tlb[j][0]==pageno)
			return 1;
	  }
	return 0;
}

void tlb_remove(int p)             //remove the entry pointed tlb_remove_poi
{
	tlb[tlb_remove_poi][0]=-1;	
}

int checktlb(int pageno)              //returns framenumber corresponding to given pagenumber
{
	int frameno,j;
	for(j=0;j<16;j++)
	{
		if(tlb[j][0]==pageno)
			frameno = tlb[j][1];
	}
	return frameno;
	
}

int tlbsize()                   //returns tlb size i.e no. of valid tlb entries
{
	int count=0,j;
   for(j=0;j<16;j++)
	{
		if(tlb[j][0]!=-1)
			count++;
	}
	return count;
}

void addtotlb(int pageno , int frameno)              //add pageno. and frame no. in tlb
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
	tlb_remove_poi=(tlb_remove_poi+1)%16;                     //update the tlb_remove poi in fifo manner
}

int getframenumber()
{
	int i;
	for(i=0;i<256;i++)
	{
		if(frametable[i]==-1)
			return i;
	}
}

int main()
{
	FILE *fp,*fw;
	char* filename = ("addresses.txt");         
	int i;
	char data[80];               //hold the value read from file addresses.txt
	
	fw=fopen("output.txt","w"); //output file
	
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
		int no = atoi(data) & 0x0000FFFF, dub=no, pagenumber = no >> 8, offset=dub & bitmask, val=0, framenumber=0;
		
		if((!(tlb_contain_key(pagenumber)) && pagetable[pagenumber]==-1))  //when pageno. is not in tlb and pagetalbe i.e pagefault
		{
			FILE *fb;
			fb = fopen("BACKING_STORE.bin","rb");   //open .bin file
			fseek(fb, ((pagenumber) * TABLE_ENTRIES), SEEK_SET); //set the read pointer to correct pagenumber
			fread(physicalmemory+physicalmemorypointer, 256, 1 , fb); //read 256bytes from there and store it in physical memory
			val = physicalmemory[physicalmemorypointer+offset];       //get the value 
			physicalmemorypointer += TABLE_ENTRIES;             //update the physicalmemorypointer i.e next location where writing should start
			int frame = getframenumber();
            pagetable[pagenumber] = frame;                //Update PageTable i.e add to pagetalbe
			frametable[frame]=1;
			if(tlbsize() <16){                        //Update TLB i.e add to tlb    
                addtotlb(pagenumber,frame);
            }
            else{                                //if no space the remove one entry and then add to tlb
                 tlb_remove(tlb_remove_poi);
				 addtotlb(pagenumber,frame);
			}
			pagefaults++;              //increase pagefault count
			
			int physicaladdress =  frame*256 + offset;          //get the pysical address
			fprintf(fw,"Virtual address: %d Physical address: %d Value: %d\n",atoi(data), physicaladdress,val);
			numofaddresses++;
			fclose(fb);
		}
	else{         
		
			if(tlb_contain_key(pagenumber)){          //when page is there in tlb 
				framenumber = checktlb(pagenumber);       //get the frame no. from tlb
				int physicaladdress =  framenumber*256 +offset;      //get the physical address
				fprintf(fw,"Virtual address: %d Physical address: %d Value: %d\n", atoi(data),physicaladdress,physicalmemory[framenumber*256+offset]);
				tlbhits++;                  //increase tlb hits 
				numofaddresses++;
			}
			else{                                        //page is not in tlb but in pagetable
				framenumber = pagetable[pagenumber];         //get the frame number
				int physicaladdress =  framenumber*256 + offset;
			    fprintf(fw,"Virtual address: %d Physical address: %d Value: %d\n", atoi(data),physicaladdress,physicalmemory[framenumber*256+offset]);
				numofaddresses++;
				if(tlbsize() <16){                    //add this page to tlb 
					addtotlb(pagenumber,framenumber);
				}
				else{
					tlb_remove(tlb_remove_poi);
					addtotlb(pagenumber,framenumber);
				}
			}	
		}
	}
	fclose(fp);
	fclose(fw);
	return 0;
}	
	
	
	
	
	
	
	
	
	
	
	
	
	
	