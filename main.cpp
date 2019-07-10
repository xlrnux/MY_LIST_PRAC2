#include<iostream>
#include<list>

using namespace std;

typedef __SIZE_TYPE__ size_t2;
typedef unsigned int ADDR;

template<typename T>
struct Node{
    T val;
    ADDR next;
    ADDR prev;
};

template<typename T, size_t2 _capacity>
class List{
public:
//private: // 테스트용 소스를 위해 public: 으로 열어둠
    size_t2 _size;
    Node<T> node[_capacity+2];
    ADDR HEAD;
    ADDR TAIL;
    ADDR pos;

    ADDR link(ADDR prev, ADDR next, T val){
        node[pos].val = val;

        node[pos].prev = prev;
        node[pos].next = next;

        node[prev].next = pos;
        node[next].prev = pos;

        _size++;
        return pos++;
    }

    ADDR unlink(ADDR target){
        ADDR prev = node[target].prev;
        ADDR next = node[target].next;

        node[prev].next = next;
        node[next].prev = prev;

        _size--;
        return next;
    }

public:
    List() : _size(0), HEAD(_capacity), TAIL(_capacity+1), pos(0) {
        node[HEAD].next = TAIL;
        node[TAIL].prev = HEAD;
    }

    void clear(){
        _size = 0;
        HEAD = _capacity;
        TAIL = _capacity+1;
        pos = 0;
        node[HEAD].next = TAIL;
        node[TAIL].prev = HEAD;
    }

    ADDR begin() const {
        return node[HEAD].next;
    }
    ADDR end() const {
        return TAIL;
    }

    ADDR next(ADDR cur) {
        return node[cur].next;
    }

    T get(ADDR cur) const {
        return node[cur].val;
    }

    T front() const {
        return node[begin()].val;
    }

    T back() const {
        return node[node[TAIL].prev].val;
    }

    ADDR push_back(T val) {
        ADDR prev = node[TAIL].prev;
        ADDR next = TAIL;

        link(prev, next, val);
    }

    ADDR push_front(T val){
        ADDR prev = HEAD;
        ADDR next = node[HEAD].next;

        link(prev, next, val);
    }

    ADDR pop_front() {
        return unlink(begin());
    }

    ADDR pop_back() {
        return unlink( node[end()].prev );
    }

    bool empty() const {
        return _size == 0;
    }

    size_t2 size() const{
        return _size;
    }

    ADDR insert(ADDR next, T val){
        ADDR prev = node[next].prev;
        return link(prev, next, val);
    }

    ADDR erase(ADDR target){
        return unlink(target);
    }

    void defrag(){
        if(_size == pos) return;

        T Temp[_size+1];
        int idx = 0;
        size_t2 __size = _size;
        for( ADDR it = begin(); it != end(); it = next(it)) Temp[idx++] = get(it);
        clear();

        for(int i = 0; i < idx; i++) push_back(Temp[i]);
    }

    void merge(T* arr, int left, int mid, int right){
        int l = left, r = mid+1, n = 0;
        T temp[right-left+1];
        while(l <= mid && r <= right){
            if(arr[l] < arr[r]) temp[n++] = arr[l++];
            else temp[n++] = arr[r++];
        }

        if(l > mid) {
            while (r <= right) temp[n++] = arr[r++];
        }else {
            while (l <= mid) temp[n++] = arr[l++];
        }

        for(int i = left; i<= right; i++) arr[i] = temp[i-left];
    }

    void sort(T* arr, int left, int right){
        if(left >= right) return;

        int mid = (left+right)>>1;

        sort(arr, left, mid);
        sort(arr, mid+1, right);
        merge(arr,left,mid,right);
    }

    void sort(){
        T Temp[_size];
        int idx = 0;
        int __size = _size;
        for( ADDR it = begin(); it != end(); it = next(it)) Temp[idx++] = get(it);

        sort(Temp, 0, _size-1);

        clear();
        for(int i = 0; i < idx; i++) push_back(Temp[i]);
    }

};


int main() {
    // STL List 동작 확인
    list<int> l;
    for(int i = 0; i < 10; i++){
        l.push_back(i);
    }
    auto _it = l.begin();
    for(_it = l.begin(); _it != l.end(); _it++){
        if(*_it == 2) break;
    }cout << "\n";
    cout << *_it << "\n";

    for(auto it = l.begin(); it != l.end(); it++){
        cout << *it << " ";
    }cout << "\n";

    _it = l.insert(_it, 11);
    for(auto it = l.begin(); it != l.end(); it++){
        cout << *it << " ";
    }cout << "\n";

    cout << *_it << "\n";
    _it = l.erase(_it);
    for(auto it = l.begin(); it != l.end(); it++){
        cout << *it << " ";
    }cout << "\n";
    cout << *_it << "\n";


    // 자작 List 동작 확인
    List<int, 100> L; // 100 은 내부 node배열의 사이즈
    L.clear();
    for(int i = 0; i < 10; i++) L.push_back(i);

    auto it = L.begin();
    for(it = L.begin(); it != L.end(); it = L.next(it))
        if(L.get(it) == 2) break;

    cout <<"\n";
    cout << L.get(it) << "\n";

    for(auto it = L.begin(); it != L.end(); it = L.next(it))
        cout << L.get(it) << " ";

    cout << "\n";

    it = L.insert(it, 11);
    for(auto it = L.begin(); it != L.end(); it = L.next(it))
        cout << L.get(it) << " ";

    cout << "\n";

    cout << L.get(it) << "\n";
    it = L.erase(it);
    for(auto it = L.begin(); it != L.end(); it = L.next(it))
        cout << L.get(it) << " ";

    cout << "\n";
    cout << L.get(it) << "\n";

    cout << "\n";
    L.pop_back(); L.pop_front();
    L.push_back(100); L.push_front(15);

    for(int i = 0; i < L.pos; i++)
        cout << L.get(i) << " ";

    cout << "\n";

    L.defrag();

    for(auto it = L.begin(); it != L.end(); it = L.next(it))
        cout << L.get(it) << " ";

    cout << "\n";
    for(int i = 0; i < L.pos; i++)
        cout << L.get(i) << " ";
    cout << "\n";

    L.sort();
    for(auto it = L.begin(); it != L.end(); it = L.next(it))
        cout << L.get(it) << " ";

    cout << "\n";
    for(int i = 0; i < L.pos; i++)
        cout << L.get(i) << " ";

    cout << "\n";

    return 0;
}