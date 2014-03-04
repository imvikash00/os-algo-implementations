/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package reader_writer;

import java.util.concurrent.Semaphore;

/**
 *
 * @author THeHeCtor
 */
    public class readers_writer implements Runnable{
     public static Semaphore wrt = new Semaphore(1,true);//semaphore for writer
     public static Semaphore red =  new Semaphore(1,true);//semaphore for reader
     int bool;
        static int rcount=0; //no. of readers
        static int a=0;  //shared variable
        readers_writer(int val){  //constructor
            this.bool=val;
           new Thread (this, "readers_writer").start();  //satartin a new thread based on bool variable
        }
        
    public void run()   //implementation 0f reader and writer funcitoon
    {
        if(this.bool==1)
        {
           do {
               try{
                red.acquire();   //acquiring the lock on reader sema
               }catch(Exception e){System.out.println("Lightthiskora!! part-1");}
                rcount++;        //incrementing the no of readers
                if (rcount == 1){
                    try{
                wrt.acquire();   ////acquiring the lock on writer sema
               }catch(Exception e){System.out.println("Lightthiskora!! part-2");}
                    
                }
                try{
                red.release();   //release the lock on reader sema
               }catch(Exception e){System.out.println("Lightthiskora!! part-3");}

    /*reading is performed*/
                System.out.println("Reader is reading the value "+ a);
               try{ Thread.sleep(100);}catch(Exception e){};
                try{
                red.acquire();  //acquiring the lock on reader sema
               }catch(Exception e){System.out.println("Lightthiskora!! part-4");}
                rcount--;
                if (rcount == 0){
                    try{
                wrt.release();   //release the lock on reader sema
               }catch(Exception e){System.out.println("Lightthiskora!! part-5");}
                }
                try{
                red.release();    //release the lock on writer sema
               }catch(Exception e){System.out.println("Lightthiskora!! part-6");}
            }while(true);
  //reader
        }
        else{
        do {

            try{
                wrt.acquire();
               }catch(Exception e){System.out.println("Lightthiskora!! part-7");}
 
    a++;
    System.out.println("Writer's value"+ a);
  
    try{
                wrt.release();
               }catch(Exception e){System.out.println("Lightthiskora!! part-8");}

}while(true);

                              //writer
        }
            
    
    }
}
