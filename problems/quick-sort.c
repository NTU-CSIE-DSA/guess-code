#include <stdio.h>

int a[100];

void function(int left, int right) {
  if (right - left < 2) {
    return;
  }
  int pivot = a[left];
  int i = left + 1;

  for (int j = i; j < right; ++j) {
    if (a[j] < pivot) {
      int tmp = a[i];
      a[i] = a[j];
      a[j] = tmp;
      ++i;
    }
  }

  int tmp = a[i - 1];
  a[i - 1] = a[left];
  a[left] = tmp;

  function(left, i - 1);
  function(i, right);
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
