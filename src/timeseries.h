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

    size_t GetSize()
    {
        return _time.size();
    }

    void Add(TX x,TY y)
    {
        _time.push_back(x);
        _val.push_back(y);
    }

    void AddOrUpdate(TX x,TY y)
    {
        if( _val.size() > 1 && _val.back() == y  && _val.at(_val.size()-2) == y )
        {
            _time.back() = x;
        }
        else
        {
            Add(x,y);
        }
    }

    void Clear()
    {
        _time.clear();
        _val.clear();
    }
};

#endif // TIMESERIES_H
