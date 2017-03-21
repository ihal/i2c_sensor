#include <iostream>
#include <thread>
#include <chrono>


void governor() {
  std::chrono::milliseconds t1(0);
  std::chrono::milliseconds sleep_time(1000);

  t1=std::chrono::duration_cast<std::chrono::milliseconds>(
    std::chrono::system_clock::now().time_since_epoch()
  );
  sleep_time= std::chrono::milliseconds(500)+(std::chrono::milliseconds(1000) - (t1 % std::chrono::milliseconds(1000)));

  std::this_thread::sleep_for( std::chrono::milliseconds(sleep_time));
}

