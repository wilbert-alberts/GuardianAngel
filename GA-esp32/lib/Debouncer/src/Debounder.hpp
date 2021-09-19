/*
 * Time24Interval.hpp
 *
 *  Created on: 19 sep. 2021
 *      Author: wilbert
 */

#ifndef DEBOUNCER_HPP_
#define DEBOUNCER_HPP_

typedef int (*ValueProvider)();

class Debouncer {
public:
    Debouncer(int stabilizationPeriod, ValueProvider valueProvider);

    int getValue() const;

private:
    ValueProvider valueProvider;
    int stabilizationPeriod;

    int stableValue;
    int stablePeriods;
    int unstableValue;

    void tick();
};


Debouncer::Debouncer(int stabilizationPeriod, ValueProvider vp) 
: valueProvider(vp)
, stabilizationPeriod(stabilizationPeriod)
{
    stableValue = valueProvider();
    unstableValue = stableValue;
    stablePeriods = 0;
}

int Debouncer::getValue() const {
    return stableValue;
}

void Debouncer::tick() {
    int v = valueProvider();
    if (v != unstableValue) {
        stablePeriods = 0;
        unstableValue = v;
    }
    else {
        if (stablePeriods < stabilizationPeriod) {
            stablePeriods++;
        }
        else {
            stableValue = unstableValue;
        }
    }
}


#endif