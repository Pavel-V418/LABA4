#ifndef LABA4_ONLINE_STATISTIC_H
#define LABA4_ONLINE_STATISTIC_H
#include <stdexcept>

template<class T>
class OnlineStatistics {

private:
    T sum;
    T min_value;
    T max_value;

    int count; // сколько элементов обработано
    bool empty;

public:
    OnlineStatistics();

    void add(const T& value);

    double get_average() const;

    const T& get_min() const;
    const T& get_max() const;
    const T& get_sum() const;

    int get_count() const;

    bool is_empty() const;

};

template<class T>
OnlineStatistics<T>::OnlineStatistics()
    : sum(0), count(0), empty(true) {}

template<class T>
void OnlineStatistics<T>::add(const T& value) {

    if(empty){
        min_value = value;
        max_value = value;

        empty = false;
    }

    else{
        if(value < min_value)
            min_value = value;

        if(value > max_value)
            max_value = value;
    }

    sum += value;

    count++;
}

template<class T>
double OnlineStatistics<T>::get_average() const { //среднее
    if(empty)
        throw std::logic_error("Statistics are empty");

    return static_cast<double>(sum) / count;
}

template<class T>
const T& OnlineStatistics<T>::get_min() const {
    if(empty)
        throw std::logic_error("Statistics are empty");

    return min_value;
}

template<class T>
const T&
OnlineStatistics<T>::get_max() const {
    if(empty)
        throw std::logic_error("Statistics are empty");

    return max_value;
}

template<class T>
const T& OnlineStatistics<T>::get_sum() const {
    return sum;
}

template<class T>
int OnlineStatistics<T>::get_count() const {
    return count;
}

template<class T>
bool OnlineStatistics<T>::is_empty() const {
    return empty;
}

#endif //LABA4_ONLINE_STATISTIC_H
