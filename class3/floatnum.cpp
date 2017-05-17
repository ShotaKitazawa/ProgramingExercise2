#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <math.h>
#include <sstream>
#include <algorithm>

using namespace std;
const int Fraction = 24;
const int Exponent = 8;

string int_binary(int b, int& count) {
  stringstream ss;
  for (int i = 0; b > 0; i++) {
    ss << (b % 2);
    b = b / 2;
    count++;
  }
  string s = ss.str();
  std::reverse(s.begin(), s.end());
  return s;
}

string dec_binary(float b, int& count) {
  stringstream ss;
  int tmp;
  for (int i = 0; i < Fraction - count; i++) {
    tmp = b * 2;
    if (tmp % 2 == 0)
      ss << "0";
    else
      ss << tmp;
    b = b * 2;
    if (b > 1) b -= 1;
  }
  return ss.str();
}

int main() {
  char sign_buf;
  char exponent_buf[Exponent];
  char fraction_buf[Fraction];
  // float n = 3.1415926;
  float n;
  cout << "小数を入力\n> ";
  cin >> n;
  if (n < 0) {
    sign_buf = '1';
    n *= -1;
  } else
    sign_buf = '0';
  int intn = n / 1;
  float decn = n - intn;
  int count = 0;
  char* fp = fraction_buf;
  char* ep = exponent_buf;
  const char* ip = int_binary(intn, count).c_str();
  const char* dp = dec_binary(decn, count).c_str();
  while (*ip != '\0') {
    *fp = *ip;
    fp++;
    ip++;
  }
  *fp = '.';
  fp++;
  while (*dp != '\0') {
    *fp = *dp;
    fp++;
    dp++;
  }
  *fp = '\0';
  cout << "2進数表記\n";
      if (sign_buf == '1') cout << "'-1"; 
  cout << fraction_buf << endl;
  fp = fraction_buf;
  count = 0;
  char tmp;
  while (*fp != '.') {
    fp++;
    count++;
  }
  for (int i = 0; i < count - 1; i++) {
    fp--;
    tmp = *fp;
    fp++;
    *fp = tmp;
    fp--;
  }
  *fp = '.';
  fp++;
  int digits = 0;
  const char* tmpp = int_binary(count - 1, digits).c_str();
  for (int i = 0; i < Exponent - digits + 127; i++) {
    *ep = '0';
    ep++;
  }
  for (int i = 0; i < digits; i++) {
    *ep = *tmpp;
    ep++;
    tmpp++;
  }
  *ep = '\0';
  cout << "浮動小数点表記:\n " << sign_buf << exponent_buf << fp << endl;
}
