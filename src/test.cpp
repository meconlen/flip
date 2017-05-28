#include <cstdint>
#include <iostream>
#include <random>

#include <sys/time.h>
#include <sys/resource.h>

double get_cpu_time(void)
{
   struct rusage ru;
   getrusage(RUSAGE_SELF, &ru);
   double time = ru.ru_utime.tv_sec + ru.ru_stime.tv_sec + ((double)(ru.ru_utime.tv_usec + ru.ru_stime.tv_usec))/((double)(1000000));
   return time;
}

void test_flips(uint64_t n)
{
   std::random_device rd;
   std::mt19937 re(rd());
   std::uniform_int_distribution<int> flipper(0, 1);
   
   uint64_t count_heads = 0;
   uint64_t max_run = 0;
   for(uint64_t i = 0; max_run < n; i++) {
      uint_fast8_t flip = flipper(re);
      if(flip == 1) {
         count_heads++;
         if(count_heads > max_run) {
            max_run++;
            double time = get_cpu_time();
            std::cout << max_run << "\t" << i << "\t" << time << std::endl;
         }
      } else {
         count_heads = 0;
      }
   }
}

int main(int argc, char** argv)
{
   test_flips(500);
	return 0;
}