package jp.co.wap.exam;
 /*
 * @author Vikash Kumar
 */
import java.util.*;

/**
 * The Queue class represents an immutable first-in-first-out (FIFO) queue of objects.
 * @param <E>
 * 
 */
public class PersistentQueue<E> {
        
        // Node class that represents the structure of the node in linked list
        private class Node<E> {
                E val;
                Node<E> next;
                
                Node(E e) {
                        val = e;
                        next = null;
                }
        }
        
        //track head and tail for O(1) insert and delete
        private Node<E> head;
        private Node<E> tail;
        private int size;

        private List<Node<E>> extHeads;
        private List<Node<E>> extTails;
        
        //default constructor 
        public PersistentQueue() {
                head = null;
                tail = null;
                size = 0;
                extHeads = new ArrayList<>();
                extTails = new ArrayList<>();
        }
        
        //  constructor to return while enqueuing
        private PersistentQueue(Node<E> h, Node<E> t, E e, int n, List<Node<E>> bh, List<Node<E>> bt) {
                extHeads = new ArrayList<>(bh);
                extTails = new ArrayList<>(bt);
                tail = new Node<>(e);
                if(t != null) {
                        // if next pointer of tail has already been assigned, do branching
                        if(t.next == null){
                                t.next = tail;
                                //System.out.println("adfadsfasdfa");
                        }       
                        else {
                            System.out.println("adfadsfasdfa");
                                extTails.add(t);
                                extHeads.add(tail);
                        }
                }
                head = h;
                if(head == null)
                        head = tail;
                size = n;
        }
        
        //  constructor to return in dequeue
        private PersistentQueue(Node<E> h, Node<E> t, int n, List<Node<E>> bh, List<Node<E>> bt) {
                head = h;
                tail = t;
                if(h == null)
                        tail = head;
                size = n;
                extHeads = new ArrayList<>(bh);
                extTails = new ArrayList<>(bt);
        }
        
        /**
         * Returns the queue that adds an item into the tail of this queue without modifying this queue.
         * <pre>
         * e.g.
         * When this queue represents the queue (2, 1, 2, 2, 6) and we enqueue the value 4 into this queue,
         * this method returns a new queue (2, 1, 2, 2, 6, 4)
         * and this object still represents the queue (2, 1, 2, 2, 6) .
         * </pre>
         * If the element e is null, throws IllegalArgumentException.
         * @param e
         * @return
         * @throws IllegalArgumentException
         */
        public PersistentQueue<E> enqueue(E e) {
                if(e == null)
                        throw new IllegalArgumentException();
                return new PersistentQueue<>(head, tail, e, size + 1, extHeads, extTails);
        }
        /**
         * Returns the queue that removes the object at the head of this queue without modifying this queue.
         * <pre>
         * e.g.
         * When this queue represents the queue (7, 1, 3, 3, 5, 1) ,
         * this method returns a new queue (1, 3, 3, 5, 1)
         * and this object still represents the queue (7, 1, 3, 3, 5, 1) .
         * </pre>
         * if the tail of this queue has already been assigned next
         * do branching, rather than reassigning the next pointer (to preserve immutability)
         * If this queue is empty, throws java.util.NoSuchElementException.
         * @return
         * @throws NoSuchElementException
         */
        public PersistentQueue<E> dequeue() {
                if(size == 0)
                        throw new NoSuchElementException();
                // check if branching has happened and we have reached a branch
                if(!extTails.isEmpty() && extTails.get(0) == head) {
                    System.out.println("adfadsfasdfa111");
                        return new PersistentQueue(extHeads.get(0), tail, size - 1, extHeads.subList(1, extHeads.size()), extTails.subList(1, extTails.size()));
                }
                return new PersistentQueue(head.next, tail, size - 1, extHeads, extTails);
        }
        
        /**
         * Looks at the object which is the head of this queue without removing it from the queue.
         * <pre>
         * e.g.
         * When this queue represents the queue (7, 1, 3, 3, 5, 1),
         * this method returns 7 and this object still represents the queue (7, 1, 3, 3, 5, 1)
         * </pre>
         * If the queue is empty, throws java.util.NoSuchElementException.
         * @return
         * @throws NoSuchElementException
         */
        public E peek() {
                if(size == 0)
                        throw new NoSuchElementException();
                return head.val;
        }
        
        /**
         * Returns the number of objects in this queue.
         * @return
         */
        public int size() {
                return size;
        }
}