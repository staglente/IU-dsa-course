#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

using ll = long long;

bool comp(pair <string, ll> &p1, pair<string, ll> &p2) {
    if (p1.second == p2.second)
        return p1.first < p2.first;
    return p1.second > p2.second;
}

void solveA() {
    ll n;
    cin >> n;
    map<string, ll> cnt;
    while (n--) {
        string s;
        cin >> s;
        cnt[s]++;
    }
    vector<pair<string, ll>> a;
    for (auto &i: cnt) a.push_back(i);
    sort(a.begin(), a.end(), comp);
    for (auto &i: a) cout << i.first << " " << i.second << "\n";
}

void solveB() {
    ll n, m;
    cin >> n;
    set<string> st1, st2;
    vector<string> a;
    for (ll i = 0; i < n; i++) {
        string s;
        cin >> s;
        st1.insert(s);
    }
    cin >> m;
    for (ll i = 0; i < m; i++) {
        string s;
        cin >> s;
        if (st1.find(s) == st1.end() && st2.find(s) == st2.end()) a.push_back(s);
        st2.insert(s);
    }
    cout << a.size() << "\n";
    for (auto &i: a) cout << i << " ";
}

signed main() {
    solveB();
}
