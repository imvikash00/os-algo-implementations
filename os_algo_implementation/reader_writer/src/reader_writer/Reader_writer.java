/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package reader_writer;
import java.util.concurrent.*;
/**
 *
 * @author THeHeCtor
 */
public class Reader_writer {
    
   
    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // TODO code application logic here
        readers_writer w1 = new readers_writer(0);   //writer object
        for(int i=0;i<2;i++){
        readers_writer r1 = new readers_writer(1);  //reader object
        }
        
    }
    
    
}
