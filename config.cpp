#ifndef PSDTSP_CONFIG
#define PSDTSP_CONFIG

#include "template.cpp"
#include "constant.cpp"

namespace Config
{
    string input;
    bool found_input = false;

    string output;

    int customer;
    bool found_customer = false;

    int drone = 1;

    double vehicle_speed = Constant::DEFAULT_VEHICLE_SPEED;
    double drone_speed = Constant::DEFAULT_DRONE_SPEED;

    string config_file_name = "config.txt";

    void load_config_from_file()
    {
        ifstream input_file(config_file_name);
        string key;
        while (input_file >> key)
        {
            transform(key.begin(), key.end(), key.begin(), ::toupper);
            if (key == "INPUT:")
            {
                found_input = true;
                input_file >> input;
            }
            else if (key == "OUTPUT:") input_file >> output;
            else if (key == "CUSTOMER:")
            {
                found_customer = true;
                input_file >> customer;
            }
            else if (key == "DRONE:") input_file >> drone;
            else if (key == "VEHICLE_SPEED:") input_file >> vehicle_speed;
            else if (key == "DRONE_SPEED:") input_file >> drone_speed;
            else
            {
                cerr << "Unknown argument " << key << "!\n";
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
                cerr << "Unknown argument " << argv[i] << "!\n";
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
