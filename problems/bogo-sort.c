#include <stdio.h>

int x = 0;
int functionE() {
  return x = (x * 89 + 64) % 24128999;
}
void functionD(int *g, int *h) {
  int i = *g;
  *g = *h;
  *h = i;
}
void functionC(int *e, int f) {
  for (int i = 0; i < f; i++) {
    functionD(e + i, e + (functionE() % f));
  }
}
int functionB(int *c, int d) {
  while (--d >= 1) {
    if (c[d] < c[d - 1])
      return 0;
  }
  return 1;
}
void functionA(int *a, int b) {
  while (!functionB(a, b)) {
    functionC(a, b);
  }
}

int main() {
  int a[] = {3, 2, 5, 1, 4, 6, 9, 8, 7};
  int b = sizeof(a) / sizeof(a[0]);
  functionA(a, b);
  for (int i = 0; i < b; i++) {
    printf("%d ", a[i]);
  }
  printf("\n");
  return 0;
}
