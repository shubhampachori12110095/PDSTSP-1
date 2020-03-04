#include<bits/stdc++.h>
#include "pms_optimizer.cpp"
#include "tsp_optimizer.cpp"
#include "NNTourBuilder.cpp"
#include "Split_Procedure.cpp"
//#include "validator.cpp"
#include "../problem.cpp"

namespace interative_two_step_heuristic {
    const double eps = 1e-9;
    const int limitRandomSeed = 10000;

    void Process(tigersugar::Instance &instance) {
        tigersugar::Tour best_veh;
        tigersugar::Drone_Tour best_drone;

        clock_t startTime = clock();

        double bestCost;
        int limit_case = 1000000;
        for (int ore_case = 1; ore_case <= limit_case; ++ore_case) {
            tigersugar::Tour vehicle = NNTourBuilder::build(instance, 0);
            tigersugar::Drone_Tour drone;

            double curCost = vehicle.distance(instance);

            while (true) {
                int UB1 = (ore_case == 1) ? curCost : min(curCost, bestCost);
                Split_Procedure::split(instance, vehicle, drone, UB1);

                    // vehicle.debug(instance);
                    // drone.debug(instance);

                tsp_optimizer::optimizeTour(instance, vehicle);
                pms_optimizer::optimizeTour(instance, drone);

                double cost_vehicle = vehicle.distance(instance);
                double cost_drone = drone.distance(instance);
                double cost = max( cost_vehicle, cost_drone );

                if ( cost + eps < curCost ) curCost = cost;
                else break;
            }
            if (ore_case == 1 || bestCost > curCost) { 
                bestCost = curCost; best_veh = vehicle; best_drone = drone; 
            }

            clock_t currentTime = clock();
            if ( (double) (currentTime - startTime) / CLOCKS_PER_SEC > 300.0 ) break;

                // cerr << "DONE :)\n";
                // exit(0);
        }

            /// OUTPUT 
           // best_veh.debug(instance);
           // best_drone.debug(instance);
           Problem::result = max( best_veh.distance(instance), best_drone.distance(instance) );
           Problem::vTour = best_veh.points;
           Problem::dTour = best_drone.node;
    }
}