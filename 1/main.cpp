#include <iostream>

using namespace std;

template <class T>
struct Node{
    T value;
    Node<T>* next = nullptr;
    Node(){
        next = nullptr;
    }
};

template <class T>
class IlinkedList{
public:
    virtual void insertToRear(T data) = 0; // add node to the rear O(n)
    virtual T getFirst() = 0; // get the first element in the list O(1)
    virtual int size() = 0; // get size of the list O(1)
    virtual T removeFirst() = 0; // delete the first element in the list O(1)
};

template <class T>
class LinkedList: IlinkedList<T>{
private:
    Node<T> *head = nullptr;
    int len = 0;
public:
    LinkedList(){
        head = nullptr;
        len = 0;
    }
    LinkedList(const LinkedList& other) = delete;
    void insertToRear(T data) override {
        if(head == nullptr){
            head = new Node<T>();
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
    T getFirst() override{
        return head->value;
    }

    int size() override{
        return len;
    }

    T removeFirst() override{
        T val = head->value;
        head = head->next;
        len--;
        return val;
    }

};

template <class T>
class ICircularBoundedQueue{
public:
    virtual void offer(T value) = 0; // insert element to the rear of the queue O(n)
    virtual T poll() = 0; // remove the oldest element O(1)
    virtual T peek() = 0; // look on the first element in the queue O(1)
    virtual void flush() = 0; // remove all elements in the queue O(n)
    virtual bool isEmpty() = 0; // is the queue empty ? O(1)
    virtual bool isFull() = 0; // is the queue full ? O(1)
    virtual int size() = 0; // number of elements in the queue O(1)
    virtual int capacity() = 0; // maximal length of the queue O(1)
};

template <class T>
class CircularBoundedQueue: ICircularBoundedQueue<T>{
private:
    LinkedList<T> elements;
    int capacity_;
public:
    CircularBoundedQueue(){
        elements = LinkedList<T>();
        capacity_ = 0;
    }
    void setCapacity(int c){
        capacity_ = c;
    }
    void offer(T value) override{
        if(elements.size() < capacity_)
            elements.insertToRear(value);
        else{
            elements.removeFirst();
            elements.insertToRear(value);
        }
    }
    T poll() override{
        T temp = elements.getFirst();
        elements.removeFirst();
        return temp;
    }
    T peek() override{
        return elements.getFirst();
    }
    void flush() override{
        while(elements.size() > 0)
            elements.removeFirst();
    }
    bool isEmpty() override{
        return (elements.size() == 0);
    }
    bool isFull() override{
        return (elements.size() == capacity_);
    }
    int size() override{
        return elements.size();
    }
    int capacity() override{
        return capacity_;
    }
};


template <class T>
class IQueuedBoundedStack{
    virtual void push(T value) = 0; // add element onto stack O(n)
    virtual T pop() = 0; // remove the last element from the stack O(1)
    virtual T top() = 0; // get the last element from the stack O(1)
    virtual void flush() = 0; // remove all elements from the stack O(n)
    virtual bool isEmpty() = 0; // is the stack empty? O(1)
    virtual bool isFull() = 0; // is the stack full? O(1)
    virtual int size() = 0; // number of elements in the stack O(1)
    virtual int capacity() = 0; // maximal number of elements in the stack; O(1)
};
template <class T>
class QueuedBoundedStack: IQueuedBoundedStack<T>{
private:
    CircularBoundedQueue<T> elements;
    CircularBoundedQueue<T> temp;
    int capacity_ = 0;
    int len = 0;
public:
    QueuedBoundedStack(){
        elements = CircularBoundedQueue<T>();
        temp = CircularBoundedQueue<T>();
    }
    void setCapacity(int c){
        elements.setCapacity(c);
        temp.setCapacity(c);
        capacity_ = c;
    }
    void push(T value) override{
        temp.offer(value);
        while(!elements.isEmpty()){
            temp.offer(elements.poll());
        }
        elements = temp;
        temp.flush();
        len++;
    }
    T pop() override{
        T val = elements.peek();
        elements.poll();
        return val;
    }
    T top() override{
        return elements.peek();
    }
    void flush() override{
        elements.flush();
    }
    bool isEmpty() override{
        return elements.isEmpty();
    }
    bool isFull() override{
        return (elements.size() == capacity_);
    }
    int size() override{
        return elements.size();
    }
    int capacity() override{
        return capacity_;
    }
};

template <class T>
int hash1(T obj){
    int ht = hash<T>{}(obj);
    return abs(ht);
}

template <class T>
int hash2(T obj){
    int a = 7, b = 257;
    int ht = (a * hash1(obj) + b) % 10001;
    return ht;
}

template<class T>
class ISet{
public:
    virtual void add(T item) = 0; // add item in the set O(1)
    virtual void remove(T item) = 0; // remove item from the set O(1)
    virtual bool contains(T item) = 0; // check if item belongs to the set O(1)
    virtual int size() = 0; // number of elements in the set O(1)
    virtual bool isEmpty() = 0; // check if set is empty O(1)
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
    T* elements;
    bool* hashes;
    int len = 0;
public:
    DoubleHashSet(){
        elements = new T[10001];
        hashes = new bool[10001];
        for(int i = 0; i < 10001; i++)
            hashes[i] = false;
    }

    ~DoubleHashSet(){
        delete[] elements;
        delete[] hashes;
    }
    T getElement(int pos){
        return elements[pos];
    }
    bool getHash(int pos){
        return hashes[pos];
    }
    bool contains(T item) override{
        return (search(item) > -1);
    }
    void add(T item) override{
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
    void remove(T item) override{
        if(len <= 0 || !contains(item)){
            cout << "ERROR: cannot execute REMOVE " << item << "\n";
            return;
        }
        int ht = search(item);
        hashes[ht] = false;
        len--;
    }
    int size() override{
        return len;
    }
    bool isEmpty() override{
        return (len == 0);
    }
};

void solveA(){
    int n, k;
    cin >> n >> k;
    auto cq = CircularBoundedQueue<string>();
    cq.setCapacity(k);
    string str;
    getline(cin, str);
    for(int i = 0; i < n; i++){
        getline(cin, str);
        cq.offer(str);
    }
    while(!cq.isEmpty())
        cout << cq.poll() << "\n";
}

void solveB(){
    int n;
    cin >> n;
    auto s = DoubleHashSet<string>();
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
            for(int i = 0; i < 10001; i++)
                if(s.getHash(i))
                    cout << s.getElement(i) << " ";
        }
    }
}


void solveC(){
    int n, k;
    cin >> n >> k;
    auto a = QueuedBoundedStack<DoubleHashSet<string>>();
    a.setCapacity(n + 1);
    a.push(DoubleHashSet<string>());
    string str;
    getline(cin, str);
    while(n--){
        getline(cin, str);
        string t;
        int curi = 0;
        auto s = a.top();
        while(curi < str.length() && str[curi] != ' '){
            t += str[curi];
            curi++;
        }
        if(t == "UNDO"){
            if(curi == str.length()){
                if(a.size() <= k){
                    cout << "ERROR: cannot execute UNDO\n";
                }
                else{
                    a.pop();
                }
            }
            else{
                int q;
                cin >> q;
                if(a.size() <= q){
                    cout << "ERROR: cannot execute UNDO " << q << "\n";
                }
                else{
                    while(q--)
                        a.pop();
                }
            }
        }
        else{
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
                a.push(s);
            }
            else if(t == "REMOVE"){
                string name;
                cin >> name;
                s.remove(name);
                a.push(s);
            }
            else if(t == "LIST"){
                for(int i = 0; i < 10001; i++)
                    if(s.getHash(i))
                        cout << s.getElement(i) << " ";
            }
        }
    }
}


signed main(){
    //solveA();
    //solveB();
    //solveC();
}