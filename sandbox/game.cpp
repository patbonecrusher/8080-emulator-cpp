#include <iostream>
#include <chrono>

void gameFunction()
{
    // start()
    // end()
}

int main()
{
    auto t1 = std::chrono::high_resolution_clock::now();
    gameFunction();
    auto t2 = std::chrono::high_resolution_clock::now();

    auto elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();

    std::cout << elapsed_time << std::endl;
    return 0;
}
