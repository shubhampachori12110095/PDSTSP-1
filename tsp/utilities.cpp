#ifndef TIGERSUGAR_UTILITIES
#define TIGERSUGAR_UTILITIES

#include "testlib.h"

#ifndef STD
#define STD

#include<bits/stdc++.h>
using namespace std;

#endif

#define FOR(i, a, b) for (int i = (a), _b = (b); i <= _b; i++)
#define FORD(i, b, a) for (int i = (b), _a = (a); i >= _a; i--)
#define REP(i, n) for (int i = 0, _n = (n); i < _n; i++)
#define FORE(i, v) for (__typeof((v).begin()) i = (v).begin(); i != (v).end(); i++)
#define ALL(v) (v).begin(), (v).end()
#define IS_INF(x)   (std::isinf(x))
#define IS_NAN(x)   (std::isnan(x))
#define fi   first
#define se   second
#define MASK(i) (1LL << (i))
#define BIT(x, i) (((x) >> (i)) & 1)
#define div   ___div
#define prev   ___prev
#define left   ___left
#define right   ___right
#define __builtin_popcount __builtin_popcountll
template<class X, class Y>
    bool minimize(X &x, const Y &y) {
        X eps = 1e-9;
        if (x > y + eps) {
            x = y;
            return true;
        } else return false;
    }
template<class X, class Y>
    bool maximize(X &x, const Y &y) {
        X eps = 1e-9;
        if (x + eps < y) {
            x = y;
            return true;
        } else return false;
    }
template<class T>
    T Abs(const T &x) {
        return (x < 0 ? -x : x);
    }

namespace tigersugar {

typedef double Distance;
const Distance INF_DISTANCE = (int)1e9 + 7;

struct Instance {
    int numPoint;
    vector<vector<Distance>> distance;

    Instance(){};

    Instance(int numPoint, vector<vector<Distance>> distance) {
        this->numPoint = numPoint;
        this->distance.assign(numPoint + 1, vector<Distance>(numPoint + 1, 0));
        for(int i = 0; i <= numPoint; ++i)
            for(int j = 0; j <= numPoint; ++j)
                this->distance[i][j] = distance[i][j];
    }

    void print(void) const {
        cerr << "Number of points: " << numPoint << endl;
        cerr << "Distance from depot:";
        FOR(i, 1, numPoint) cerr << " " << distance[0][i]; cerr << endl;
        cerr << "Distance matrix:" << endl;
        FOR(i, 1, numPoint) {
            FOR(j, 1, numPoint) cerr << distance[i][j] << " "; cerr << endl;
        }
        cerr << "***END***" << endl;
    }
};

struct Tour {
    vector<int> points; // this does not include the depot.

    Tour(const vector<int> &points = vector<int>()) {
        this->points = points;
    }

    void add(int x) {
        points.push_back(x);
    }

    int& operator [] (int x) {
        return points[x];
    }
    int operator [] (int x) const {
        return points[x];
    }

    bool empty(void) const {
        return points.empty();
    }

    int size(void) const {
        return points.size();
    }

    Distance distance(const Instance &instance) const {
        if (points.empty()) return 0;
        Distance res = instance.distance[0][points.front()] + instance.distance[points.back()][0];
        REP(i, (int)points.size() - 1) res += instance.distance[points[i]][points[i + 1]];
        return res;
    }

    void reverse(void) {
        std::reverse(ALL(points));
    }

    void print(const string &name) const {
        printf("%s:", name.c_str());
        FORE(it, points) printf(" %d", *it); printf("\n");
    }

    void verify(const Instance &instance) const {
        set<int> used;
        ensuref(!points.empty(), "Tour should not be empty.");
        FORE(it, points) {
            ensuref(1 <= *it && *it <= instance.numPoint,
                    "Invalid point index: %d", *it);
            ensuref(used.insert(*it).se, "Duplicated point index: %d", *it);
        }
    }
};

struct Solution {
    vector<Tour> tours;

    void add(const Tour &tour) {
        tours.push_back(tour);
    }

    Tour& operator [] (int x) {
        return tours[x];
    }
    Tour operator [] (int x) const {
        return tours[x];
    }

    bool empty(void) const {
        return tours.empty();
    }

    int size(void) const {
        return tours.size();
    }

    Distance distance(const Instance &instance) const {
        Distance res = 0;
        FORE(it, tours) res += it->distance(instance);
        return res;
    }

    void print(void) const {
        REP(i, tours.size()) tours[i].print(format("Route #%d", i + 1));
    }

    void verify(const Instance &instance) const {
        set<int> used;
        FORE(it, tours) {
            it->verify(instance);
            FORE(jt, it->points) ensuref(used.insert(*jt).se, "%d is visited in two different tours.", *jt);
        }
        FOR(i, 1, instance.numPoint)
            ensuref(used.find(i) != used.end(), "%d has not been visited.", i);
    }
};

} // tigersugar

#endif // TIGERSUGAR_UTILITIES
