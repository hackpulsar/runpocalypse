#ifndef RANDOMIZE_HPP
#define RANDOMIZE_HPP

#include <random>

namespace Randomize
{

    template <typename type>
    type GetRandom(type min, type max) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<type> dist(min, max);
        return dist(gen);
    }

}

#endif // RANDOMIZE_HPP
