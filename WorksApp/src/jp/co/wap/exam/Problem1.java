/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package jp.co.wap.exam;
import java.util.*;
import jp.co.wap.exam.lib.Interval;
/***
 *
 * @author Vikash Kumar
 */
public class Problem1 {
    
    public  int getMaxIntervalOverlapCount(List<Interval> intervals){
        
        //if the list is empty or null return 0
        if(intervals==null || intervals.isEmpty() ) {
            return 0;
        } 
        int n=intervals.size();
        int count=0;
        int[] storecount = new int[2*n];
        int[] starttime = new int[n];   //contains starttime of an interval
        int[] endtime = new int[n];      //contains endtime of an interval
        int[][] sortedArr = new int[n*2][2]; //contains start and end time in sorted order and start time first if both
                                                       // start time and end time are same
        int i=0,j=0,k=0;
        
         /* Loop over the list and store startime and endtime of each interval in the startime and endtime
             array
            */
        for(Interval it1:intervals){ 
           
            starttime[i]=it1.getBeginMinuteUnit();  //storing startime of an interval in startime array
            endtime[i]=it1.getEndMinuteUnit();     //storing startime of an interval in startime array
            i++;
        }

        Arrays.sort(starttime);         //Sorting startime and endtime array
        Arrays.sort(endtime);
        i=0;
         /*for(int m=0;m<n;m++)
        {
            System.out.println(starttime[m]+"   "+endtime[m]);
        }*/
        while(i<n && j<n)
        {
            if(starttime[i]<endtime[j])
            {
                sortedArr[k][0]=starttime[i];
                sortedArr[k][1]=0;           //0 means startime
                k++;i++;
            }
            else if(starttime[i]>endtime[j])
            {
                sortedArr[k][0]=endtime[j];
                sortedArr[k][1]=1;            //1 means endtime
                k++;j++;
            }
            else if(starttime[i]==endtime[j])
            {
                sortedArr[k][0]=starttime[i];
                sortedArr[k][1]=0;
                k++;i++;
            }
        }
        if(i==n && j<n)
        {
            for(;j<n;j++,k++){
                sortedArr[k][0]=endtime[j];
                sortedArr[k][1]=1;
            }
        }
        else if(j==n && i<n)
        {
            for(;i<n;i++,k++){
                sortedArr[k][0]=starttime[i];
                sortedArr[k][1]=0;
            }
        }
        
        
        /*Traverse the sortedArr[][] and when startpoint is encountered increment count and store it in storecount and 
         * similarly when endpoint is encountered decrement count and again store count
         */
        int retVal;
        for(i=0;i<2*n;i++)
        {
            retVal=Arrays.binarySearch(starttime, sortedArr[i][0]);  //cheching if retval is starttime or not
          //  System.out.println(sortedArr[i][0]+"   "+retVal);
            if(retVal >=0 && sortedArr[i][1]==0)
                storecount[i]=++count;        //if starttime then increment count
            else
                storecount[i]= --count;     //else decrement count
        }
        Arrays.sort(storecount);    //sorting to make find of max easy
        /*for(int m=0;m<2*n;m++)
        {
            System.out.println(storecount[m]);
        }*/
    return storecount[storecount.length-1];
            
    }
}
