#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <cstdint>
#include <cassert>
#include <numeric>
#include <string>
#include <complex>
#include <algorithm>
#include <iostream>
#include <vector>
#include <bitset>
#include <deque>
#include <queue>
#include <map>
#include <utility>
#include <functional>
#include <iomanip>
#include <chrono>
#include <unordered_map>
#include <set>
using namespace std;
using ull = uint64_t;
using ll = int_fast64_t;
using prll = pair<ll, ll>;
constexpr ll MOD = 1000000007; //10億　= 10^9になってる
constexpr ll mINF = -(1LL << 60);
constexpr ll pINF = 1LL << 60;
constexpr ull uINF = 1844674407399900000; //ullのmax-1桁してる
constexpr long double pi = 3.1415926535897932384;
constexpr ll juu = 100000;    //10万 10e5
constexpr ll hyaku = 1000000; //100万　10e6
constexpr int dx[4] = {0, 0, 1, -1};
constexpr int dy[4] = {1, -1, 0, 0};
constexpr int ddx[8] = {0, 0, 1, -1, 1, 1, -1, -1};
constexpr int ddy[8] = {1, -1, 0, 0, -1, 1, 1, -1};
#define all(v) v.begin(), v.end()
#define rep(i, n) for (ll i = 0; i < (ll)(n); ++i)
#define repN(i, first, end) for (ll i = first; i < (ll)(end); ++i)
#define Debug(xx) cerr << " DEBUG:" << xx << endl;
#define Debug2(xx, yy) cerr << " DEBUG:" << xx << ":" << yy << endl;

/*ソートして被りを無くす*/
template <typename T>
void eraseSame(T &a)
{
    sort(all(a));
    a.erase(unique(all(a)), a.end());
}

/*一行に入力が何個あるかを1,ぶち込むvectorを2*/
template <typename T>
void splitIn(ll N, vector<T> &array)
{
    T temp;
    int i = 0;
    if (N == 0)
    {
        return;
    }
    array.resize(N);
    while (cin >> temp)
    {
        array[i] = temp;
        ++i;
        if (i >= N)
        {
            break;
        }
    }
    return;
}

//繰り返し二乗。掛けられる数、回数、mod
ll powpow(ll n, ll p)
{
    if (p == 0)
    {
        return 1;
    }
    if (p == 1)
        return n % MOD;
    if (p % 2 == 1)
    {
        return (n * powpow(n, p - 1)) % MOD;
    }
    ll dob = powpow(n, p / 2);
    return (dob * dob) % MOD;
}

ll inv_num(ll num)
{
    return powpow(num, MOD - 2);
}

int main(int argc, char const *argv[])
{

    return 0;
}