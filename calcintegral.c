#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

/**
 * Function to change as fit to user 
*/
double f(double x)
{
    return (4.0) / (1 + x * x);
}

double calc_rect(double height, double width)
{
    return height * width;
}

int main(int argc, char const *argv[])
{
    //Read number of threads
    int NUM_THREADS;
    printf("Specify number of threads\n");
    scanf("%d", &NUM_THREADS);
    printf("Num of threads is %d\n", NUM_THREADS);
    //ensure threads do  not exceed max threads
    if (NUM_THREADS > omp_get_max_threads())
    {
        NUM_THREADS = omp_get_max_threads();
    }

    //Read lower bound
    int lower;
    printf("Specify lower bound\n");
    scanf("%d", &lower);
    printf("lower bound is %d\n", lower);

    //Read upper bound
    int upper;
    printf("Specify upper bound\n");
    scanf("%d", &upper);
    printf("upper bound is %d\n", upper);

    //Read number of steps
    int steps;
    printf("Specify number of steps\n");
    scanf("%d", &steps);
    printf("number of steps %d\n", steps);

    //Calculate increment
    double step = 1.0 / steps;

    //Area
    double integral = 0;

    omp_set_num_threads(NUM_THREADS);


#pragma omp parallel shared(integral)
    {   
        double local_integral = 0;
        int my_rank = omp_get_thread_num();
        #pragma omp for schedule(static,16) 
        for (int i = lower; i <= steps; i++)
        {
            double x = (0.5 + i) * step;
            double y = f(x);
            local_integral = local_integral + calc_rect(y, step);
        }

        #pragma omp critical
        {
            printf("Hi, I am %d and my local integral is %f\n", my_rank, local_integral);
            integral = integral + local_integral;
        }
        #pragma omp barrier
    }

    //Display integral
    printf("Area = %f\n", integral);

    return 0;
}
