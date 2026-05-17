#include <iostream>
#include "lazy_sequence.h"
#include "src/generators/rule_generator.h"
#include "LABA_2/mutableArraySequence.h"

int main() {

    /*====================================================*/
    /*================ FIBONACCI SOURCE ==================*/
    /*====================================================*/

    int items[] = {1, 1};

    Sequence<int>* cache =
        new MutableArraySequence<int>(
            items,
            2
        );

    auto fib_rule = [](Sequence<int>* seq)
    {
        int n = seq->get_length();

        return seq->get(n - 1)
             + seq->get(n - 2);
    };

    Generator<int>* fib_generator =new RuleGenerator<int>(fib_rule,cache);

    LazySequence<int> fib(fib_generator,cache);

    /*====================================================*/
    /*===================== MAP TEST =====================*/
    /*====================================================*/

    auto square = [](const int& x)
    {
        std::cout<< "TRANSFORM: "<< x<< " -> "<< x * 2<< std::endl;

        return x * 2;
    };

    LazySequence<int>* doubled = fib.map(square);

    std::cout << std::endl;
    std::cout << "FIRST CALL" << std::endl;

    std::cout << doubled->get(0) << std::endl;
    std::cout << doubled->get(1) << std::endl;
    std::cout << doubled->get(5) << std::endl;

    std::cout << std::endl;
    std::cout << "SECOND CALL" << std::endl;

    std::cout << doubled->get(5) << std::endl;

    delete doubled;

    /*====================================================*/
    /*==================== WHERE TEST ====================*/
    /*====================================================*/

    auto even = [](const int& x)
    {
        std::cout<< "CHECK: "<< x<< std::endl;

        return x % 2 == 0;
    };

    LazySequence<int>* evens = fib.where(even);

    std::cout << std::endl;
    std::cout << "WHERE FIRST CALL" << std::endl;

    std::cout << evens->get(0) << std::endl;
    std::cout << evens->get(1) << std::endl;
    std::cout << evens->get(2) << std::endl;

    std::cout << std::endl;
    std::cout << "WHERE SECOND CALL" << std::endl;

    std::cout << evens->get(2) << std::endl;

    delete evens;

    return 0;
}