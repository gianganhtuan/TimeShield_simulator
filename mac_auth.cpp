#include <iostream>
#include <vector>
#include <random>
#include <iomanip>

// Function to generate a list of event times following a Poisson process
std::vector<double> generatePoissonEventTimes(double rate, double totalTime) {
    std::vector<double> eventTimes;
    std::random_device rd;
    std::mt19937 gen(rd());

    // Exponential distribution for inter-arrival times
    std::exponential_distribution<> expDist(rate);

    double currentTime = 0.0;

    // Generate event times until we reach the total time limit
    while (currentTime < totalTime) {
        double interArrivalTime = expDist(gen);  // Time until the next event
        currentTime += interArrivalTime;  // Update the current time with the inter-arrival time

        if (currentTime < totalTime) {
            eventTimes.push_back(currentTime);  // Record the event time if within the total time
        }
    }

    return eventTimes;
}

// Function to count events within a specific time interval [t0, t0 + delta_t]
int countEventsInInterval(const std::vector<double>& eventTimes, double t0, double delta_t) {
    int count = 0;
    double t1 = t0 + delta_t;

    for (double eventTime : eventTimes) {
        if (eventTime >= t0 && eventTime <= t1) {
            ++count;
        }
    }

    return count;
}

int main(int argc, char* argv[]) {
    // Check if there are enough arguments (program name + 2 arguments)
    if (argc < 3) {
        std::cerr << "Usage: ./mac_auth [real AP rate] [delta_t]\n";
        return 1;
    }

    int noSample=1000;
    int noFail=0;
    double rate=std::strtod(argv[1], nullptr);
    int totalTime=1000000;
    int t0=500000;
    int delta_t=std::strtod(argv[2], nullptr);

    for (int i=0;i<=noSample;i++) {
    // Generate event times
    std::vector<double> eventTimes = generatePoissonEventTimes(rate, totalTime);
    //std::cout << "Debug: event size: "<< eventTimes.size() <<std::endl;
    // Count and display the number of events in the interval
    int eventCount = countEventsInInterval(eventTimes, t0, delta_t);
    if (eventCount == 0) noFail++;
    }
    std::cout << 1 - (double) noFail/noSample << std::endl;
    return 0;
}
