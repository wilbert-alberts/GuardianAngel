#include <string>
#include <vector>

#include "Time24.hpp"
#include "Time24Interval.hpp"

class AngelRepository {
    // depends on GSM
    public:
        void processRequests();
    private:
        std::vector<Angel&> angels;
};

class Angel
{
    std::string name;
    std::string phoneNumber;
    std::vector<WatchInterval&> watchIntervals;
};

class WatchInterval
{
public:
    WatchInterval(const Angel& angel, const Time24& start, const Time24& end, int activityLevel);
    void processInterval(const Time24& now, bool activityDetected, bool helpRequested);

    const Angel& angel;
    Time24Interval* interval;

};

class WatchDog {
public:
    void addInterval(WatchInterval& i);
    void delInterval(WatchInterval& i);

    void processIntervals(); // 


    std::vector<WatchInterval&> intervals;
}