#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>


int count = 0;
std::mutex mtx;
std::condition_variable cv;

void safe_increment() {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, []{ return true; });
    count++;
    mtx.unlock();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << std::this_thread::get_id() << ": " << count << '\n';
}

int main() {
    std::cout << "main: " << count << '\n';
    std::thread threads[10];
    for (int i=0; i<10; ++i) {
        threads[i] = std::thread(&safe_increment);
    }
    std::cout << "10 threads ready to start...\n";
    cv.notify_all();
    for (auto& th : threads) th.join();
    std::cout << "main: " << count << std::endl;
}
