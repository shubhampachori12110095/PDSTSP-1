#ifndef PDSTSP_SOLVER
#define PDSTSP_SOLVER

#include "tsp/utilities.cpp"
#include "tsp/tsp_optimizer.cpp"

#include "problem.cpp"
#include "constant.cpp"


namespace Solver
{
    double upper_bound = Constant::INF;

    vector<int> customers1;
    vector<int> customers2;

    tigersugar::Instance instance;

    void reset_upper_bound()
    {
        upper_bound = 0;
        for(auto customer : customers2)
            upper_bound += Problem::dCost[customer] / Problem::m;
    }

    void classify_customers()
    {
        customers1.clear();
        customers2.clear();
        for(int i = 1; i <= Problem::n; ++i)
            (Problem::dCost[i] == Constant::INF ? customers1 : customers2).push_back(i);
    }

    void init()
    {
        instance = tigersugar::Instance(Problem::n, Problem::vCost);
        classify_customers();
        reset_upper_bound();
    }

    vector<int> tsp_optimize(const vector<int>& customers, tsp_optimizer::OptimizeMethod method = tsp_optimizer::threeOptsMethod)
    {
        tigersugar::Tour tour(customers);
        tsp_optimizer::optimizeTour(instance, tour, method);
        return tour.points;
    }

    struct ResultDp {
        double costV;
        double costD;
        int trackI;
        int trackJ;
        int trackP;
        int trackL;

        ResultDp()
        {
            costV = costD = Constant::INF;
            trackI = trackJ = trackP = trackL = 0;
        }

        ResultDp(const double& costV, const double& costD, const int& trackI, const int& trackJ, const int& trackP, const int& trackL)
        {
            this->costV = costV;
            this->costD = costD;
            this->trackI = trackI;
            this->trackJ = trackJ;
            this->trackP = trackP;
            this->trackL = trackL;
        }

        double minCost()
        {
            return min(costV, costD);
        }

        double maxCost()
        {
            return max(costV, costD);
        }

        bool operator < (ResultDp another)
        {
            if (maxCost() < another.maxCost()) return true;
            if (maxCost() == another.maxCost())
                return minCost() < another.minCost();
            return false;
        }

        bool better(ResultDp another)
        {
            if (costV < another.costV) return true;
            if (costV == another.costV)
                return costD < another.costD;
            return false;
        }

    };

    ResultDp f[501][501][101][2];
    vector<int> split_customers1;
    vector<int> split_customers2;

    void track_back(const ResultDp& result, const int& i, const int& j, const int &l)
    {
        ResultDp previous = f[result.trackI][result.trackJ][result.trackP][result.trackL];
        // cerr << i << " " << j << " " << l << "\n";
        if (i || j)
            track_back(previous, result.trackI, result.trackJ, result.trackL);
        if (l == 0)
        {
            if (i) split_customers1.push_back(customers1[i-1]);
        }
        else
        {
            if (j) split_customers1.push_back(customers2[j-1]);
            for(int k = result.trackJ + 1; k < j; ++k)
                split_customers2.push_back(customers2[k-1]);
        }
    }

    void split(vector<int>& ctm1 = customers1, vector<int>& ctm2 = customers2)
    // Customers in ctm1 are not drone-eligible
    {
        int n1 = ctm1.size();
        int n2 = ctm2.size();
        if (n1 > 500 || n2 > 500)
        {
            cerr << "Size of the instance is too large!";
            exit(0);
        }

        ctm1 = tsp_optimize(ctm1);
        ctm2 = tsp_optimize(ctm2);

//        cerr << "\nVehicle group: " << n1 << " " << ctm1.size() << "\n";
//        for(auto x : ctm1) cerr << x << " ";
//        cerr << "\nDrone group: " << n2 << " " << ctm2.size() << "\n";
//        for(auto x : ctm2) cerr << x << " ";
//
//        cerr << "\n";


        f[0][0][0][0] = ResultDp(0, 0, 0, 0, 0, 0);
        f[0][0][0][1] = ResultDp(0, 0, 0, 0, 0, 0);

        for(int i = 0; i <= n1; ++i)
            for(int j = 0; j <= n2; ++j)
                for(int p = 0; p <= 100; ++p)
                    for(int e = 0; e < 2; ++e)
                        if (f[i][j][p][e].costD != Constant::INF)
                        {
                            int last = e ? (j ? ctm2[j-1] : 0) : (i ? ctm1[i-1] : 0);
                            if (i < n1)
                            {
                                ResultDp temp(
                                        f[i][j][p][e].costV + Problem::vCost[last][ctm1[i]],
                                        f[i][j][p][e].costD,
                                        i, j, p, e
                                );
                                if (temp.better(f[i+1][j][p][0]))
                                {
                                    f[i+1][j][p][0] = temp;
                                    //cerr << i+1 << " " << j << " " << p << " " << 0 << " " << temp.costV << " " << temp.costD << "\n";
                                }
                            }

                            double sumD = 0;
                            for (int k = j + 1; k <= n2; ++k)
                            {
                                int pp = min((f[i][j][p][e].costD + sumD) * 100 / upper_bound, 100.0);
                                ResultDp temp(
                                        f[i][j][p][e].costV + Problem::vCost[last][ctm2[k-1]],
                                        f[i][j][p][e].costD + sumD,
                                        i, j, p, e
                                );
                                if (temp.better(f[i][k][pp][1]))
                                {
                                    f[i][k][pp][1] = temp;
                                    //cerr << i << " " << k << " " << pp << " " << 1 << " " << temp.costV << " " << temp.costD << "\n";
                                }
                                sumD += Problem::dCost[ctm2[k-1]] / Problem::m;
                            }
                        }

        ResultDp result;
        double sumD = 0;
        for(int j = n2; j >= 0; --j)
        {
            for (int p = 0; p <= 100; ++p)
                for (int e = 0; e <= 1; e++)
                    if (f[n1][j][p][e].costD != Constant::INF)
                    {
                        ResultDp temp(
                                f[n1][j][p][e].costV,
                                f[n1][j][p][e].costD + sumD,
                                n1, j, p, e
                        );
                        if (temp < result) result = temp;
                    }
            if (j) sumD += Problem::dCost[ctm2[j-1]] / Problem::m;
        }

        split_customers1.clear();
        split_customers2.clear();

        track_back(f[result.trackI][result.trackJ][result.trackP][result.trackL], result.trackI, result.trackJ, result.trackL);
        for(int k = result.trackJ + 1; k <= n2; ++k)
            split_customers2.push_back(ctm2[k-1]);
    }

    vector<int> vTour;
    vector<vector<int>> dTour;

    bool drone_compare(const int& i, const int& j)
    {
        return Problem::dCost[i] > Problem::dCost[j];
    }

    vector<vector<int>> manage_drones(vector<int>& ctm = split_customers2)
    {
        vector<vector<int>> result(Problem::m);
        priority_queue<pair<int, int>> drones;
        for(int i = 0; i < Problem::m; ++i)
            drones.push({0, i});

        sort(ctm.begin(), ctm.end(), drone_compare);

        for(auto customer: ctm)
        {
            pair<int, int> temp = drones.top();
            drones.pop();
            result[temp.second].push_back(customer);
            temp.first -= Problem::dCost[customer];
            drones.push(temp);
        }
        return result;
    }

    void optimize_vehicle_and_drones_tour()
    {
//        cerr << "\n->" << split_customers1.size() << " ";
//        for(auto t : split_customers1) cerr << t << " ";
//        cerr << "\n============================\n";
//        cerr << "\n->" << split_customers2.size() << " ";
//        for(auto t : split_customers2) cerr << t << " ";

        vTour = tsp_optimize(split_customers1);
        dTour = manage_drones(split_customers2);
//        cerr << "\n=========================\n";
//        cerr << dTour.size() << "\n";
//        for(auto drone : dTour)
//        {
//            cerr << drone.size() << " ";
//            for(auto x : drone) cerr << x << " ";
//            cerr << "\n";
//        }
    }

    int unchanged_time = 0;

    void update_result()
    {
        double vResult = 0;
        double dResult = 0;

        if (!vTour.empty())
        {
            vResult = Problem::vCost[0][vTour.front()];
            for(int i = 1; i < vTour.size(); ++i)
                vResult += Problem::vCost[vTour[i-1]][vTour[i]];
            vResult += Problem::vCost[vTour.back()][0];
        }

        for(const vector<int>& drone: dTour)
        {
            double sum = 0;
            for(int i : drone)
                sum += Problem::dCost[i];
            dResult = max(dResult, sum);
        }

        double result = max(vResult, dResult);

        cerr << "Result: " << result << "\n";
        cerr << "Vehicle Result: " << vResult << "\n";
        cerr << "Drone Result: " << dResult << "\n";

        unchanged_time++;

        if (result < Problem::result)
        {
            unchanged_time = 0;
            upper_bound = result;
            Problem::result = result;
            Problem::vResult = vResult;
            Problem::dResult = dResult;
            Problem::vTour = vTour;
            Problem::dTour = dTour;
        }

    }

}


#endif
