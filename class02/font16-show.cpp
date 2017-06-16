#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include "font16.h"

using namespace std;

int each_line = 16;
int each_word = 16;
int all_word = 4;

string to_sbit(int a) {
  stringstream ss;
  for (int i = 0; i < each_word; i++) {
    if (a % 2 == 0)
      ss << '0';
    else if (a % 2 == 1)
      ss << '1';
    a /= 2;
  }
  string s = ss.str();
  reverse(s.begin(), s.end());
  return s;
}

void cout_word_line(string s) {
  for (int i = 0; i < each_word * each_line; i++) {
    if (s[i] == '0')
      cout << " ";
    else if (s[i] == '1')
      cout << "o";
  }
}

int main() {
  int line;
  if (all_word % each_line == 0)
    line = all_word / each_line;
  else
    line = all_word / each_line + 1;
  for (int i = 0; i < line; i++) {
    for (int k = 0; k < each_word; k++) {
      for (int j = 0; j < each_line; j++) {
	if (j + i * each_line >= all_word) break;
	cout_word_line(to_sbit(Font[j + i * each_line][k]));
      }
      cout << endl;
    }
  }
}
