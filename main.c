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
 * sum(buf1[i] * buf2[i]) within a window
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
  int w, shift, peak;
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
  w = BUFSZ / 4;
  peak = 0;
  int start = 0;
  int end = 0;
  int *ptr = &start;
  // let k = the sum of the products
  int maxsofar = 0;
  int k = 0;

  // period idx2 - idx1;
  for (shift = 1; shift < w; shift++) {
    for (i = 0; i < w; i++) {
      k += input[i] * input[i + shift];
    }
    if (shift == 1) {
      maxsofar = k; // record first
      continue;
    }
    if (k > maxsofar) {
      maxsofar = k;
      // record index of peak
      *ptr = shift;
    } else {
      // we know that prevk is a peak
      // start tracking end
      ptr = &end;
    }
  }

  // period = end - start (in num samples)
  // calc freq using period
  return 0;
}
