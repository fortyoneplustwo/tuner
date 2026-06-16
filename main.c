#include <math.h>
#include <stdio.h>

#define BITRATE 44100
#define BUFSZ BITRATE * 5

/**
 * Find how many samples for a period
 * a b c d a b c d a b c d
 *   a b c d a b c d a b c d
 * 				a ....
 *
 * buf1, buf2
 * sum(buf1[i] * buf2[i])
 * keep going until we hit second peak
 * samples between peak1 and peak2;
 *
 * shift amount (T) = period in in samples
 *
 * frequency = 1/T
 * frequency = bitrate/T
 *
 * cmp(f, f_0);
 */

int main() {
  // 44.1KHz = 44100 samples/s
  size_t i;
  double input[BUFSZ];

  for (i = 0; i < BUFSZ; i++) {
    input[i] = sin((double)i);
  }

  for (i = 0; i < BUFSZ; i++) {
    printf("%f\n", input[i]);
  }

	// algorithm
	// must determine period
	

  return 0;
}
