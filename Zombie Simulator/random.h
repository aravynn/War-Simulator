#pragma once

/**
 *
 * Random Class will produce random numbers on a specific seed.
 * an undefined seed will generate based on the time. seed can be saved for future use.
 *
 *
 */

#include <random>
#include <chrono>
#include <type_traits>

template<class T>
class random
{
private:
    static inline int m_seed{ -1 };
    std::mt19937 m_mt;
public:

    random() {
        if (m_seed < 0) {
            std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
            auto duration = now.time_since_epoch();
            m_seed = (int)std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

            m_mt = std::mt19937(m_seed);
        }
    }

    random(int seed) {

        m_seed = seed;

        std::mt19937 mt(m_seed);

    }

    T num(T min, T max) {

        if constexpr (std::is_integral_v<T>) {

            std::uniform_int_distribution<int> dist(min, max);
            return dist(m_mt);

        }
        else {

            std::uniform_real_distribution<double> dist(min, max);
            return dist(m_mt);

        }

    }
    int getSeed() { return m_seed; }

};