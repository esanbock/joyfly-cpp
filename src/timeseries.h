#ifndef TIMESERIES_H
#define TIMESERIES_H

#include <vector>

using namespace std;

template<typename TX, typename TY>
class TimeSeries{
private:
    std::vector<TX> _time;
    std::vector<TY> _val;
public:
    const TX* GetTimes()
    {
        return &_time[0];
    }

    const TY* GetVals()
    {
        return &_val[0];
    }

    const size_t GetSize()
    {
        return _time.size();
    }

    void Add(TX x,TY y)
    {
        _time.push_back(x);
        _val.push_back(y);
    }
};

#endif // TIMESERIES_H
