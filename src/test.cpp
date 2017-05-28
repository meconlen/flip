#include <cmath>
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

uint64_t expected_flips(uint64_t n)
{
   return std::pow(2, n+1)-2;
}

void test_flips(uint64_t n)
{
   std::random_device rd;
   std::mt19937 re(rd());
   std::uniform_int_distribution<int> flipper(0, 1);
   
   uint64_t count_heads = 0;
   uint64_t max_run = 0;
   uint64_t heads = 0, tails = 0;
   std::cout << "run\tactual\texpected\tdifference\t% diff\theads\ttails\tratio diff\ttime" << std::endl;
   for(uint64_t i = 1; max_run < n; i++) {
      uint_fast8_t flip = flipper(re);
      if(flip == 1) {
         heads++;
         count_heads++;
         if(count_heads > max_run) {
            max_run++;
            double time = get_cpu_time();
            uint64_t expected = expected_flips(max_run);
            int64_t difference = (double)i - expected;
            double p_diff = ((double)difference) / ((double)expected);
            double ratio_diff = (double)(1) - (double)(heads)/(double)(tails);
            std::cout << max_run << "\t" << i << "\t" << expected << "\t" << difference << "\t" << p_diff << "\t" << heads << "\t" << tails << "\t" << ratio_diff << "\t" << time << std::endl;
         }
      } else {
         tails++;
         count_heads = 0;
      }
   }
}

int main(int argc, char** argv)
{
   std::cout << "expected flips = " << expected_flips(500) << std::endl;
   test_flips(500);
	return 0;
}