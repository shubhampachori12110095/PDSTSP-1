#include "template.cpp"
#include "config.cpp"
#include "problem.cpp"
#include "solver.cpp"
#include "timer.cpp"

int main(int argc, char *argv[])
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    Config::load_config_from_file();

    //Config::parse_arguments(argc, argv);

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

    Timer::timer t;
    int attempt = 0;
    Solver::init();
    while (Solver::unchanged_time < Constant::CONVERGING_THRESHOLD)
    {
        t.set_checkpoint();
        cerr << "\n==================================\n";
        cerr << "Attempt #" << ++attempt << ": ";
        Solver::split();
        Solver::optimize_vehicle_and_drones_tour();
        Solver::update_result();
        cerr << "Time: " << t.since_last_checkpoint() << "\n";
        cerr << "\n==================================\n";
    }

    Problem::validate_result();

    cerr << "\nFinal result: " << Problem::result;
    cerr << "\nFinal vehicle result: " << Problem::vResult;
    cerr << "\nFinal drone result: " << Problem::dResult;
    cerr << "\nTime: " << t.since_beginning();
    cerr << "\n" << Problem::result << " " << Problem::vResult << " " << Problem::dResult;
    Problem::print_result_to_file(Config::output);

}
