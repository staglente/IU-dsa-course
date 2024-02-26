package Dsa;

import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class Main{
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int n = sc.nextInt();
        RangeQueries<Integer, Integer> bTree = new RangeQueries<>(123456);
        for(int i = 0; i <= n; i++){
            String inputLine = sc.nextLine();
            if(inputLine.isEmpty()) continue;
            String input[] = inputLine.split(" ");
            if(input.length == 3){
                if(input[1].equals("DEPOSIT"))
                    bTree.add(dateToInt(input[0]), Integer.parseInt(input[2]));
                else
                    bTree.add(dateToInt(input[0]), -Integer.parseInt(input[2]));
            }
            else{
                List<Integer> transactions = bTree.lookupRange(dateToInt(input[2]), dateToInt(input[4]));
                System.out.println(sumOfIntegerList(transactions));
            }
        }
    }

    static int[] amountOfLeaps = new int[10000];
    public static int dateToInt(String s){
        int res = 0;
        String[] input = s.split("-");
        int year = Integer.parseInt(input[0]) - 1;
        res += year * 365 + amountOfLeaps[year];
        int month = Integer.parseInt(input[1]) - 1;
        for(int i = 1; i <= month; i++){
            switch(i){
                case 1:
                case 3:
                case 5:
                case 7:
                case 8:
                case 10:
                    res += 31;
                    break;
                case 2:
                    if((year + 1) % 4 == 0 && (year + 1) % 100 != 0 || (year + 1) % 400 == 0)
                        res += 29;
                    else
                        res += 28;
                    break;
                case 4:
                case 6:
                case 9:
                case 11:
                    res += 30;
                    break;

            }
        }
        return (res + Integer.parseInt(input[2]));
    }
    public static int sumOfIntegerList(List<Integer> a){
        int sum = 0;
        for(int i : a) sum += i;
        return sum;
    }

}

interface RangeMap<K, V>{
    int size();
    boolean isEmpty();
    void add(K key, V value);
    boolean contains(K key);
    V lookup(K key);
    List<V> lookupRange(K from, K to);
}

class RangeQueries<K extends Comparable<K>, V> implements RangeMap<K, V>{
    private int T; // b-tree degree
    private RangeNode<K, V> root;
    private int numberOfKeys = 0;

    public RangeQueries(int t){
        T = t;
        root = new RangeNode<>(t);
        root.n = 0;
        root.isLeaf = true;
    }

    static class Node<K, V>{
        private K key;
        private V value;

        public Node(K key, V value){
            this.key = key;
            this.value = value;
        }
    }

    static class RangeNode<K, V>{
        private int n;
        boolean isLeaf = true;
        private RangeNode<K, V>[] child;
        private Node<K, V>[] element;

        public RangeNode(int degree){
            this.child = new RangeNode[2 * degree];
            this.element = new Node[2 * degree - 1];
        }

    }

    @Override
    public int size(){
        return numberOfKeys;
    }

    @Override
    public boolean isEmpty(){
        return size() > 0;
    }

    private V search(RangeNode<K, V> curRangeNode, K key){
        if(curRangeNode == null)
            return null;
        int i = 0;
        for(; i < curRangeNode.n; i++){
            if(key.compareTo(curRangeNode.element[i].key) < 0)
                break;
            if(key.compareTo(curRangeNode.element[i].key) == 0)
                return curRangeNode.element[i].value;
        }
        if(curRangeNode.isLeaf)
            return null;
        return search(curRangeNode.child[i], key);
    }

    private void split(RangeNode<K, V> curRangeNode, int index, RangeNode<K, V> anotherCurRangeNode){
        RangeNode<K, V> tempRangeNode = new RangeNode<>(T);
        tempRangeNode.isLeaf = anotherCurRangeNode.isLeaf;
        tempRangeNode.n = T - 1;
        for(int i = 0; i < T - 1; i++)
            tempRangeNode.element[i] = anotherCurRangeNode.element[i + T];
        if(!anotherCurRangeNode.isLeaf)
            for(int i = 0; i < T; i++)
                tempRangeNode.child[i] = anotherCurRangeNode.child[i + T];
        anotherCurRangeNode.n = T - 1;
        for(int i = curRangeNode.n; i >= index + 1; i--)
            curRangeNode.child[i + 1] = curRangeNode.child[i];
        curRangeNode.child[index + 1] = tempRangeNode;
        for(int i = curRangeNode.n - 1; i >= index; i--)
            curRangeNode.element[i + 1] = curRangeNode.element[i];
        curRangeNode.element[index] = anotherCurRangeNode.element[T - 1];
        curRangeNode.n++;
    }

    private void insert(RangeNode<K, V> curRangeNode, Node<K, V> k){
        if(curRangeNode.isLeaf){
            int i = curRangeNode.n - 1;
            while(i >= 0 && k.key.compareTo(curRangeNode.element[i].key) < 0){
                curRangeNode.element[i + 1] = curRangeNode.element[i];
                i--;
            }
            curRangeNode.element[i + 1] = k;
            curRangeNode.n++;
        }
        else{
            int i = curRangeNode.n - 1;
            while(i >= 0 && k.key.compareTo(curRangeNode.element[i].key) < 0)
                i--;
            i++;
            RangeNode<K, V> tempRangeNode = curRangeNode.child[i];
            if(tempRangeNode.n == 2 * T - 1){
                split(curRangeNode, i, tempRangeNode);
                if(k.key.compareTo(curRangeNode.element[i].key) > 0)
                    i++;
            }
            insert(curRangeNode.child[i], k);
        }
    }

    @Override
    public void add(K key, V value){
        Node<K, V> node = new Node<>(key, value);
        insert(root, node);
    }

    @Override
    public boolean contains(K key){
        return search(root, key) != null;
    }

    @Override
    public V lookup(K key){
        return search(root, key);
    }

    private List<V> goThrough(K from, K to, RangeNode<K, V> curRangeNode){
        if(curRangeNode == null)
            return null;
        int i = 0;
        List<V> ans = new ArrayList<>();
        for(; i < curRangeNode.n; i++){
            if(curRangeNode.element[i].key.compareTo(from) < 0)
                continue;
            if(curRangeNode.element[i].key.compareTo(to) > 0)
                break;
            if(!curRangeNode.isLeaf)
                ans.addAll(goThrough(from, to, curRangeNode.child[i]));
            ans.add(curRangeNode.element[i].value);
        }
        if(!curRangeNode.isLeaf)
            ans.addAll(goThrough(from, to, curRangeNode.child[i]));
        return ans;

    }

    @Override
    public List<V> lookupRange(K from, K to){
        return goThrough(from, to, root);
    }

}

