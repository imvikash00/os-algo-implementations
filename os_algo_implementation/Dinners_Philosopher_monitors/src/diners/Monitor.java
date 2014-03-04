/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package diners;

/**
 *
 * @author THeHeCtor
 */
class Monitor {
	int phil_States[] = new int[5];   // 0=thinking, 1=waiting // 2=eating
	boolean fork_States[] = new boolean[5];  // false = in use, true = free
	Monitor() {        // constructor
	   for(int i=0;i<5;i++) {
	      phil_States[i]=0;
	      fork_States[i]=true;
	   }
	}
	synchronized void print_State()
	{
	System.out.println();   
        String val;// newline
	for(int i=0;i<5;i++){
            if(phil_States[i]==0)
                val="Thinking";
            else if(phil_States[i]==1)
                 val="Waiting";
            else
                val="Eating";
	     System.out.println("Philosopher "+ i + " is "+ val);        
	}
        }
	synchronized void ask_to_eat(int pId)
	{
	   while(!fork_States[pId] || !fork_States[(pId+1)%4])
	   {    // while it can't have both forks, wait
	      phil_States[pId] = 1;  
	      try 
	  {wait();} 
	  catch(InterruptedException e){} // it gets released
	   }    // by a process doing a call to notify()
	   phil_States[pId] = 2;  // eating
	   fork_States[pId] = false;  // in use
	   fork_States[(pId+1)%4] = false;
	}
	synchronized void ask_to_leave(int pId)
	{
	     fork_States[pId] = true; // available
	     fork_States[(pId+1)%4] = true;
	     phil_States[pId] = 0;  // thinking
	     notify(); // free the Phil that has waited the longest
	}
}
	 