#include <stdio.h>
#include <string.h>

#define ML 100005

char x[ML];
int t;

int funcB(char x, char y) {
  return (x == 40 && y == 41) ||
         (x == 91 && y == 93) ||
         (x == 123 && y == 125);
}

int funcA(const char* str) {
  t = 0;
  for (int i = 0; str[i]; ++i) {
    char c = str[i];
    if (c == 40 || c == 91 || c == 123) {
      x[t++] = c;
    } else if (c == 41 || c == 93 || c == 125) {
      if (t == 0 || !funcB(x[t - 1], c)) {
        return 0;
      }
      --t;
    }
  }
  return t == 0;
}

int main() {
  char s[ML];
  fgets(s, sizeof(s), stdin);
  printf("%s\n", funcA(s) ? "YES" : "NO");
}
