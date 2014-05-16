package bankers;
import java.util.Scanner;

public class Bankers{
    private int need[][],allocate[][],max[][],avail[][],no_proc,no_res; //defining the matrixes need for banker
    private int seq[];
    private boolean s[];
    private int[][] need_matrix(){        //function to calculate the need matrix
       for(int i=0;i<no_proc;i++)
         for(int j=0;j<no_res;j++)  
          need[i][j]=max[i][j]-allocate[i][j];
      return need;
    }
 
    private boolean check(int i){  //checking if ith process can be allocated the resources 
     for(int j=0;j<no_res;j++)  
       if(avail[0][j]<need[i][j])
          return false;
     return true;
    }

    public void Safe_system()
    {
     Scanner scan1=new Scanner(System.in);   
     System.out.print("Enter no. of processes and resources : ");   
     no_proc=scan1.nextInt();  //no. of process
     no_res=scan1.nextInt();  //no. of resources
     need=new int[no_proc][no_res];  
     max=new int[no_proc][no_res];
     allocate=new int[no_proc][no_res];
     avail=new int[1][no_res];
     seq = new int[no_proc];
     s = new boolean[no_proc];
     
     System.out.println("Enter allocation matrix:");
     for(int i=0;i<no_proc;i++)
          for(int j=0;j<no_res;j++)
         allocate[i][j]=scan1.nextInt();  //allocation matrix
      
     System.out.println("Enter max matrix:");
     for(int i=0;i<no_proc;i++)
          for(int j=0;j<no_res;j++)
         max[i][j]=scan1.nextInt();  //max matrix
      
        System.out.println("Enter available matrix:");
        for(int j=0;j<no_res;j++)
         avail[0][j]=scan1.nextInt();  //available matrix
        
        scan1.close();
        
       need_matrix();
       System.out.println("adfadfadsf");
       boolean Finish[]=new boolean[no_proc]; //initialised to false
       int j=0;
       int a=0;
       for(j=0;j<no_proc;j++)
           s[j]=false;
       j=0;
       /*if(!Finish[0])
           System.out.println("al;dfja;ldjsf;l*/
       while(j<no_proc)   //until all process allocated
       {  
            boolean allocated=false;
            for(int i=0;i<no_proc;i++)
                if(!Finish[i] && check(i))
                {  
                    for(int k=0;k<no_res;k++)
                        avail[0][k]=avail[0][k]-need[i][k]+max[i][k];
                    //System.out.println(i);
                    seq[a]=i;
                    a=a+1;
                    allocated=Finish[i]=true;
                    j++;
                }
            if(!allocated) break;  //if no allocation
       }
       if(j==no_proc)  //if all process are allocated then a safe sequence exist
       {  
            System.out.println("\nSafely Sequence is there");
            for(j=0;j<a;j++)
                System.out.print(seq[j]+" ");
       }
       else{
            System.out.println("Posibility of deadlock\n");
            if(a!=0){
            for(j=0;j<a;j++)
               s[seq[j]]=true;}
            for(j=0;j<no_proc;j++)
              if(!s[j])
               System.out.println("Process "+j+" is creating problem");           
           }
       
    }
    
    public static void main(String[] args) 
    { 
        new Bankers().Safe_system();
    }
}