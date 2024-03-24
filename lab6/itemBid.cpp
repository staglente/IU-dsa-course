#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

#define all(v) v.begin(), v.end()

struct Bid{
    int index, minBid, maxBid;
};

bool comp(Bid &b1, Bid &b2){
    if(b1.minBid == b2.minBid && b1.maxBid == b2.maxBid)
        return b1.index < b2.index;
    if(b1.minBid == b2.minBid)
        return b1.maxBid < b2.maxBid;
    return b1.minBid > b2.minBid;
}

void solve(){
    int n;
    cin >> n;
    vector<Bid> a(n);
    for(int i = 0; i < n; i++){
        int mn, mx;
        cin >> mn >> mx;
        a[i] = {i + 1, mn, mx};
    }
    sort(all(a), comp);
    for(auto &i: a)
        cout << i.index << " ";
}

signed main(){
    solve();
}
