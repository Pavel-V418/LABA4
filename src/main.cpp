#include <iostream>
#include <cstdlib>
#include "../src/lazy/lazy_sequence.h"
#include "../src/generators/stream_number_generator.h"
#include "../src/streams/sequence_input_stream.h"
#include "../src/statistics/statistics_processor.h"
#include "../src/generators/fibonacci_generator.h"
#include "../src/generators/random_generator.h"
#include "../src/streams/sequence_output_stream.h"

// menus
void main_menu();
void lazy_sequence_menu();
void streams_menu();
void statistics_menu();

// streams
LazySequence<int>* create_stream_sequence();

void demo_stream_read();
void demo_stream_seek();
void demo_stream_write();
void demo_stream_open_close();

// statistics demos
void demo_arithmetic_pipeline();
void demo_random_pipeline();

int main() {
    main_menu();

    return 0;
}

void main_menu() {
    bool running = true;

    while(running){

        std::cout << "\n LABA 4 - STREAM PROCESSING SYSTEM\n";

        std::cout << "1. LazySequence\n";
        std::cout << "2. Streams\n";
        std::cout << "3. Online Stream Statistics\n";
        std::cout << "0. Exit\n";

        int choice;

        std::cout << "Choice: ";
        std::cin >> choice;

        switch(choice){

            case 1:
                lazy_sequence_menu();
                break;

            case 2:
                streams_menu();
                break;

            case 3:
                statistics_menu();
                break;

            case 0:
                running = false;
                break;

            default:
                std::cout << "Invalid choice\n";
        }
    }
}

void lazy_sequence_menu() {
    LazySequence<int>* current_sequence = nullptr;

    bool running = true;

    while(running){
        std::cout << "LAZY SEQUENCE MENU\n";

        if(current_sequence == nullptr)
            std::cout << "Current sequence: NOT CREATED\n";

        std::cout << "1. Create sequence\n";
        std::cout << "2. Show sequence\n";
        std::cout << "3. Get element\n";
        std::cout << "4. Take\n";
        std::cout << "5. Skip\n";
        std::cout << "6. Map (*2)\n";
        std::cout << "7. Filter (%2)\n";
        std::cout << "8. Subsequence\n";
        std::cout << "9. Append\n";
        std::cout << "10. Prepend\n";
        std::cout << "11. InsertAt\n";
        std::cout << "12. Concat\n";
        std::cout << "13. Get by Cardinal\n";
        std::cout << "14. Show length\n";
        std::cout << "15. Delete sequence\n";
        std::cout << "0. Back\n";

        int choice;

        std::cout << "Choice: ";
        std::cin >> choice;

        switch(choice){

            case 1: {

                if(current_sequence != nullptr){

                    delete current_sequence;
                    current_sequence = nullptr;
                }

                std::cout << "Choose generator type:\n\n";

                std::cout << "1. Arithmetic progression\n";
                std::cout << "2. Fibonacci\n";
                std::cout << "3. Random\n";

                int generator_choice;

                std::cout << "Choice: ";
                std::cin >> generator_choice;

                switch(generator_choice){

                    case 1: {
                        int start;
                        int step;

                        std::cout << "Start value: ";
                        std::cin >> start;

                        std::cout << "Step: ";
                        std::cin >> step;

                        auto factory = [start, step]() {
                            return new StreamNumberGenerator<int>(start,step);
                        };

                        current_sequence = new LazySequence<int>(factory,Cardinal::infinity(),100);

                        std::cout << "Arithmetic sequence created.\n";

                        break;
                    }

                    case 2: {
                        auto factory = []() {
                            return new FibonacciGenerator<int>();
                        };

                        current_sequence = new LazySequence<int>(factory,Cardinal::infinity(),100);

                        std::cout << "Fibonacci sequence created.\n";

                        break;
                    }

                    case 3: {
                        int min_value;
                        int max_value;

                        std::cout << "Minimum value: ";
                        std::cin >> min_value;

                        std::cout << "Maximum value: ";
                        std::cin >> max_value;

                        auto factory = [min_value, max_value]() {
                            return new RandomGenerator<int>(min_value,max_value);
                        };

                        current_sequence = new LazySequence<int>(factory,Cardinal::infinity(),100);

                        std::cout << "Random sequence created.\n";

                        break;
                    }

                        default:

                            std::cout << "Invalid generator type.\n";
                            }

                            break;
            }

            case 2: {
                if(current_sequence == nullptr){

                    std::cout << "Create sequence first.\n";

                    break;
                }

                int count;

                std::cout << "Elements count: ";
                std::cin >> count;

                for(int i = 0; i < count; i++)
                    std::cout << current_sequence->get(i) << " ";

                std::cout << std::endl;

                break;
            }

            case 3: {
                if(current_sequence == nullptr){
                    std::cout << "Create sequence first.\n";

                    break;
                }

                int index;

                std::cout << "Index: ";
                std::cin >> index;
                std::cout << "Element = " << current_sequence->get(index) << std::endl;

                break;
            }

            case 4: {
                if(current_sequence == nullptr){
                    std::cout << "Create sequence first.\n";
                    break;
                }

                int count;

                std::cout << "Take count: ";
                std::cin >> count;

                LazySequence<int>* result = current_sequence->take(count);

                current_sequence = result;

                std::cout << "Sequence updated.\n";

                break;
            }

            case 5: {
                if(current_sequence == nullptr){
                    std::cout << "Create sequence first.\n";
                    break;
                }

                int skip_count;

                std::cout << "Skip count: ";
                std::cin >> skip_count;

                LazySequence<int>* result = current_sequence->skip(skip_count);

                current_sequence = result;

                std::cout << "Sequence updated.\n";

                break;
            }

            case 6: {
                if(current_sequence == nullptr){
                    std::cout << "Create sequence first.\n";
                    break;
                }

                LazySequence<int>* mapped = current_sequence->map([](const int& x){
                            return x * 2;
                });

                current_sequence = mapped;

                std::cout << "Sequence updated.\n";

                break;
            }

            case 7: {
                if(current_sequence == nullptr){
                    std::cout << "Create sequence first.\n";
                    break;
                }

                LazySequence<int>* filtered =current_sequence->where([](const int& x){
                            return x % 2 == 0;
                        }
                    );

                current_sequence = filtered;

                std::cout << "Sequence updated.\n";

                break;
            }

            case 8: {
                if(current_sequence == nullptr){
                    std::cout << "Create sequence first.\n";
                    break;
                }

                int start;
                int end;

                std::cout << "Start index: ";
                std::cin >> start;

                std::cout << "End index: ";
                std::cin >> end;

                LazySequence<int>* sub = current_sequence->get_subsequence(start,end);

                current_sequence = sub;

                std::cout << "Sequence updated.\n";

                break;
            }

            case 9: {
                if(current_sequence == nullptr){
                    std::cout << "Create sequence first.\n";
                    break;
                }

                int value;

                std::cout << "Append value: ";
                std::cin >> value;

                LazySequence<int>* result = current_sequence->append(value);

                current_sequence = result;

                std::cout << "Sequence updated.\n";

                break;
            }

            case 10: {
                if(current_sequence == nullptr){
                    std::cout << "Create sequence first.\n";
                    break;
                }

                int value;

                std::cout << "Prepend value: ";
                std::cin >> value;

                LazySequence<int>* result = current_sequence->prepend(value);

                current_sequence = result;

                std::cout << "Sequence updated.\n";

                break;
            }

            case 11: {

                if(current_sequence == nullptr){
                    std::cout << "Create sequence first.\n";
                    break;
                }

                int value;
                int index;

                std::cout << "Value: ";
                std::cin >> value;

                std::cout << "Index: ";
                std::cin >> index;

                LazySequence<int>* result =
                    current_sequence->insert_at(
                        value,
                        index
                    );

                current_sequence = result;

                std::cout << "Sequence updated.\n";

                break;
            }

            case 12: {
                if(current_sequence == nullptr){
                    std::cout << "Create sequence first.\n";
                    break;
                }

                int start;
                int step;

                std::cout << "Second sequence start: ";
                std::cin >> start;

                std::cout << "Second sequence step: ";
                std::cin >> step;

                auto factory = [start, step]() {
                    return new StreamNumberGenerator<int>(start, step);
                };

                auto* second = new LazySequence <int> (factory,Cardinal::infinity(),100);

                current_sequence = current_sequence->concat(second);

                std::cout << "Sequence concatenated.\n";

                break;
            }

            case 13: {

                if(current_sequence == nullptr){
                    std::cout << "Create sequence first.\n";
                    break;
                }

                int omega;
                int offset;

                std::cout << "Omega count: ";
                std::cin >> omega;

                std::cout << "Offset: ";
                std::cin >> offset;

                try{
                    Cardinal index(omega, offset);

                    std::cout
                        << "Element = "
                        << current_sequence->get(index)
                        << std::endl;
                }
                catch(const std::exception& e){

                    std::cout
                        << "Error: "
                        << e.what()
                        << std::endl;
                }

                break;
            }

            case 14: {

                if(current_sequence == nullptr){
                    std::cout << "Create sequence first.\n";
                    break;
                }

                Cardinal len =
                    current_sequence->get_length();

                std::cout
                    << len.get_omega_count()
                    << "w + "
                    << len.get_offset()
                    << std::endl;

                break;
            }

            case 15: {

                if(current_sequence != nullptr){

                    delete current_sequence;

                    current_sequence = nullptr;

                    std::cout << "Sequence deleted.\n";
                }

                else
                    std::cout << "Sequence does not exist.\n";

                break;
            }

            case 0: {

                running = false;

                break;
            }

            default:
                std::cout << "Invalid choice\n";
        }
    }

    if(current_sequence != nullptr)
        delete current_sequence;
}

void streams_menu() {

    bool running = true;

    while(running){

        std::cout << "\nSTREAMS MENU\n";

        std::cout << "1. Read stream\n";
        std::cout << "2. Seek stream\n";
        std::cout << "3. Open / Close stream\n";
        std::cout << "4. Write stream\n";
        std::cout << "0. Back\n";

        int choice;

        std::cout << "Choice: ";
        std::cin >> choice;

        try{

            switch(choice){

                case 1:
                    demo_stream_read();
                    break;

                case 2:
                    demo_stream_seek();
                    break;

                case 3:
                    demo_stream_open_close();
                    break;

                case 4:
                    demo_stream_write();
                    break;

                case 0:
                    running = false;
                    break;

                default:
                    std::cout << "Invalid choice\n";
            }
        }
        catch(const std::exception& e){

            std::cout
                << "Error: "
                << e.what()
                << std::endl;
        }
    }
}

LazySequence<int>* create_stream_sequence() {

    std::cout << "\nChoose generator:\n";

    std::cout << "1. Arithmetic progression\n";
    std::cout << "2. Fibonacci\n";
    std::cout << "3. Random\n";

    int choice;

    std::cout << "Choice: ";
    std::cin >> choice;

    switch(choice){

        case 1: {

            int start;
            int step;

            std::cout << "Start value: ";
            std::cin >> start;

            std::cout << "Step: ";
            std::cin >> step;

            auto factory = [start, step]() {
                return new StreamNumberGenerator<int>(start, step);
            };

            return new LazySequence<int>(factory,Cardinal::infinity(),100);
        }

        case 2: {

            auto factory = []() {
                return new FibonacciGenerator<int>();
            };

            return new LazySequence<int>(factory,Cardinal::infinity(),100);
        }

        case 3: {

            int min_value;
            int max_value;

            std::cout << "Minimum value: ";
            std::cin >> min_value;

            std::cout << "Maximum value: ";
            std::cin >> max_value;

            auto factory = [min_value, max_value]() {
                return new RandomGenerator<int>(
                    min_value,
                    max_value
                );
            };

            return new LazySequence<int>(
                factory,
                Cardinal::infinity(),
                100
            );
        }

        default:
            throw std::logic_error(
                "Invalid generator type"
            );
    }
}

void demo_stream_read() {

    LazySequence<int>* sequence =
        create_stream_sequence();

    SequenceInputStream<int> stream(sequence);

    stream.open();

    int count;

    std::cout << "Read count: ";
    std::cin >> count;

    for(int i = 0; i < count; i++)
        std::cout << stream.read() << " ";

    std::cout << std::endl;

    stream.close();

    delete sequence;
}

void demo_stream_seek() {

    LazySequence<int>* sequence =
        create_stream_sequence();

    SequenceInputStream<int> stream(sequence);

    stream.open();

    int position;

    std::cout << "Seek position: ";
    std::cin >> position;

    stream.seek(position);

    std::cout
        << "Value = "
        << stream.read()
        << std::endl;

    stream.close();

    delete sequence;
}

void demo_stream_open_close() {

    LazySequence<int>* sequence =
        create_stream_sequence();

    SequenceInputStream<int> stream(sequence);

    std::cout << "Opening stream...\n";

    stream.open();

    std::cout
        << "Reading value: "
        << stream.read()
        << std::endl;

    std::cout << "Closing stream...\n";

    stream.close();

    std::cout << "Stream closed.\n";

    delete sequence;
}

void demo_stream_write() {

    MutableArraySequence<int> sequence;

    SequenceOutputStream<int> stream(&sequence);

    stream.open();

    int count;

    std::cout << "Elements count: ";
    std::cin >> count;

    for(int i = 0; i < count; i++) {

        int value;

        std::cout << "Value #" << i + 1 << ": ";
        std::cin >> value;

        stream.write(value);
    }

    stream.close();

    Sequence<int>* result =
        stream.get_target();

    std::cout << "\nStored values:\n";

    for(int i = 0; i < result->get_length().get_offset(); i++)
        std::cout << result->get(i) << " ";

    std::cout << std::endl;
}

void statistics_menu() {

    bool running = true;

    while(running){

        std::cout << "ONLINE STREAM STATISTICS MENU\n";

        std::cout << "1. Arithmetic stream pipeline\n";
        std::cout << "2. Random stream pipeline\n";
        std::cout << "0. Back\n";

        int choice;

        std::cout << "Choice: ";
        std::cin >> choice;

        switch(choice){

            case 1:
                demo_arithmetic_pipeline();
                break;

            case 2:
                demo_random_pipeline();
                break;

            case 0:
                running = false;
                break;

            default:
                std::cout << "Invalid choice\n";
        }
    }
}

void demo_arithmetic_pipeline() {

    int start;
    int step;
    int window_size;
    int count;

    std::cout << "Start value: ";
    std::cin >> start;

    std::cout << "Step: ";
    std::cin >> step;

    std::cout << "Median window size: ";
    std::cin >> window_size;

    std::cout << "Elements count: ";
    std::cin >> count;

    auto factory = [start, step]() {
        return new StreamNumberGenerator<int>(start,step);
    };

    LazySequence<int> sequence(factory,Cardinal::infinity(),100);

    SequenceInputStream<int> stream(&sequence);

    stream.open();

    StatisticsProcessor<int> processor(&stream,window_size);

    for(int i = 0; i < count; i++){

        processor.process_next();

        std::cout << "Generated value: " << sequence.get(i) << std::endl;
        std::cout << "Count: " << processor.get_count() << std::endl;
        std::cout << "Average: " << processor.get_average() << std::endl;
        std::cout << "Median: " << processor.get_median() << std::endl;
        std::cout << "Min: " << processor.get_min()<< std::endl;
        std::cout << "Max: " << processor.get_max() << std::endl;
    }

    stream.close();
}

void demo_random_pipeline() {

    int window_size;
    int count;

    std::cout << "Median window size: ";
    std::cin >> window_size;

    std::cout << "Elements count: ";
    std::cin >> count;

    auto factory = []() {
        return new StreamNumberGenerator<int>(std::rand() % 100,std::rand() % 10 + 1);
    };

    LazySequence<int> sequence(factory,Cardinal::infinity(),100);

    SequenceInputStream<int> stream(&sequence);
    stream.open();
    StatisticsProcessor<int> processor(&stream,window_size);

    for(int i = 0; i < count; i++){

        processor.process_next();

        std::cout << "Generated value: " << sequence.get(i) << std::endl;
        std::cout << "Count: " << processor.get_count() << std::endl;
        std::cout << "Average: " << processor.get_average() << std::endl;
        std::cout << "Median: " << processor.get_median() << std::endl;
        std::cout << "Min: " << processor.get_min() << std::endl;
        std::cout << "Max: " << processor.get_max() << std::endl;
    }

    stream.close();
}