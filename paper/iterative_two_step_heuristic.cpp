#include<bits/stdc++.h>
#include "pms_optimizer.cpp"
#include "tsp_optimizer.cpp"
#include "NNTourBuilder.cpp"
#include "Split_Procedure.cpp"
//#include "validator.cpp"
#include "../problem.cpp"

namespace interative_two_step_heuristic {
    const double eps = 1e-9;

    void Process(tigersugar::Instance &instance) {
            //instance.Initialize();
        tigersugar::Tour best_veh;
        tigersugar::Drone_Tour best_drone;

        double bestSol;
        int limit_case = 3;
        for (int ore_case = 1; ore_case <= limit_case; ++ore_case) {
            tigersugar::Tour vehicle = NNTourBuilder::build(instance, 0);
            tigersugar::Drone_Tour drone;

                // FOR(i, 0, instance.numPoint) vehicle.add(i);
                // vehicle.add(0);
                // vehicle.debug(instance);

            double bestCost = vehicle.distance(instance);
            while (true) {
                Split_Procedure::split(instance, vehicle, drone, bestCost);

                tsp_optimizer::optimizeTour(instance, vehicle);
                pms_optimizer::optimizeTour(instance, drone);

                double cost_vehicle = vehicle.distance(instance);
                double cost_drone = drone.distance(instance);
                double cost = max( cost_vehicle, cost_drone );

                if ( cost + eps < bestCost ) bestCost = cost;
                else break;
            }
            if (ore_case == 1 || bestSol < bestCost) { bestSol = bestCost; best_veh = vehicle; best_drone = drone; }
        }


            /// OUTPUT 
           // best_veh.debug(instance);
           // best_drone.debug(instance);
           Problem::result = max( best_veh.distance(instance), best_drone.distance(instance) );
           Problem::vTour = best_veh.points;
           Problem::dTour = best_drone.node;
    }
}