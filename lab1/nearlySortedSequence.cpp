#include <iostream>
#include <vector>

using namespace std;

typedef long long ll;

// O(N^2), W(N)
void insertionSort(vector<ll> &v){
    for(ll i = 1; i < v.size(); i++){
        ll j = i - 1;
        while(j >= 0 && v[j] > v[j + 1]){
            swap(v[j], v[j + 1]);
            j--;
        }
    }
}

// O(N^2), W(N^2)
void selectionSort(vector<ll> &v){
    for(ll i = 0; i < v.size(); i++){
        ll resIndex = i;
        for(ll j = i + 1; j < v.size(); j++){
            if(v[j] < v[resIndex]){
                resIndex = j;
            }
        }
        swap(v[i], v[resIndex]);
    }
}

signed main() {
    ll n;
    cin >> n;
    vector<ll> a(n);
    for(auto &i: a) cin >> i;
    insertionSort(a);
    for(auto &i: a) cout << i << " ";
}
