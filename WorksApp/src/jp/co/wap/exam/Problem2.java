/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package jp.co.wap.exam;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;
import jp.co.wap.exam.lib.Interval;
/**
 *
 * @author Vikash Kumar
 */
public class Problem2 {
    
    public int getMaxWorkingTime(List<Interval> intervals){
        //if the list is empty return 0
        if(intervals==null || intervals.isEmpty() ) {
            return 0;
        } 
        
        /*Sorting the objects in the list based on the ending point and if endpoint are same then 
         * on the basis of starttime
         */
        Collections.sort( intervals, new Comparator<Interval>() {
            @Override
            public int compare( Interval o1, Interval o2 ) {
                if(o1.getEndMinuteUnit() > o2.getEndMinuteUnit())
                    return 1;
                else if(o1.getEndMinuteUnit() == o2.getEndMinuteUnit())
                {
                    if(o1.getBeginMinuteUnit() < o2.getBeginMinuteUnit())
                        return -1;
                    else 
                        return 1;
                }
                else
                    return -1;
            }
        } );
        
        int n=intervals.size();
        int i,j=0;
        int[] workdone = new int[n]; //maximum work that can be assigned till ith interval
        
        
        /*for (Interval number :intervals) {
             System.out.println(number.getBeginHour()+":"+number.getBeginMinute()+"--"+number.getEndHour()+":"+number.getEndMinute() );
        } */
        
        
        workdone[0]=intervals.get(0).getIntervalMinute();
        int add;
        
        /*Filling workdone[] using dynamic progrmming
         * workdone[i] = max((length_of_interval[i] + workdone[i*]),workdone[i-1])
         * 
         * where i* is the index of the interval object whose endtime is less startime[i] and is closest to current interval
         * getPrevInterval() gives the i*
         */
        for(i=1;i<n;i++)
        {
            j=getPrevInterval(i,n,intervals);
            if(j==-1)   //if no such interval is there then add 0 to length_of_interval
                add=0;
            else
                add=workdone[j];
                 
            workdone[i]=Math.max((intervals.get(i).getIntervalMinute()+add),workdone[i-1]);
        }
        return workdone[n-1];
    }
    
    
    public static int getPrevInterval(int a,int size,List<Interval> intervals)
    {
        int j=-1,k,minIndex=0;
        int start=intervals.get(a).getBeginMinuteUnit();
        for(k=0;k<size;k++)
        {
            if(intervals.get(k).getEndMinuteUnit() < start)
            {
                if(intervals.get(minIndex).getEndMinuteUnit()<=intervals.get(k).getEndMinuteUnit())
                    j=k;
            }
        }
        return j;
    }
     
}
