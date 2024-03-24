#include <iostream>
#include <vector>
#include <set>
using namespace std;

typedef long long ll;

pair<ll, set<ll>> calc(vector<ll> &w_, vector<ll> &c_, ll curw, ll curc, ll &W_, ll index, ll N, set<ll> path_){
   if(index == N)
       return {curc, path_};
   if(curw + w_[index] <= W_){
       set<ll> new_path_ = path_;
       new_path_.insert(index);
       return max(calc(w_, c_, curw + w_[index], curc + c_[index], W_, index + 1, N, new_path_),
                  calc(w_, c_, curw, curc, W_, index + 1, N, path_));
   }
    return calc(w_, c_, curw, curc, W_, index + 1, N, path_);
}



void solveA(){
    ll n, W;
    cin >> n >> W;
    vector<ll> w(n), c(n);
    for(auto &i: w) cin >> i;
    for(auto &i: c) cin >> i;
    set<ll> res = calc(w, c, 0, 0, W, 0, n, {}).second;
    cout << res.size() << "\n";
    for(auto &i: res) cout << (i + 1) << " ";
}

void findPath(vector<ll> &w_, vector<vector<ll>> &dp_, set<ll> &res, ll curi, ll curw){
    if(dp_[curi][curw] == 0)
        return;
    if(dp_[curi-1][curw] == dp_[curi][curw])
        findPath(w_, dp_, res, curi - 1, curw);
    else{
        res.insert(curi);
        findPath(w_, dp_, res, curi - 1, curw - w_[curi]);
    }
}

void solveB(){
    ll n, W;
    cin >> n >> W;
    vector<ll> w(n + 1), c(n + 1);
    vector<vector<ll>> dp(n + 1, vector<ll>(W + 1, 0));
    set<ll> ans;
    for(ll i = 1; i <= n; i++) cin >> w[i];
    for(ll i = 1; i <= n; i++) cin >> c[i];
    for(ll i = 1; i <= n; i++){
        for(ll j = 0; j <= W; j++){
            if(w[i] <= j){
                dp[i][j] = max(dp[i-1][j], dp[i-1][j - w[i]] + c[i]);
            }
            else{
                dp[i][j] = dp[i-1][j];
            }
        }
    }
    findPath(w, dp, ans, n, W);
    cout << ans.size() << "\n";
    for(auto &i: ans) cout << i << " ";
}


signed main(){
    //solveA();
    solveB();
}
