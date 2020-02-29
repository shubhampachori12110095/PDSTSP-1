#include "utilities.cpp"

namespace pms_optimizer {
    void optimizeTour(const tigersugar::Instance &instance, tigersugar::Drone_Tour &tour) {
        multiset<pair<double, int> > Set;
        REP(i, instance.numDrone) {
            if ( i < (int) tour.node.size() ) tour[i].clear();
            Set.insert(make_pair(0, i)); 
        }

        REP(i, instance.numDroneEligible) {
            auto it = Set.begin();
            int id = it -> second;
            double nTime = it->first + instance.lsDroneEligible[i].second;
            Set.erase(it);
            Set.insert( make_pair(nTime, id) );

            while ( id >= (int) tour.size() ) tour.add_drone();
            tour[id].push_back( instance.lsDroneEligible[i].first );
        }   
    }
}