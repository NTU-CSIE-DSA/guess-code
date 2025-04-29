#include <stdio.h>

int a[100];

void function(int l, int r) {
  if (r - l < 2) {
    return;
  }
  int t = a[l];
  int i = l + 1;

  for (int j = i; j < r; ++j) {
    if (a[j] < t) {
      int tmp = a[i];
      a[i] = a[j];
      a[j] = tmp;
      ++i;
    }
  }

  int tmp = a[i - 1];
  a[i - 1] = a[l];
  a[l] = tmp;

  function(l, i - 1);
  function(i, r);
}

int main() {
  int n = 10;
  for (int i = 0; i < n; ++i) {
    scanf("%d", &a[i]);
  }
  function(0, n);
  for (int i = 0; i < n; ++i) {
    printf("%d%c", a[i], " \n"[i == n - 1]);
  }
  return 0;
}
