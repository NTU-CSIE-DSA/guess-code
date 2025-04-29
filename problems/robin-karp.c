#include <stdio.h>
#include <string.h>
#define d 256
#define q 101

int ANS;

void function(char *AAA, char *BBB) {
  int n = strlen(AAA);
  int m = strlen(BBB);
  int i, j;
  int p = 0;
  int t = 0;
  int h = 1;

  for (i = 0; i < m - 1; i++)
    h = h * d;
  h %= q;

  for (i = 0; i < m; i++) {
    p = d * p;
    p += BBB[i];
    p %= q;
    t = d * t;
    t += AAA[i];
    t %= q;
  }

  for (i = 0; i <= n - m; i++) {
    if (p == t) {
      for (j = 0; j < m; j++) {
        if (AAA[i + j] != BBB[j])
          break;
      }
      if (j == m)
        ANS++;
    }

    if (i < n - m) {
      t = (d * (t - AAA[i] * h) + AAA[i + m]) % q;
      if (t < 0)
        t = (t + q);
    }
  }
}

int main() {
  char AAA[1000], BBB[100];
  scanf("%s", AAA);

  scanf("%s", BBB);

  function(AAA, BBB);
  printf("%d", ANS);
  return 0;
}
