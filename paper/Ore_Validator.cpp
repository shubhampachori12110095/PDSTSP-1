#include "utilities.cpp"

namespace ORE_Validator {
    const long double EPS = 1e-9;

    void valid(const tigersugar::Instance &instance, const tigersugar::Tour &vehicle, tigersugar::Drone_Tour &drone) {
        if ( (int) vehicle.size() > 1 ) {
            if ( vehicle[0] != 0 || vehicle.points.back() != 0 ) {
                cout  << "ERROR: vehicle tour should have 0 in start and finish\n";
                exit(0); 
            }
        }

        /// Check duplicated
        vector<bool> seen;
        seen.resize(instance.numPoint+3, false);

        for (int x : vehicle.points) {
            if (x != 0 && seen[x]) {
                cout << "ERROR: " << x << " is duplicated \n";
                exit(0);
            }
            seen[x] = true;
        }
        for (vector<int> ls : drone.node) {
            for (int x : ls) {
                if (x == 0) {
                    cout << "ERROR: " << x << " shoudn't appear in drone tour\n";
                    exit(0);
                }
                if (seen[x]) {
                    cout << "ERROR: " << x << " is duplicated \n";
                    exit(0);
                }
                seen[x] = true;
            } 
        }

        /// Check Missing
        FOR(i, 1, instance.numPoint) if (!seen[i]) {
            cerr << "ERROR: Missing " << i << '\n';
            exit(0);
        }

        /// Check cost
        double vehicle_cost = 0;
        FOR(i, 0, vehicle.size()-2) vehicle_cost += instance.distance[ vehicle[i] ][ vehicle[i+1] ];
        if (vehicle_cost != vehicle.distance(instance)) {
            cerr << "ERROR: vehicle cost doesn't match for vehicle tour \n";
            exit(0);
        } 

        double drone_cost = 0;
        for (vector<int> ls : drone.node) {
            double cost = 0;
            for (int x : ls) cost += instance.drone_cost[x];
            drone_cost = max(drone_cost, cost);
        }
        
        if (drone_cost != drone.distance(instance)) {
            cerr << "ERROR: drone cost doesn't match for drone tour\n";
            exit(0);
        }

        cerr << "VALIDATOR: PASS\n\n";
    }
}