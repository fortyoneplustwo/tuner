#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define BITRATE 44100
#define BUFSZ 44100 // 1 second of audio

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Normalized Square Difference Function (NSDF) — McLeod Pitch Method.
// Values in [-1, 1]: 1 = perfect correlation, -1 = perfect anti-correlation.
static double nsdf(const double *x, int n, int shift) {
  double num = 0.0, denom = 0.0;
  int m = n - shift;
  for (int i = 0; i < m; i++) {
    num += x[i] * x[i + shift];
    denom += x[i] * x[i] + x[i + shift] * x[i + shift];
  }
  return (denom < 1e-12) ? 0.0 : 2.0 * num / denom;
}

int main() {
  // Test signal: A4 = 440 Hz, properly scaled to sample rate
  double freq_in = 440.0;
  double *input = malloc(BUFSZ * sizeof(double));
  if (!input) {
    fprintf(stderr, "malloc failed\n");
    return 1;
  }

  for (int i = 0; i < BUFSZ; i++) {
    input[i] = sin(2.0 * M_PI * freq_in * i / BITRATE);
  }

  // Audible frequency range: 20 Hz – 5 kHz
  // period = BITRATE / freq, so invert the bounds:
  int min_shift = BITRATE / 5000; //   9 samples (~5 kHz)
  int max_shift = BITRATE / 20;   // 2205 samples (~20 Hz)

  // McLeod Pitch Method: find the first peak in the NSDF that exceeds a
  // threshold, but only AFTER the function has first gone negative.
  // This skips spurious high harmonics near shift=0.
  double threshold = 0.8;
  double best_val = -1.0;
  int period = 0;
  int gone_negative = 0;
  int in_peak = 0;

  double prev = nsdf(input, BUFSZ, min_shift);

  for (int shift = min_shift + 1; shift <= max_shift; shift++) {
    double cur = nsdf(input, BUFSZ, shift);

    if (!gone_negative && cur < 0.0)
      gone_negative = 1;

    if (gone_negative) {
      if (!in_peak && cur > threshold) {
        in_peak = 1;
        best_val = cur;
        period = shift;
      } else if (in_peak) {
        if (cur > best_val) {
          best_val = cur;
          period = shift; // walk up to true peak of this lobe
        } else if (prev > cur) {
          break; // past the peak — first lobe done
        }
      }
    }
    prev = cur;
  }

  if (period == 0) {
    printf("No pitch detected (signal too weak or out of range)\n");
  } else {
    double frequency = (double)BITRATE / (double)period;
    printf("Input frequency:    %.4f Hz\n", freq_in);
    printf("Detected period:    %d samples\n", period);
    printf("Detected frequency: %.4f Hz\n", frequency);
    printf("Error:              %.4f cents\n",
           1200.0 * log2(frequency / freq_in));
  }

  free(input);
  return 0;
}
