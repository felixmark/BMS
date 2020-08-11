#include <time.h>

class Clock
{
        private:
        clock_t t;
        bool still;
        double bisherwert;
        public:
        Clock()
        {
                still = true;
                bisherwert = 0.0;
        }
        void start()
        {
                still = false;
                t = clock();
        }
        double stop()
        {
                if (still != true)
                        t = clock() - t;
                still = true;
                return ((double)t)/CLOCKS_PER_SEC;
        }
        double stopexact()
        {
                if (still != true)
                        t = clock() - t;
                still = true;
                return t;
        }
        double is()
        {
                return (double) (((double)(clock() - t))/CLOCKS_PER_SEC);
        }
};
