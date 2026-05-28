#ifndef LABA4_STATISTICS_PROCESSOR_H
#define LABA4_STATISTICS_PROCESSOR_H

// читает поток, обновляет статистики, медиану, показывает резы
#include "../streams/input_stream.h"
#include "online_statistics.h"
#include "sliding_median.h"

template<class T>
class StatisticsProcessor {

private:

    InputStream<T>* stream; // источник данных

    OnlineStatistics<T> statistics;

    SlidingMedian<T> median;

public:

    StatisticsProcessor(InputStream<T>* stream,int median_window_size);

    void process_next();

    double get_average() const;
    double get_median() const;

    const T& get_min() const;
    const T& get_max() const;
    const T& get_sum() const;

    int get_count() const;

    bool is_empty() const;
};

template<class T>
StatisticsProcessor<T>::StatisticsProcessor(InputStream<T>* stream,int median_window_size)
    : stream(stream), median(median_window_size) {}

template<class T>
void StatisticsProcessor<T>::process_next() {
    if(stream->is_end_of_stream())
        throw std::out_of_range("End of stream");

    T value = stream->read();

    statistics.add(value);

    median.add(value);
}

template<class T>
double StatisticsProcessor<T>::get_average() const {
    return statistics.get_average();
}

template<class T>
double StatisticsProcessor<T>::get_median() const {
    return median.get_median();
}

template<class T>
const T& StatisticsProcessor<T>::get_min() const {
    return statistics.get_min();
}

template<class T>
const T& StatisticsProcessor<T>::get_max() const {
    return statistics.get_max();
}

template<class T>
const T& StatisticsProcessor<T>::get_sum() const {
    return statistics.get_sum();
}

template<class T>
int StatisticsProcessor<T>::get_count() const {
    return statistics.get_count();
}

template<class T>
bool StatisticsProcessor<T>::is_empty() const {
    return statistics.is_empty();
}

#endif //LABA4_STATISTICS_PROCESSOR_H
