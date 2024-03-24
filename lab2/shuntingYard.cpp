#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <map>

using namespace std;

typedef long long ll;

string toPost(string &s) {
    map<char, ll> m;
    m['('] = 0;
    m['['] = 0;
    m['{'] = 0;
    m['+'] = 1;
    m['-'] = 1;
    m['*'] = 2;
    m['/'] = 2;
    m[')'] = 3;
    m[']'] = 3;
    m['}'] = 3;
    string res, cur;
    stack<char> c;
    for (ll i = 0; i <= s.length(); i++) {
        if (s[i] == ' ' || i == s.length()) {
            if (cur.length() > 1 || (cur >= "0" && cur <= "9")) {
                res += cur;
                res += ' ';
            } else if (m[cur.front()] == 0) {
                c.push(cur.front());
            } else if (m[cur.front()] < 3) {
                while (!c.empty() && m[cur.front()] <= m[c.top()]) {
                    res += c.top();
                    res += ' ';
                    c.pop();
                }
                c.push(cur.front());
            } else {
                while (!c.empty() && m[c.top()] > 0) {
                    res += c.top();
                    res += ' ';
                    c.pop();
                }
                c.pop();
            }
            cur.clear();
        } else {
            cur += s[i];
        }
    }
    if (!cur.empty()) {
        res += cur;
        res += ' ';
    }
    while (!c.empty()) {
        res += c.top();
        res += ' ';
        c.pop();
    }
    return res;
}

ll calculateIn(string &str) {
    string s = toPost(str), cur;
    stack<ll> c;
    for (ll i = 0; i <= s.length(); i++) {
        if (s[i] == ' ' || i == s.length()) {
            if (cur.length() > 1 || (cur >= "0" && cur <= "9")) {
                c.push(stoll(cur));
            } else {
                ll a, b, res;
                b = c.top();
                c.pop();
                a = c.top();
                c.pop();
                switch (cur.front()) {
                    case '+':
                        res = a + b;
                        break;
                    case '-':
                        res = a - b;
                        break;
                    case '*':
                        res = a * b;
                        break;
                    default:
                        res = a / b;
                        break;
                }
                c.push(res);
            }
            cur.clear();
        } else {
            cur += s[i];
        }
    }
    return c.top();
}

signed main() {
    string str;
    getline(cin, str);
    cout << toPost(str);
}
