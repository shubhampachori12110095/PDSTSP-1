#ifndef PDSTSP_PROBLEM
#define PDSTSP_PROBLEM

#include <bits/stdc++.h>

#include "point.cpp"
#include "constant.cpp"

using namespace std;

namespace Problem
{
    int n; // Number of customers
    int m; // Number of drones
    int depot = 0;
    vector<double> cD; // cost of traveling by drone from depot to customer and return
    vector<vector<double> > cV; // cost of traveling by vehicle from this customer to another

    int nD; // Number of drone-eligible customers

    void readDataFromTSPLIBInstance(string input_file, int _n, int _m, double sV, double sD)
    {
        // sV: speed of vehicle
        // sD: speed of drone

        cerr << "Reading data from " << input_file << "...\n";

        n = _n;
        m = _m;
        
        ifstream input(Constant::INSTANCE_FOLDER + "/" + input_file);
        vector<Point::point> points;

        for(int i = 0; i <= n; ++i)
        {
            char temp;
            int id, drone_eligible;
            double x, y;
            input >> id >> temp >> x >> temp >> y >> temp >> drone_eligible;
            points.push_back(Point::point(x, y));
            if (drone_eligible)
            {
                cD.push_back(2 * Point::euclidean_distance(points[0], points[i]) / sD);
                nD++;
            }
            else 
                cD.push_back(Constant::INF);
        }

        for(int i = 0; i <= n; ++i)
        {
            vector<double> a = vector<double>();
            cV.push_back(vector<double>());
            for(int j = 0; j <= n; ++j)
                cV[i].push_back(Point::manhattan_distance(points[i], points[j]) / sV);
        }
    }
}

#endif

