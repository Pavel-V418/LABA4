#include <iostream>
#include "generators/rule_generator.h"
#include "lazy_sequence.h"
#include "streams/sequence_input_stream.h"
#include "streams/sequence_output_stream.h"
#include "streams/file_input_stream.h"
#include "streams/file_output_stream.h"
#include "streams/string_input_stream.h"
#include "streams/string_output_stream.h"
#include "../LABA_2/mutableArraySequence.h"

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

    Sequence<int>* cache = new MutableArraySequence<int>();

    Generator<int>* fib_generator = new RuleGenerator<int>(fib_rule, cache,true);

    LazySequence<int> fib(fib_generator,cache,Cardinal::infinity());


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


    delete doubled;

    delete evens;

    delete first_three;

    delete even_take;

    delete concat_seq;

    /*================================================*/
    /*============== STRING INPUT TEST ===============*/
    /*================================================*/

    std::cout
        << "========== STRING INPUT TEST =========="
        << std::endl;

    StringInputStream string_input(
        "Hello"
    );

    string_input.open();

    while(
        !string_input.is_end_of_stream()
    )
    {
        std::cout
            << string_input.read();
    }

    std::cout << std::endl;

    string_input.close();

    std::cout << std::endl;


    /*================================================*/
    /*============= STRING OUTPUT TEST ===============*/
    /*================================================*/

    std::cout
        << "========== STRING OUTPUT TEST =========="
        << std::endl;

    StringOutputStream string_output;

    string_output.open();

    string_output.write('H');
    string_output.write('i');
    string_output.write('!');

    std::cout
        << string_output.get_string()
        << std::endl;

    std::cout
        << "POSITION: "
        << string_output.get_position()
        << std::endl;

    string_output.close();

    std::cout << std::endl;


    /*================================================*/
    /*=============== FILE OUTPUT TEST ===============*/
    /*================================================*/

    std::cout
        << "========== FILE OUTPUT TEST =========="
        << std::endl;

    FileOutputStream file_output(
        "test.txt"
    );

    file_output.open();

    file_output.write('A');
    file_output.write('B');
    file_output.write('C');

    file_output.close();

    std::cout
        << "Written to file test.txt"
        << std::endl;

    std::cout << std::endl;


    /*================================================*/
    /*================ FILE INPUT TEST ===============*/
    /*================================================*/

    std::cout
        << "========== FILE INPUT TEST =========="
        << std::endl;

    FileInputStream file_input(
        "test.txt"
    );

    file_input.open();

    try
    {
        while(true)
        {
            std::cout
                << file_input.read();
        }
    }
    catch(const std::out_of_range&)
    {
    }

    std::cout << std::endl;

    file_input.close();

    std::cout << std::endl;


    /*================================================*/
    /*================= SEEK TEST ====================*/
    /*================================================*/

    std::cout
        << "========== SEEK TEST =========="
        << std::endl;

    StringInputStream seek_stream(
        "abcdef"
    );

    seek_stream.open();

    seek_stream.seek(3);

    std::cout
        << seek_stream.read()
        << std::endl;

    seek_stream.close();

    std::cout << std::endl;


    /*================================================*/
    /*=========== SEQUENCE OUTPUT TEST ===============*/
    /*================================================*/

    std::cout
        << "========== SEQUENCE OUTPUT TEST =========="
        << std::endl;

    auto* sequence =
        new MutableArraySequence<char>();

    SequenceOutputStream<char>
        sequence_output(sequence);

    sequence_output.open();

    sequence_output.write('X');
    sequence_output.write('Y');
    sequence_output.write('Z');

    sequence_output.close();

    for(
        int i = 0;
        i < sequence->get_length().get_value();
        i++
    )
    {
        std::cout
            << sequence->get(i);
    }

    std::cout << std::endl;

    std::cout << std::endl;


    /*================================================*/
    /*============ SEQUENCE INPUT TEST ===============*/
    /*================================================*/

    std::cout
        << "========== SEQUENCE INPUT TEST =========="
        << std::endl;

    SequenceInputStream<char>
        sequence_input(sequence);

    sequence_input.open();

    while(
        !sequence_input.is_end_of_stream()
    )
    {
        std::cout
            << sequence_input.read();
    }

    std::cout << std::endl;

    sequence_input.close();

    delete sequence;

    return 0;
}