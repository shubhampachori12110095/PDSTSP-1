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
    vector<vector<double> > vCost; // cost of traveling by the vehicle from this customer to another
    vector<double> dCost; // cost of traveling by a drone from depot to customer and return

    int nD; // Number of drone-eligible customers

    double result = Constant::INF;
    double vResult = Constant::INF;
    double dResult = Constant::INF;
    vector<int> vTour; // tour of the vehicle, the first and the last must be depot
    vector<vector<int>> dTour; // list of customers assigned of drones, not containing the depot


    void import_data_from_tsplib_instance(const string& input_file, int _n, int _m, double vSpeed, double dSpeed)
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
            drone_eligible = 1 - drone_eligible;
            points.push_back(Point::point(x, y));
            if (drone_eligible)
            {
                dCost.push_back(2 * Point::euclidean_distance(points[0], points[i]) / dSpeed);
                nD++;
            }
            else
                dCost.push_back(Constant::INF);
        }

        for(int i = 0; i <= n; ++i)
        {
            vector<double> a = vector<double>();
            vCost.push_back(vector<double>());
            for(int j = 0; j <= n; ++j)
                vCost[i].push_back(Point::manhattan_distance(points[i], points[j]) / vSpeed);
        }
    }

    void validate_result()
    {
        double sV = 0;
        double sD = 0;
        set<int> s;

        if (!vTour.empty())
        {
            sV += vCost[0][vTour.front()];
            for(auto id: vTour)
            {
                Template::check(0 < id && id <= n, "Wrong customer");
                s.insert(id);
            }
            for(int i = 1; i < vTour.size(); ++i)
                sV += vCost[vTour[i-1]][vTour[i]];
            sV += vCost[vTour.back()][0];
        }
        Template::check(sV == vResult, "Wrong vehicle result");

        for(const vector<int>& drone: dTour)
        {
            double sum = 0;
            for(int id : drone)
            {
                sum += Problem::dCost[id];
                s.insert(id);
            }
            sD = max(sD, sum);
        }
        Template::check(sD == dResult, "Wrong drones result");

        Template::check(s.size() == n, "Missing customers");

    }

    void print_result_to_file(const string& output_file)
    {
        ofstream output(("result/" + output_file).c_str());

        output << "Result: " << result << "\n";
        output << "Vehicle result: " << vResult << "\n";

        output << "Vehicle tour: " << vTour.size() << " customers\n";
        output << 0;
        for(auto id : vTour) output << id << " ";
        output << "0\n";

        output << "Drone result: " << dResult << "\n";
        for(int i = 0; i < m; ++i)
        {
            output << "Customers of drone #" << i+1 << ": " << dTour[i].size() << " customers\n";
            for(auto id : dTour[i]) output << id << " ";
            output << "\n";
        }
    }

}

#endif

