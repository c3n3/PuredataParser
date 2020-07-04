#pragma once
#include <chrono>
#include <thread>
#include <iostream>
#include <bitset>
using namespace std::literals::chrono_literals;
struct Timer {
    std::chrono::high_resolution_clock::time_point  start, end;
    static std::chrono::high_resolution_clock::time_point begin;
    std::chrono::duration<float> duration;
    Timer(const char *);
    const char* name;
    static double millis();
    ~Timer();
};

double Timer::millis() {
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now - begin);
    return duration.count() / 2000;
}

Timer::Timer(const char * name): name(name) {
    start = std::chrono::high_resolution_clock::now();
}

Timer::~Timer() {
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << name <<" Timer: " << duration.count() * 1000.0 << " ms\n";
}
std::chrono::high_resolution_clock::time_point Timer::begin = std::chrono::high_resolution_clock::now();