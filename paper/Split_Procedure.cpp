#include "utilities.cpp"

namespace Split_Procedure {
    const double eps = 1e-9;
    #define ORE_INFINITY            1000000007

    int numDrone;
    vector<int> V;
    vector<double> S2;
    vector<vector<int> > adj;

    void Build_Graph(const tigersugar::Instance &instance) {
        int n = V.size();
        adj.clear(); adj.resize(n);
        S2.clear(); S2.resize(n);
        
        REP(i, V.size()) {
            bool flag_eligible = true;
            S2[i] = (i == 0) ? instance.drone_cost[V[i]] : S2[i-1] + instance.drone_cost[V[i]];

            FOR(j, i+1, V.size()-1) {
                if ( !flag_eligible ) break;
                if ( !instance.drone_cost[V[j]] ) flag_eligible = false;
                adj[j].push_back(i);
            }
        }
    }

    double get_S2(int u, int v) {
        ++u; --v;
        if (u > v) return 0;
        if (u == 0) return S2[v];
        return (S2[v] - S2[u-1]) / (double) numDrone; 
    }

    struct dataLabel {
        double c1, c2;
        int preNode, pos;
        dataLabel() {}; dataLabel(double c1, double c2, int preNode, int pos) : c1(c1), c2(c2), preNode(preNode), pos(pos) {};
    };

    double UB;
    vector<vector<dataLabel> > L;
    vector<double> LB_veh, LB_tot;
    vector<pair<double, double> > UB2;

    void process(const tigersugar::Instance &instance, double UB1) {
        /// get lower_bound
        LB_tot.clear(); LB_veh.clear();
        LB_tot.resize(V.size(), ORE_INFINITY);
        LB_veh.resize(V.size(), ORE_INFINITY);

        LB_veh[(int)V.size()-1] = LB_tot[(int)V.size()-1] = 0;
        FORD(i, V.size()-1, 0) {
            for (int j : adj[i]) {
                double c1 = instance.distance[ V[j] ][ V[i] ];
                double c2 = get_S2(j, i);
                minimize( LB_veh[j], LB_veh[i] + c1 );
                minimize( LB_tot[j], LB_tot[i] + c1+c2 ); 
            }
        }

        /// get upper_bound
        pair<double, double> UB3 = make_pair(0, 0);
        UB2.clear();
        UB2.resize( V.size(), make_pair(ORE_INFINITY,ORE_INFINITY) );
        
        UB2[0] = make_pair(0, 0);
        int pre_veh = 0; 
        FOR(i, 1, V.size()-1) {
            /// caculate UB2
            for (int j : adj[i]) {
                double c1 = instance.distance[ V[j] ][ V[i] ] + UB2[j].first;
                double c2 = get_S2(j, i) + UB2[j].second;
                if ( max(UB2[i].first, UB2[i].second) > max(c1, c2) + eps ) UB2[i] = make_pair(c1, c2);
            }

            /// caculate UB3
            if ( instance.drone_cost[V[i]] == 0 ) {
                UB3.first += instance.distance[ pre_veh ][ V[i] ];
                pre_veh = V[i];
            }
            else {
                double c1 = UB3.first;
                double c2 = UB3.second + instance.drone_cost[ V[i] ];
                if ( max(c1, c2) == max(UB3.first, UB3.second) ) { UB3 = make_pair(c1, c2); pre_veh = V[i]; }
                else {
                    UB3.first += instance.distance[ pre_veh ][ V[i] ];
                    pre_veh = V[i];
                }
            }
        }
        UB = min({ UB1, max(UB2[(int)V.size()-1].first, UB2[(int)V.size()-1].second), max(UB3.first, UB3.second) });

        /// main split 
        L.resize( V.size() );
        REP(i, L.size()) L[i].clear();
        L[0].push_back( dataLabel(0, 0, -1, -1) );

        FOR(i, 1, V.size()-1) {
            vector<dataLabel> tmp;

            for (int j : adj[i]) {
                int cur = -1;
                for (dataLabel lab : L[j]) {
                    ++cur;
                    double c1 = instance.distance[ V[j] ][ V[i] ] + lab.c1;
                    double c2 = get_S2(j, i) + lab.c2;

                    /// bounding mechanisms
                    if ( c2 > UB + eps ) continue;
                    if ( c1 + LB_veh[i] > UB + eps ) continue;
                    if ( c1 + c2 + LB_tot[i] > 2*UB + eps ) continue;     

                    /// Adding without dominance
                    tmp.push_back( dataLabel(c1, c2, j, cur) );
                }
            }

            /// dominance
            sort( tmp.begin(), tmp.end(), [] (dataLabel u, dataLabel v){
                return u.c1 < v.c1;
            } );

            for (dataLabel lab : tmp) {
                if ( L[i].empty() ) L[i].push_back(lab);
                else {
                    if ( L[i].back().c1 <= lab.c1 && L[i].back().c2 <= lab.c2 ) continue;
                    L[i].push_back(lab);
                }
            }

                // cerr << "LAG: " << i << " " << L[i].size() << " " << tmp.size() << '\n';
        }
            // exit(0);
    }

    void split(const tigersugar::Instance &instance, tigersugar::Tour &vehicle, tigersugar::Drone_Tour &drone, double UB1) {
        numDrone = instance.numDrone;

        V.clear();
        V.push_back(0);
        for (int x : vehicle.points) if (x != 0) V.push_back(x);
        for (vector<int> idDrone : drone.node) for (int x : idDrone) V.push_back(x);
        V.push_back(0);

        assert( (int) V.size() == instance.numPoint+2 && V[0] == 0 && V.back() == 0 );

        Build_Graph(instance);
        process(instance, UB1);

        /// trace_back to split
        int v = V.size()-1;
        bool flag = false;
        dataLabel bestLab;
        for (dataLabel lab : L[v]) {
            if ( !flag ) { bestLab = lab; flag = true; }
            if ( max(lab.c1, lab.c2) + eps < max(bestLab.c1, bestLab.c2) ) bestLab = lab;
        }

        if (!flag) {
                // cerr << "LOZ:\n";
                // vehicle.debug(instance);
                // exit(0);
            return;
        }

            // cerr << "#dm\n";

        vehicle.points.clear();    
        drone.node.clear(); 
            // cerr << "?? " << v << '\n';
            //cerr << "?? " ,< b
            // exit(0);

        vehicle.add(0);
        while (v != 0) {
            int u = bestLab.preNode, pos = bestLab.pos;
                // cerr << "?? " << u << " " << pos << " " << L[u].size() << '\n';

            if ( u+1 <= v-1 && drone.empty() ) drone.add_drone(); 
            FOR(i, u+1, v-1) drone.node[0].push_back(V[i]);
            vehicle.add(V[u]);

            v = u;
            bestLab = L[u][pos]; 
        }
    }
}