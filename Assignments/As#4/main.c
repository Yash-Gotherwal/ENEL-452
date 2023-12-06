#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h> // Include for rand()

#define LUT_SIZE 1440
#define SCALE_FACTOR 100000

// Lookup table for sin values
int sin_lut[LUT_SIZE];

void build_sin_lut() {
    for (int i = 0; i < LUT_SIZE; i++) {
        double angle = (i - 719) * 0.5;
        sin_lut[i] = (int)(sin(angle * M_PI / 180) * SCALE_FACTOR);
    }
}

int sin_from_lut(int index) {
    if (index < 0 || index >= LUT_SIZE) {
        printf("Index out of bounds\n");
        return 0;
    }
    return sin_lut[index];
}

int main() {
    build_sin_lut();

    clock_t start, end;
    double lut_time_used, sin_time_used;
    
    // Measure LUT performance
    start = clock();
    for (long i = 0; i < 1000000000; i++) {
        sin_from_lut(rand() % LUT_SIZE);
    }
    end = clock();
    lut_time_used = ((double) (end - start)) / CLOCKS_PER_SEC / 1000000000;
    printf("table lookup: %f ns\n", lut_time_used * 1e9);

    // Measure standard sin function performance
    start = clock();
    for (long i = 0; i < 1000000000; i++) {
        sin(((rand() % 719) - 359.5) * M_PI / 180);
    }
    end = clock();
    sin_time_used = ((double) (end - start)) / CLOCKS_PER_SEC / 1000000000;
    printf("Built in sin(): %f ns\n", sin_time_used * 1e9);

    // Calculate speedup
    printf("Table Speedup: %fx\n", sin_time_used / lut_time_used);

    return 0;
}
