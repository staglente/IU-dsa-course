package Dsa;


import java.util.ArrayList;
import java.util.Scanner;

import static java.util.Collections.swap;

public class Main{
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int n = sc.nextInt();
        MinimumPriorityQueue<Integer, String> testQueue = new MinimumPriorityQueue<>();
        for(int i = 0; i <= n; i++){
            String inputLine = sc.nextLine();
            if(inputLine.isEmpty()) continue;
            String[] line = inputLine.split(" ");
            if(line[0].equals("ADD"))
                testQueue.insert(new Node<>(Integer.parseInt(line[2]), line[1]));
            else if(line[0].equals("PRINT_MIN"))
                System.out.println(testQueue.extractMin().getValue());
        }
    }
}

/**
 * This class represents node data-type which consists of two linked generic elements K-typed key and V-typed value
 */

class Node<K extends Comparable<K>, V extends Comparable<V>>{
    private K key;
    private final V value;

    public Node(K key_, V value_){
        key = key_;
        value = value_;
    }

    public K getKey(){
        return key;
    }
    public V getValue(){
        return value;
    }
    public void setKey(K key_){
        key = key_;
    }

    public boolean isLess(Node<K, V> anotherNode){ // check if current node is less than another one
        return (key.compareTo(anotherNode.getKey()) < 0 || (key.compareTo(anotherNode.getKey()) == 0 && value.compareTo(anotherNode.getValue()) < 0));
    }

    public boolean isEqual(Node<K, V> anotherNode){ // check if current node is equal to another one
        return (key.compareTo(anotherNode.getKey()) == 0 && value.compareTo(anotherNode.getValue()) == 0);
    }
}

interface IPriorityQueue<K, V>{
    void insert(Object item); // insert item in the right position in priority queue
    Object findMin(); // return minimum element in the heap
    Object extractMin(); // return and remove minimum in the heap
    void decreaseKey(Object item, K key); // set new key-value to the element in the heap with not less current key
    void delete(Object item); // remove element from the heap
    void union(Object anotherQueue); // merge two heaps into one
}

class MinimumPriorityQueue<K extends Comparable<K>, V extends Comparable<V>> implements IPriorityQueue<K, V>{
    private ArrayList<Node<K, V>> heap; // array-based heap

    public MinimumPriorityQueue(){
        heap = new ArrayList<>();
    }

    public int parent(int index){ // find index of the current node's parent
        return (index - 1) / 2;
    }
    public int left(int index){ // find index of the current node's left child
        return 2 * index + 1;
    }
    public int right(int index){ // find index of the current node's right child
        return 2 * index + 2;
    }


    void minHeapify(int index){ // pull current node downwards till it reaches right position
        int leftChildIndex = left(index);
        int rightChildIndex = right(index);
        int leastIndex = index;
        if(leftChildIndex < heap.size() && heap.get(leftChildIndex).isLess(heap.get(leastIndex)))
            leastIndex = leftChildIndex;
        if(rightChildIndex < heap.size() && heap.get(rightChildIndex).isLess(heap.get(leastIndex)))
            leastIndex = rightChildIndex;
        if(leastIndex != index){
            swap(heap, index, leastIndex);
            minHeapify(leastIndex);
        }
    }

    void fixUpwards(int i){ // pull current node upwards till it reaches right position
        while(i > 0 && (heap.get(i).isLess(heap.get(parent(i))))){
            swap(heap, i, parent(i));
            i = parent(i);
        }
    }

    public Node<K, V> findMin(){
        return heap.get(0);
    }

    /**
     * This function finds index of the given object in a heap recursively. We start checking from the root.
     * If current node is equal to ours than return its index
     * If current node is leaf and is not equal to needed node than we return -1 as a sign of non-existence because there is no more data in current subtree
     * If current node is bigger than needed than we return -1 too because according to the minimum heap rule all elements in subtree of current node are not less than it, so it's redundant to check this subtree
     * In another case we check subtrees with roots of our current node's children if they exist
     */
    int findIndexByObject(Node<K, V> node, int index){
        if(node.isLess(heap.get(index)) || left(index) >= heap.size()) // if we reach leaf of node that is bigger than needed node - end recursion with non-found "-1" exit
            return -1;
        if(node.isEqual(heap.get(index))) // we found it!
            return index;
        if(right(index) < heap.size()) // current node has two children - check both of them
            return Math.max(findIndexByObject(node, left(index)), findIndexByObject(node, right(index)));
        return findIndexByObject(node, left(index)); // check only child of current node
    }

    public void delete(Object o){
        Node<K, V> node = (Node<K, V>)o;
        int index = findIndexByObject(node, 0);
        if(index < 0)
            return;
        swap(heap, index, heap.size() - 1);
        heap.remove(heap.size() - 1);
        minHeapify(index);
    }

    public Node<K, V> extractMin(){
        Node<K, V> res = heap.get(0);
        delete(heap.get(0));
        return res;
    }

    public void insert(Object o){
        Node<K, V> node = (Node<K, V>)o;
        heap.add(node);
        fixUpwards(heap.size() - 1);
    }

    public void decreaseKey(Object o, K newKey){
        Node<K, V> node = (Node<K, V>)o;
        int index = findIndexByObject(node, 0);
        if(index >= 0)
            delete(heap.get(index));
        node.setKey(newKey);
        insert(node);
    }

    public void union(Object o){ // merge sort based merging algorithm for two queues
        MinimumPriorityQueue<K, V> anotherQueue = (MinimumPriorityQueue<K, V>)o;
        MinimumPriorityQueue<K, V> res = new MinimumPriorityQueue<>();
        while(!this.heap.isEmpty() && !anotherQueue.heap.isEmpty()){
            Node<K, V> u = findMin();
            Node<K, V> v = anotherQueue.findMin();
            if(u.isLess(v)){
                res.insert(u);
                extractMin();
            }
            else{
                res.insert(v);
                anotherQueue.extractMin();
            }
        }
        while(!this.heap.isEmpty())
            res.insert(this.extractMin());
        while(!anotherQueue.heap.isEmpty())
            res.insert(anotherQueue.extractMin());
        heap = res.heap;
    }
}