/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package jp.co.wap.exam;

import jp.co.wap.exam.lib.Interval;
import java.util.*;
//import jp.co.wap.exam.Problem2;
//import jp.co.wap.exam.Problem1;
import jp.co.wap.exam.PersistentQueue;
/**
 *
 * @author THeHeCtor
 */
public class main {
    public static void main( String[] args ) {
        PersistentQueue<Integer> p = new PersistentQueue<Integer>();
        PersistentQueue<Integer> p1 = new PersistentQueue<Integer>();

        PersistentQueue<Integer> p3 = new PersistentQueue<Integer>();
        //System.out.println(p.size());
	p=p.enqueue(1);
        p = p.enqueue(2);
                p = p.enqueue(3);
                p = p.enqueue(4);
		
		p1 = p.dequeue();
		p1 = p1.enqueue(5);
               p = p.enqueue(6);
                
                
                
		System.out.println(p.peek());
                System.out.println(p.size());
		
    }    
}
