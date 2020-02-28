#include "template.cpp"
#include "config.cpp"
#include "problem.cpp"

int main(int argc, char *argv[])
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    Config::parse_arguments(argc, argv);

    Problem::readDataFromTSPLIBInstance(
        Config::input,
        Config::customer,
        Config::drone,
        Config::vehicle_speed,
        Config::drone_speed
    );

    cerr << "Number of customers: " << Problem::n << "\n";
    cerr << "Number of drones: " << Problem::m << "\n";
    cerr << "Number of drone_eligible: " << Problem::nD << "\n";
}
