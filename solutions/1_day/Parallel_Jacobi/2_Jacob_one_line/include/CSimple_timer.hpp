#include <chrono>
#include <map>
#include <string>
#include <algorithm>
#include <utility>
#include <iostream>
/**
 *  USE THIS AS
 *  {CSimple_timer t("WHATEVER YOU ARE TIMING");
 *        THE CODE YOU ARE TIMING
 *    }
 *
 *  THEN AT THE END OF MAIN PUT
 *  CSimple_timer::print_timing_results();
 */
struct Time{

  long long int timings;
  int num_calls;

};

std::map<std::string, struct Time> table;

class CSimple_timer {
public:

  using time_units = std::chrono::milliseconds;

  // DECLARE VARIABLES FOR WHAT WE ARE TIMING, FOR CLOCK START AND END
  std::chrono::time_point<std::chrono::steady_clock> t_start;
  std::chrono::time_point<std::chrono::steady_clock> t_end;
 
  struct Time TimerTime; 
  
  long long int duration;

  std::string timewhat;   

  // constructior
  CSimple_timer(const std::string &timewhat0) {
    // SET WHAT WE ARE TIMING FROM THE PASSED PARAMETER
    timewhat = timewhat0;
    // LOOK FOR THE STRING OCCURENCE AND EITHER AUGMENT THE CALLS OR
    if(table.find(timewhat) != table.end()){
      table[timewhat].num_calls++;  
      TimerTime = table[timewhat];

    } else {
    // INSERT A NEW THING INTO THE TABLE
      TimerTime.num_calls = 1;
      TimerTime.timings = 0;
      table.insert(std::make_pair(timewhat, TimerTime));
    }
    // START THE CLOCKS
    t_start = std::chrono::steady_clock::now();
  };

  // destructor
  ~CSimple_timer() {
    // STOP THE CLOCK
    t_end = std::chrono::steady_clock::now();
    // CALCULATE DURATION
    duration =
        std::chrono::duration_cast<std::chrono::microseconds>(t_end - t_start)
            .count();
    // INSERT THAT INTO YOUR TABLE
      table[timewhat].timings += duration; 
  } // destructor

  // here comes important new info: static class functions - they can be called
  // without the object of the class
  static void print_timing_results(); // IMPLEMENT THIS
}; // of class

void CSimple_timer::print_timing_results() {
  for (const auto &pair : table) { 
    double av = (double)(pair.second).timings/(pair.second).num_calls;
    std::cout << pair.first << " called " << (pair.second).num_calls << " times " <<  "with a cummulative time: " << (pair.second).timings << " and an average time: "<< av << "micro s" << std::endl;
  }
}

