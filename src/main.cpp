#include <iostream>
#include "generators/rule_generator.h"
#include "lazy_sequence.h"

int main() {

    /*====================================================*/
    /*================ FIBONACCI RULE ====================*/
    /*====================================================*/

    auto fib_rule = [](Sequence<int>* seq)
    {
        int size = seq->get_length().get_value();

        if(size == 0)
            return 1;

        if(size == 1)
            return 1;

        return seq->get(size - 1)+ seq->get(size - 2);
    };

    /*====================================================*/
    /*=================== FIBONACCI ======================*/
    /*====================================================*/

    Sequence<int>* cache =
        new MutableArraySequence<int>();

    Generator<int>* fib_generator = new RuleGenerator<int>(
            fib_rule,
            cache,
            true
        );

    LazySequence<int> fib(fib_generator,cache,Cardinal::infinity());

    /*====================================================*/
    /*==================== BASIC TEST ====================*/
    /*====================================================*/

    std::cout << "========== BASIC TEST ==========" << std::endl;

    std::cout << fib.get(0) << std::endl;
    std::cout << fib.get(1) << std::endl;
    std::cout << fib.get(2) << std::endl;
    std::cout << fib.get(3) << std::endl;
    std::cout << fib.get(10) << std::endl;

    std::cout << std::endl;

    /*====================================================*/
    /*===================== MAP TEST =====================*/
    /*====================================================*/

    auto multiply_by_2 = [](const int& x)
    {
        std::cout
            << "TRANSFORM: "
            << x
            << " -> "
            << x * 2
            << std::endl;

        return x * 2;
    };

    LazySequence<int>* doubled =
        fib.map(multiply_by_2);

    std::cout << "========== MAP TEST ==========" << std::endl;

    std::cout << doubled->get(0) << std::endl;
    std::cout << doubled->get(1) << std::endl;
    std::cout << doubled->get(5) << std::endl;

    std::cout << std::endl;

    std::cout << "MAP CACHE TEST" << std::endl;

    std::cout << doubled->get(5) << std::endl;

    std::cout << std::endl;

    /*====================================================*/
    /*==================== WHERE TEST ====================*/
    /*====================================================*/

    auto even = [](const int& x)
    {
        std::cout
            << "CHECK: "
            << x
            << std::endl;

        return x % 2 == 0;
    };

    LazySequence<int>* evens =
        fib.where(even);

    std::cout << "========== WHERE TEST ==========" << std::endl;

    std::cout << evens->get(0) << std::endl;
    std::cout << evens->get(1) << std::endl;
    std::cout << evens->get(2) << std::endl;

    std::cout << std::endl;

    std::cout << "WHERE CACHE TEST" << std::endl;

    std::cout << evens->get(2) << std::endl;

    std::cout << std::endl;

    /*====================================================*/
    /*===================== TAKE TEST ====================*/
    /*====================================================*/

    LazySequence<int>* first_three =
        fib.take(3);

    std::cout << "========== TAKE TEST ==========" << std::endl;

    std::cout << first_three->get(0) << std::endl;
    std::cout << first_three->get(1) << std::endl;
    std::cout << first_three->get(2) << std::endl;

    std::cout << std::endl;

    std::cout << "TAKE LENGTH TEST" << std::endl;

    std::cout
        << first_three
               ->get_length()
               .get_value()
        << std::endl;

    std::cout << std::endl;

    /*====================================================*/
    /*==================== REDUCE TEST ===================*/
    /*====================================================*/

    auto sum = [](const int& a, const int& b)
    {
        std::cout
            << "REDUCE: "
            << a
            << " + "
            << b
            << " = "
            << a + b
            << std::endl;

        return a + b;
    };

    LazySequence<int>* even_take =
        fib
            .where(even)
            ->take(3);

    std::cout << "========== REDUCE TEST ==========" << std::endl;

    int result =
        even_take->reduce(sum, 0);

    std::cout << std::endl;

    std::cout
        << "FINAL RESULT: "
        << result
        << std::endl;

    std::cout << std::endl;

    /*====================================================*/
    /*==================== CONCAT TEST ===================*/
    /*====================================================*/

    LazySequence<int>* concat_seq =
        fib
            .take(3)
            ->concat(
                fib.take(2)
            );

    std::cout << "========== CONCAT TEST ==========" << std::endl;

    std::cout << concat_seq->get(0) << std::endl;
    std::cout << concat_seq->get(1) << std::endl;
    std::cout << concat_seq->get(2) << std::endl;
    std::cout << concat_seq->get(3) << std::endl;
    std::cout << concat_seq->get(4) << std::endl;

    std::cout << std::endl;

    /*====================================================*/
    /*=================== CARDINAL TEST ==================*/
    /*====================================================*/

    std::cout << "========== CARDINAL TEST ==========" << std::endl;

    Cardinal inf =
        fib.get_length();

    if(inf.is_infinite())
    {
        std::cout
            << "Fibonacci length is INFINITE"
            << std::endl;
    }

    Cardinal finite =
        first_three->get_length();

    std::cout
        << "take(3) length = "
        << finite.get_value()
        << std::endl;

    std::cout << std::endl;

    /*====================================================*/
    /*====================== CLEANUP =====================*/
    /*====================================================*/

    delete doubled;

    delete evens;

    delete first_three;

    delete even_take;

    delete concat_seq;

    return 0;
}