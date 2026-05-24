#ifndef LABA4_CARDINAL_H
#define LABA4_CARDINAL_H

#include <stdexcept>

class Cardinal {

private:
    bool infinite;
    int value;

public:

    Cardinal() : infinite(false), value(0) {}

    Cardinal(int value) : infinite(false), value(value) {}

    static Cardinal infinity() {
        Cardinal inf;

        inf.infinite = true;

        return inf;
    }

    bool is_infinite() const {
        return infinite;
    }

    int get_value() const {
        if(infinite)
            throw std::logic_error("Infinite cardinal has no finite value");

        return value;
    }

    bool operator==(const Cardinal& other) const { // сравнивает 2 мощности
        if (infinite && other.infinite)
            return true;

        if (infinite || other.infinite)
            return false;

        return value == other.value;
    }

    bool operator!=(const Cardinal& other) const {
        return !(*this == other);
    }

    Cardinal operator+(const Cardinal& other) const { // сложение мощностей
        if(infinite || other.infinite)
            return infinity();

        return Cardinal(value + other.value);
    }
};




#endif //LABA4_CARDINAL_H
