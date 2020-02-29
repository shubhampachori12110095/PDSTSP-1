#ifndef PSDTSP_CONFIG
#define PSDTSP_CONFIG

#include "template.cpp"
#include "constant.cpp"

namespace Config
{
    string input;
    bool found_input = false;

    string output = "";

    int customer;
    bool found_customer = false;

    int drone = 1;

    double vehicle_speed = Constant::DEFAULT_VEHICLE_SPEED;
    double drone_speed = Constant::DEFAULT_DRONE_SPEED;

    void parse_arguments(int argc, char *argv[])
    {
        for(int i = 1; i < argc; ++i)
        {
            string key = argv[i];

            if (key == "--input")
            {
                string value = argv[++i];
                input = value;
                found_input = true;
            }
            else
            if (key == "--output")
            {
                string value = argv[++i];
                output = value;
            }
            else
            if (key == "--customer")
            {
                int value = stoi(argv[++i]);
                customer = value;
                found_customer = true;
            }
            else
            if (key == "--drone")
            {
                int value = stoi(argv[++i]);
                drone = value;
            }
            else
            if (key == "--vehicle-speed")
            {
                int value = stof(argv[++i]);
                vehicle_speed = value;
            }
            else
            if (key == "--drone-speed")
            {
                int value = stof(argv[++i]);
                drone_speed = value;
            }
            else
            {
                cerr << "Unknow argument " << argv[i] << "!\n";
                exit(0);
            }

        }

        if (!found_input)
            {
                cerr << "Input is required!\n";
                exit(0);
            }
            if (!found_customer)
            {
                cerr << "Number of customers is required!\n";
                exit(0);
            }
    }
}

#endif