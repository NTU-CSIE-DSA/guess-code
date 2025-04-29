#include <stdio.h>

int functionD(int *g, int *h) {
  int i = *g;
  *g = *h;
  *h = i;
  return 0;
}

int functionC(int *e, int f, int r) {
  e[f] > e[f + 1] && functionD(e + f, e + f + 1);
  int ret = f < r - 2 && functionC(e, f + 1, r);
  return ret;
}

int functionB(int *a, int l, int r) {
  l < r && printf("%d ", a[l]);
  int ret = l < r - 1 && functionB(a, l + 1, r);
  return ret;
}

int functionA(int *a, int l, int r) {
  functionC(a, l, r);
  int ret = l < r - 2 && functionA(a, l, r - 1);
  return ret;
}

int main(void) {
  int a[] = {3, 2, 5, 1, 4, 6, 9, 8, 7};
  int b = sizeof(a) / sizeof(a[0]);
  functionA(a, 0, b);
  functionB(a, 0, b);
  printf("\n");
  return 0;
}