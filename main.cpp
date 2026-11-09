// Variant 15.
#include <chrono>
#include <future>
#include <iostream>
#include <string>
#include <syncstream>
#include <thread>

using namespace std;

void slow(const string& name) {
    this_thread::sleep_for(chrono::seconds(7));
    osyncstream(cout) << name << endl;
}

void quick(const string& name) {
    this_thread::sleep_for(chrono::seconds(1));
    osyncstream(cout) << name << endl;
}

void work() {
    auto start = chrono::steady_clock::now();

    // launch::async forces a new thread; without it the call may be deferred.
    future<void> a2 = async(launch::async, slow, "A2");
    future<void> c = async(launch::async, [] {
        quick("A3");
        quick("A4");
        quick("C");
        });

    slow("A1");
    a2.get();  // B needs both A1 and A2
    quick("B");
    quick("D");
    c.get();

    auto finish = chrono::steady_clock::now();
    double seconds = chrono::duration<double>(finish - start).count();
    osyncstream(cout) << "Time: " << seconds << " s" << endl;
    osyncstream(cout) << "Work is done!" << endl;
}

int main() {
    work();
    return 0;
}