#include "template.cpp"
#include "config.cpp"
#include "problem.cpp"
#include "solver.cpp"

#include "tsp/tsp_optimizer.cpp"

int main(int argc, char *argv[])
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    Config::parse_arguments(argc, argv);

    Problem::import_data_from_tsplib_instance(
        Config::input,
        Config::customer,
        Config::drone,
        Config::vehicle_speed,
        Config::drone_speed
    );

    cerr << "Number of customers: " << Problem::n << "\n";
    cerr << "Number of drones: " << Problem::m << "\n";
    cerr << "Number of drone_eligible: " << Problem::nD << "\n";

    // Problem::print_result_to_file(Config::output);

}
