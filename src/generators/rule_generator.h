#ifndef LABA4_RULE_GENERATOR_H
#define LABA4_RULE_GENERATOR_H

#include "generator.h"
#include "../../LABA_2/sequence.h"

// знает как вычислить сл элемент
template<class T>
class RuleGenerator : public Generator<T> {

public:
    RuleGenerator(std::function<T(Sequence<T>*)> rule, Sequence<T>* materialized, bool infinite = true);

    T get_next() override;
    T get(const Cardinal& index) override;

    bool has_next() const override;

private:

    std::function<T(Sequence<T>*)> rule; // правило генерации следующего элемента
    Sequence<T>* materialized; // хранит указатель на все уже вычесленные элементы(сами элементы лежат в LazySequence
    bool infinite; // флаг - конечная ли посл-ть или бесконечная
};

template<class T>
RuleGenerator<T>::RuleGenerator(std::function<T(Sequence<T> *)> rule, Sequence<T> *materialized, bool infinite)
    : rule(rule), materialized(materialized), infinite(infinite) {}

template<class T>
T RuleGenerator<T>::get_next() {
    return rule(materialized); // передает mat в rule, далее вычислется новый элемент и возвращается это значение
}

template<class T>
bool RuleGenerator<T>::has_next() const{
    return infinite;
}

template<class T>
T RuleGenerator<T>::get(const Cardinal&){
    throw std::logic_error("RuleGenerator does not support indexed access");
}

#endif //LABA4_RULE_GENERATOR_H
