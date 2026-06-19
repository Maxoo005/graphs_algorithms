#include "Parameters.h"

int main(int argc, char** argv)
{
    if (Parameters::readParameters(argc - 1, argv + 1) != 0) {
        return 1;
    }

    switch (Parameters::runMode) {
        case Parameters::RunModes::help:
            Parameters::help();
            break;

        case Parameters::RunModes::singleFile:
            // TODO: implementacja trybu pojedynczego testu
            break;

        case Parameters::RunModes::benchmark:
            // TODO: implementacja trybu badan
            break;

        default:
            Parameters::help();
            break;
    }

    return 0;
}
