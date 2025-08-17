#include <stdio.h>
#include <omp.h>

#define BUFF_SIZE 5
#define PRODUCE_COUNT 10

int buffer[BUFF_SIZE];
int in = 0, out = 0;
int count = 0;
int prod_item = 0;
int cons_item = 0;

int main() {
    omp_set_num_threads(2);
    double start = omp_get_wtime();

    #pragma omp parallel sections shared(buffer, in, out, count, prod_item, cons_item)
    {
        // Producer
        #pragma omp section
        {
            while (prod_item < PRODUCE_COUNT) {
                #pragma omp critical
                {
                    if (count < BUFF_SIZE) {
                        int item = prod_item + 1;
                        buffer[in] = item;
                        in = (in + 1) % BUFF_SIZE;
                        prod_item++;
                        count++;
                        printf("Producer produced: %d\n", item);
                    }
                }
                #pragma omp flush
            }
        }

        // Consumer
        #pragma omp section
        {
            while (cons_item < PRODUCE_COUNT) {
                #pragma omp critical
                {
                    if (count > 0) {
                        int item = buffer[out];
                        out = (out + 1) % BUFF_SIZE;
                        cons_item++;
                        count--;
                        printf("Consumer consumed: %d\n", item);
                    }
                }
                #pragma omp flush
            }
        }
    }

    double end = omp_get_wtime();
    printf("Total time: %f sec\n", end - start);

    return 0;
}
