#include "template.cpp"
#include "config.cpp"
#include "problem.cpp"
#include "solver.cpp"
#include "validator.cpp"

#include "paper/iterative_two_step_heuristic.cpp"
#include "paper_Input_adaptation.cpp"

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

    tigersugar::Instance instance = Input_Adaptation::Process();
    interative_two_step_heuristic::Process(instance);
    paper_validator::checking();

    cerr << "Number of customers: " << Problem::n << "\n";
    cerr << "Number of drones: " << Problem::m << "\n";
    cerr << "Number of drone_eligible: " << Problem::nD << "\n\n";
    
        cerr << "RESULT: " << Problem::result << "\n\n";
        
        cerr << "VEHICLE TOUR: ";
        for (int x : Problem::vTour) cerr << x << " ";
        cerr << "\n\n";
        
        cerr << "DRONE TOUR:\n";
        for (int i = 0; i < (int) Problem::dTour.size(); ++i) {
            cerr << "drone " << i+1 << ": ";
            for (int x : Problem::dTour[i]) cerr << x << " ";
            cerr << '\n';
        }


    // Problem::print_result_to_file(Config::output);
}
