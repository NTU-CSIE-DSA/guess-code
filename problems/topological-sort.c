#include <stdio.h>
#include <stdlib.h>
#define MAX 100

typedef struct structureA {
  int ddd;
  struct structureA *nnn;
} StructureA;

typedef struct {
  StructureA *aaa;
  StructureA *bbb;
} StructureB;

StructureB a[MAX];
int v[MAX], s[MAX], t = -1;

void functionB(int x, int y) {
  StructureA *na = (StructureA *)
    malloc(sizeof(StructureA));
  na->ddd = y;
  na->nnn = NULL;
  if (a[x].aaa == NULL) {
    a[x].aaa = na;
    a[x].bbb = na;
  } else {
    a[x].bbb->nnn = na;
    a[x].bbb = na;
  }
}

void functionC(int x, int n) {
  v[x] = 1;
  StructureA *now = a[x].aaa;
  while (now != NULL) {
    if (v[x] == 0)
      functionC(x, n);
    now = now->nnn;
  }
  s[++t] = x;
}

void functionA(int n) {
  for (int i = 0; i < n; i++) 
    if (v[i] == 0) 
      functionC(i, n);
  printf("Ans: ");
  while (t != -1) 
    printf("%d ", s[t--]);
  printf("\n");
}

int main() {
  int n = 6;
  functionB(5, 2);
  functionB(5, 0);
  functionB(4, 0);
  functionB(4, 1);
  functionB(2, 3);
  functionB(3, 1);
  functionA(n);
}
