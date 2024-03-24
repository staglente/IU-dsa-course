#include <iostream>
#include <assert.h>
using namespace std;

/*template <class T>
struct Node{
    T value;
    Node<T>* next;
    Node(){
        value;
        next = nullptr;
    }
};

template <class T>
class IlinkedList{
public:
    virtual void insertToRear(T data) = 0; // add node to the rear
    virtual T getFirst() = 0; // get the first element in the list
    virtual int size() = 0; // get size of the list
    virtual T removeFirst() = 0; // delete the first element in the list
};

template <class T>
class LinkedList: IlinkedList<T>{
private:
    Node<T> *head = nullptr;
    int len = 0;
public:
    void insertToRear(T data){
        if(head == nullptr){
            head = new Node<T>;
            head->value = data;
            head->next = nullptr;
        }
        else{
            Node<T> *temp = head;
            while(temp->next != nullptr)
                temp = temp->next;
            auto *newNode = new Node<T>();
            newNode->value = data;
            newNode->next = nullptr;
            temp->next = newNode;
        }
        len++;
    }
    T getFirst(){
        return head->value;
    }

    int size(){
        return len;
    }

    T removeFirst(){
        Node<T> *temp = head;
        T val = temp->value;
        head = head->next;
        delete temp;
        len--;
        return val;
    }


};

template <class T>
class ICircularBoundedQueue{
public:
    virtual void offer(T value) = 0; // insert element to the rear of the queue
    virtual T poll() = 0; // remove the oldest element
    virtual T peek() = 0; // look on the first element in the queue
    virtual void flush() = 0; // remove all elements in the queue
    virtual bool isEmpty() = 0; // is the queue empty ?
    virtual bool isFull() = 0; // is the queue full ?
    virtual int size() = 0; // number of elements in the queue
    virtual int capacity() = 0; // maximal length of the queue
};

template <class T>
class CircularBoundedQueue: ICircularBoundedQueue<T>{
private:
    LinkedList<T> elements;
    int capacity_ = 0;
public:
    void setCapacity(int c){
        capacity_ = c;
    }
    void offer(T value){
        if(elements.size() < capacity_)
            elements.insertToRear(value);
        else{
            elements.removeFirst();
            elements.insertToRear(value);
        }
    }
    T poll(){
        assert(elements.size() > 0);
        T temp = elements.getFirst();
        elements.removeFirst();
        return temp;
    }
    T peek(){
        assert(elements.size() > 0);
        return elements.getFirst();
    }
    void flush(){
        while(elements.size() > 0)
            elements.removeFirst();
    }
    bool isEmpty(){
        return (elements.size() == 0);
    }
    bool isFull(){
        return (elements.size() == capacity_);
    }
    int size(){
        return elements.size();
    }
    int capacity(){
        return capacity_;
    }
}; */
/*
template <class T>
class ICircularBoundedStack{
    virtual void push(T value) = 0; // add element onto stack
    virtual T pop() = 0; // remove the last element from the stack
    virtual T top() = 0; // get the last element from the stack
    virtual void flush() = 0; // remove all elements from the stack
    virtual bool isEmpty() = 0; // is the stack empty?
    virtual bool isFull() = 0; // is the stack full?
    virtual int size() = 0; // number of elements in the stack
    virtual int capacity() = 0; // maximal number of elements in the stack;
};
template <class T>
class CircularBoundedStack: ICircularBoundedStack<T>{
private:
    CircularBoundedQueue<T> elements, temp;
    int capacity_ = 0;
public:
    void setCapacity(int c){
        capacity_ = c;
    }
    void push(T value){
        temp.flush();
        temp.offer(value);
        while(!elements.isEmpty()){
            temp.offer(elements.poll());
            temp.poll();
        }
        elements = temp;
    }
    T pop(){
        T val = temp.peek();
        temp.poll();
        return val;
    }
    T top(){
        return temp.peek();
    }
    void flush(){
        elements.flush();
    }
    bool isEmpty(){
        return elements.isEmpty();
    }
    bool isFull(){
        return (elements.size() == capacity_);
    }
    int size(){
        return elements.size();
    }
    int capacity(){
        return capacity_;
    }
}; */

template <class T>
int hash1(T obj){
    int ht = hash<T>{}(obj);
    return abs(ht);
}
template <class T>
int hash2(T obj){
    int a = 7, b = 257;
    int ht = (a * hash1(obj) + b) % 5000251;
    return ht;
}

template<class T>
class ISet{
public:
    virtual void add(T item) = 0; // add item in the set
    virtual void remove(T item) = 0; // remove item from the set
    virtual bool contains(T item) = 0; // check if item belongs to the set
    virtual int size() = 0; // number of elements in the set
    virtual bool isEmpty() = 0; // check if set is empty
};

template<class T>
class DoubleHashSet: ISet<T>{
private:
    int search(T item){
        int ht = hash2(item);
        while(hashes[ht]){
            if(elements[ht] == item)
                return ht;
            ht = hash2(ht);
        }
        return -1;
    }
public:
    T* elements;
    bool* hashes;
    int len = 0;
public:
    DoubleHashSet(){
        elements = new T[5000251];
        hashes = new bool[5000251];
    }
    ~DoubleHashSet(){
        delete[] elements;
        delete[] hashes;
    }
    bool contains(T item){
        return (search(item) > -1);
    }
    void add(T item){
        if(contains(item)){
            cout << "ERROR: cannot execute NEW" << item << "\n";
            return;
        }
        int ht = hash2(item);
        while(hashes[ht])
            ht = hash2(ht);
        hashes[ht] = true;
        elements[ht] = item;
        len++;
    }
    void remove(T item){
        //assert(len > 0);
        if(len <= 0 || !contains(item)){
            cout << "ERROR: cannot execute REMOVE " << item << "\n";
            return;
        }
        int ht = search(item);
        hashes[ht] = false;
        len--;
    }
    int size(){
        return len;
    }
    bool isEmpty(){
        return (len == 0);
    }
};

/*void solveA(){
    int n, k;
    cin >> n >> k;
    CircularBoundedQueue<string> cq;
    cq.setCapacity(k);
    string str;
    getline(cin, str);
    for(int i = 0; i < n; i++){
        getline(cin, str);
        cq.offer(str);
    }
    while(!cq.isEmpty())
        cout << cq.poll() << "\n";
} */

void solveB(){
    int n;
    cin >> n;
    DoubleHashSet<string> s;
    for(int i = 0; i < 5000251; i++)
        s.hashes[i] = false;
    while(n--){
        string t;
        cin >> t;
        if(t == "NEW"){
            string name;
            cin >> name;
            if(name.back() == '/'){
                string tName = name;
                name.erase(name.length() - 1, 1);
                if(s.contains(name) || s.contains(tName))
                    cout << "ERROR: cannot execute " << t << " " << tName << "\n";
                else
                    s.add(tName);
            }
            else{
                string tName = name;
                name += '/';
                if(s.contains(name) || s.contains(tName))
                    cout << "ERROR: cannot execute " << t << " " << tName << "\n";
                else
                    s.add(tName);
            }

        }
        else if(t == "REMOVE"){
            string name;
            cin >> name;
            s.remove(name);
        }
        else if(t == "LIST"){
            for(int i = 0; i < 5000251; i++)
                if(s.hashes[i])
                    cout << s.elements[i] << " ";
        }
    }
}

signed main(){
    solveB();
}
