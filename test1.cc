#include <iostream>
#include <array>
#include <chrono>
#include <random>
#include <bitset>
#include <fstream>


uint16_t randU16();
uint32_t randU32();


uint16_t randU16() {
    uint32_t x = __builtin_readcyclecounter() & 0xffff;
    x ^= x << 7;
    x ^= x >> 9;
    x ^= x << 8;
    return static_cast<uint16_t>(x);
}

uint32_t randU32() {
    uint32_t x = __builtin_ia32_rdtsc() & 0xffffffff;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    return x;
}

uint32_t randU32_lehmer();
uint32_t randU32_lehmer() {
    __uint128_t x = static_cast<uint32_t>(__builtin_ia32_rdtsc());
    x *= 0xda942042e4dd58b5;
    return static_cast<uint32_t>(x >> 64);
}

static uint64_t wyhash64_x = __builtin_readcyclecounter();
static uint64_t counter = 0;

uint64_t wyhash64();
uint64_t wyhash64() {

    //__builtin_ia32_aesdec();

    // if (!(counter++ & 0xf)) [[unlikely]] {
    //     wyhash64_x = __builtin_readcyclecounter();
    // }
    // auto wyhash64_x = __builtin_readcyclecounter();
    __uint128_t tmp;

    wyhash64_x += 0x60bee2bee120fc15;

    tmp = (__uint128_t)wyhash64_x * 0xa3b195354a39b70d;
    tmp = (__uint128_t)((tmp >> 64) ^ tmp) * 0x1b03738712fad5c9;

    uint64_t m2 = (tmp >> 64) ^ tmp;
    return m2;
}


static uint32_t _crc32_seed = __builtin_readcyclecounter();
static uint32_t _crc32_iter = 0x4a39b70d;
uint32_t randU32_final() {
    // return _crc32_iter = __builtin_ia32_crc32si(_crc32_seed, _crc32_iter), _crc32_iter;

    // _crc32_seed = __builtin_ia32_crc32si(_crc32_seed, _crc32_iter++);
    // return _crc32_seed;

    _crc32_seed = __builtin_ia32_crc32si(_crc32_seed, _crc32_seed & 0x0000ffff);
    return _crc32_seed;

    // uint64_t rip;
    // asm volatile("lea (%%rip), %0" : "=r"(rip));
    // _crc32_seed = __builtin_ia32_crc32si(rip, _crc32_seed);
    // return __builtin_ia32_crc32si(__builtin_readcyclecounter(), _crc32_seed);
}
// uint32_t randU32_wyhash() {
//     uint64_t x = static_cast<uint32_t>(__builtin_readcyclecounter());
//     x += 0x60bee2bee120fc15;
//     __uint128_t tmp;
//     tmp = static_cast<__uint128_t>(x) * 0xa3b195354a39b70d;
//     uint64_t m1 = (tmp >> 64) ^ tmp;
//     tmp = static_cast<__uint128_t>(m1) * 0x1b03738712fad5c9;
//     uint64_t m2 = (tmp >> 64) ^ tmp;
//     return static_cast<uint32_t>(m2);
// }

/*
uint64_t wyhash64() {
  wyhash64_x += 0x60bee2bee120fc15;
  __uint128_t tmp;
  tmp = (__uint128_t) wyhash64_x * 0xa3b195354a39b70d;
  uint64_t m1 = (tmp >> 64) ^ tmp;
  tmp = (__uint128_t)m1 * 0x1b03738712fad5c9;
  uint64_t m2 = (tmp >> 64) ^ tmp;
  return m2;
}
*/

#define RAND_TEST_SIZE 31250
static std::array<uint32_t, RAND_TEST_SIZE> t1;


int main() {

    __builtin_printf("%d\n", RAND_MAX);


    {
        auto start1 = std::chrono::high_resolution_clock::now();

        for (size_t i = 0; i < t1.size(); i++)
            t1[i] = __builtin_bit_cast(uint32_t, rand());

        auto stop1 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop1 - start1);
        std::cout << "rand(): " << duration.count() << " microseconds" << std::endl;
    }

    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<uint32_t> dis(0, 0xffffffff);

        auto start1 = std::chrono::high_resolution_clock::now();

        for (size_t i = 0; i < t1.size(); i++)
            t1[i] = dis(gen);

        auto stop1 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop1 - start1);
        std::cout << "std::mt19937: " << duration.count() << " microseconds" << std::endl;
    }

    {
        auto start1 = std::chrono::high_resolution_clock::now();

        for (size_t i = 0; i < t1.size(); i++)
            t1[i] = randU32();

        auto stop1 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop1 - start1);
        std::cout << "randU32: " << duration.count() << " microseconds" << std::endl;
    }

    {
        auto start1 = std::chrono::high_resolution_clock::now();

        for (size_t i = 0; i < t1.size(); i++)
            t1[i] = static_cast<uint32_t>(wyhash64());

        auto stop1 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop1 - start1);
        std::cout << "randU32_wyhash: " << duration.count() << " microseconds" << std::endl;
    }

    {
        auto start1 = std::chrono::high_resolution_clock::now();

        for (size_t i = 0; i < t1.size(); i++)
            t1[i] = randU32_final();

        auto stop1 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop1 - start1);
        std::cout << "randU32_final: " << duration.count() << " microseconds" << std::endl;
    }

    /*
    {
        auto start1 = std::chrono::high_resolution_clock::now();

        for (size_t i = 0; i < t1.size(); i++)
            t1[i] = static_cast<uint32_t>(wyhash64());

        auto stop1 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop1 - start1);
        std::cout << "randU32_wyhash: " << duration.count() << " microseconds" << std::endl;
    }
    */


    std::ofstream oss("out.txt", std::ios::binary);

    for (auto x : t1) {
        oss << std::bitset<32>(x);
    }

    oss.close();


    // {
    //     auto start1 = std::chrono::high_resolution_clock::now();

    // for (size_t i = 0; i < t1.size(); i++)
    //     t1[i] = randU32_wyhash();

    // auto stop1 = std::chrono::high_resolution_clock::now();
    // auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop1 - start1);
    // std::cout << "randU32_wyhash: " << duration.count() << " microseconds" << std::endl;
    // }

    return 0;
}
