/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package diners;


/**
 *
 * @author THeHeCtor
 */
import java.util.Random;


class Diners 
{
	public static void main(String args[]) 
        { 
	   Monitor m = new Monitor(); // thing begins here
	   Timer t = new Timer(m);  // make a new timer
	   Phil p[] = new Phil[5];  // make an array of 5 refs to Philosopher
	   for(int i=0; i<5; i++)
	      p[i] = new Phil(i,m,t); // create the philosopher and start them
	}
}
	class Phil implements Runnable 
        {
	Monitor m;
	Timer t;
	int pId;
	float time;
	Phil(int pId, Monitor m, Timer t)  // constructor
        { 
	   this.pId = pId;
	   this.m = m;
	   this.t = t;
	   new Thread(this, "Phil").start(); // make a new thread and start it
	}
	public void run() {   // must override run, this is what
	   for(int i=0; i<2; i++)  // is executed when the thread starts
           {  
	     m.ask_to_eat(pId);  // running
	     time = 1000;
	     try {Thread.sleep((int)time);} catch(Exception e){}
	     m.ask_to_leave(pId);
	     time = 1000;
	     try {Thread.sleep((int)time);} catch(Exception e){}
	   }
	t.report_Stop();   
	}       
	}
	class Timer implements Runnable 
        {
	Monitor m;
	int completed;
	Timer(Monitor m) {   // constructor
	   this.m = m;
	   new Thread(this, "Tim").start(); // make a new thread and start it
	   completed=0;
	}
	public void report_Stop() 
        {
	   completed++;
	}
	public void run() 
        {   // must override run(), this is 
	   while(completed!=5) {  // what happens when the thread
	      m.print_State();  // begins
	      try {Thread.sleep(500);} catch(Exception e){}
	   }
	}
	}
