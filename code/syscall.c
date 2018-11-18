#include <u.h>
#include <libc.h>

#define TRIALS_SIZE 64

// Either errstr(2) seems to be the lightest - no errored syscall before
// pipe seems to take 6x the amount of time as errstr
double time_syscall(void) {
    char err_str[4];
    uvlong time_tot = 0;
    uvlong time_begin, time_end;

	ulong i;
	// start trying to read time
    for (i = 0; i < 16384; i++) {
        cycles(&time_begin);
        rerrstr(err_str, 4);
        cycles(&time_end);
        time_tot = time_tot + (time_end - time_begin);
	}

    return time_tot / (16384.0);
}

double calc_mean(double trials[TRIALS_SIZE]) {
	double mean = 0.0;

	ulong i;
    for (i = 0; i < TRIALS_SIZE; i++) {
		mean += trials[i];
    }
    mean = mean / TRIALS_SIZE;

    return mean;
}

double calc_stddev(double trials[TRIALS_SIZE], double mean) {
	double stddev = 0.0;

    ulong i;
	for (i = 0; i < TRIALS_SIZE; i++) {
		double diff = trials[i] - mean;	
		stddev = stddev + (diff * diff);
	}
    stddev = stddev / TRIALS_SIZE;
    stddev = sqrt(stddev);

    return stddev;
}

void main(int argc, char *argv[]) {
	// Contains trial timings in nanoseconds
    double trials[TRIALS_SIZE];
    
	ulong i;
    for (i = 0; i < TRIALS_SIZE; i++) {
         trials[i] = time_syscall();
	} 

    double mean = calc_mean(trials);
    double stddev = calc_stddev(trials, mean);

	print("(cycles) mean: %f\t stddev: %f\n", mean, stddev);

	exits(nil);
}
