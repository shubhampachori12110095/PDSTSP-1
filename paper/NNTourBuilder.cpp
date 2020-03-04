#include "utilities.cpp"

/*
 * Build a giant tour with nearest-neighbor method.
 */
namespace NNTourBuilder {
    #define ORE_INFINITY        1000000007
    // const  INF_DISTANCE = (double) 1e9 + 7;

    tigersugar::Tour build(tigersugar::Instance &instance, int srcNode) {
        tigersugar::Tour tour;
        vector<bool> added(instance.numPoint+1, false);

        tour.add(srcNode);
        added[srcNode] = true;

        int x = srcNode;
        int depotPos = 0;
        for (int i = 1; i <= instance.numPoint; ++i) {
            double bestDist0 = ORE_INFINITY, bestDist1 = ORE_INFINITY, bestDist2 = ORE_INFINITY;
            int bestY0 = -1, bestY1 = -1, bestY2 = -1;

            for (int y = 0; y <= instance.numPoint; ++y) {
                if (added[y]) continue;
                if (instance.distance[x][y] < bestDist0) {
                    bestDist2 = bestDist1; bestY2 = bestY1; 
                    bestDist1 = bestDist0; bestY1 = bestY0;

                    bestDist0 = instance.distance[x][y];
                    bestY0 = y;
                }
                else if (instance.distance[x][y] < bestDist1) {
                    bestDist2 = bestDist1; bestY2 = bestY1;

                    bestDist1 = instance.distance[x][y];
                    bestY1 = y;
                }
                else if (instance.distance[x][y] < bestDist2) {
                    bestDist2 = instance.distance[x][y];
                    bestY2 = y;
                }
            }

                // cerr << "# " << rnd.next(100) << '\n';

            int bestY = -1;
            if (bestY1 < 0) bestY = bestY0;
            else if (bestY2 < 0) bestY = ( (int) rnd.next(1000000000) % 2 == 0 ) ? bestY0 : bestY1;
            else {
                int tmp = (int) rnd.next(1000000000) % 3;
                if (tmp == 0) bestY = bestY0;
                else if (tmp == 1) bestY = bestY1;
                else bestY = bestY2;
            }

                // cerr << "??? " << bestT1 

            assert(bestY >= 0);
            tour.add(bestY);
            x = bestY;
            added[x] = true;

            if (x == 0) depotPos = i;
        }

        // Shift depot to first position
        assert(depotPos >= 0);
        rotate(tour.points.begin(), tour.points.begin() + depotPos, tour.points.end());
        assert(tour.points.front() == 0);
        tour.add(0);

            // cerr << "LAG: " << tour.size() << '\n';
            // exit(0);
        // return
        return tour;
    }
}
