#include <iostream>
#include <bitset>
#include <climits>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>


using namespace std;

void cout_bit(char c){
  char bit[4+1];
  bit[4] = '\0';
  switch(c){
    case '0':
      bit[3] = '0';
      bit[2] = '0';
      bit[1] = '0';
      bit[0] = '0';
      break;
    case '1':
      bit[3] = '1';
      bit[2] = '0';
      bit[1] = '0';
      bit[0] = '0';
      break;
    case '2':
      bit[3] = '0';
      bit[2] = '1';
      bit[1] = '0';
      bit[0] = '0';
      break;
    case '3':
      bit[3] = '1';
      bit[2] = '1';
      bit[1] = '0';
      bit[0] = '0';
      break;
    case '4':
      bit[3] = '0';
      bit[2] = '0';
      bit[1] = '1';
      bit[0] = '0';
      break;
    case '5':
      bit[3] = '1';
      bit[2] = '0';
      bit[1] = '1';
      bit[0] = '0';
      break;
    case '6':
      bit[3] = '0';
      bit[2] = '1';
      bit[1] = '1';
      bit[0] = '0';
      break;
    case '7':
      bit[3] = '1';
      bit[2] = '1';
      bit[1] = '1';
      bit[0] = '0';
      break;
    case '8':
      bit[3] = '0';
      bit[2] = '0';
      bit[1] = '0';
      bit[0] = '1';
      break;
    case '9':
      bit[3] = '1';
      bit[2] = '0';
      bit[1] = '0';
      bit[0] = '1';
      break;
    case 'a':
      bit[3] = '0';
      bit[2] = '1';
      bit[1] = '0';
      bit[0] = '1';
      break;
    case 'b':
      bit[3] = '1';
      bit[2] = '1';
      bit[1] = '0';
      bit[0] = '1';
      break;
    case 'c':
      bit[3] = '0';
      bit[2] = '0';
      bit[1] = '1';
      bit[0] = '1';
      break;
    case 'd':
      bit[3] = '1';
      bit[2] = '0';
      bit[1] = '1';
      bit[0] = '1';
      break;
    case 'e':
      bit[3] = '0';
      bit[2] = '1';
      bit[1] = '1';
      bit[0] = '1';
      break;
    case 'f':
      bit[3] = '1';
      bit[2] = '1';
      bit[1] = '1';
      bit[0] = '1';
      break;
  }
  cout<<bit;
}

template <typename T>
void print_byte_in_order(const T &a) {
  unsigned int n = 0x76543210;         // for checking endian
  char str[2+1];str[2] = '\0';
  char bit[4];
  unsigned char* tp = (unsigned char*)&a;
  if (*(unsigned char *)&n == 0x10) {  // little endian
    for (int i = 0; i < sizeof(a); i++) tp++;
    for (int i = 0; i < sizeof(a); i++) {
      tp--;
      sprintf(str, "%02x",*tp);
      cout_bit(str[0]);
      cout_bit(str[1]);
      cout << " ";
    }
  }
  else {  // big endian
    for (int i = 0; i < sizeof(a); i++) {
      tp++;
      sprintf(str, "%02x",*tp);
      cout_bit(str[0]);
      cout_bit(str[1]);
      cout << " ";
    }
  cout << endl;
  }
}

int main() {
  char c = 0xf0;                     // character c
  short s = 0xfe01;                  // short integer s
  int i = 0x76543210;                // integer i
  long int l = 0xfedcba9876543210L;  // long integer l
  float f = 3.75;     // float  f, single-precision floating-point number
  double d = -0.125;  // double d, double-precision floating-point number
  long double ld = 3.14159265358979323;  // long double ld

  cout << "\n c = 0x" << hex << (c & UCHAR_MAX) << " = ";
  print_byte_in_order(c);
  cout << "\n s = 0x" << hex << (s & USHRT_MAX) << " = ";
  print_byte_in_order(s);
  cout << "\n i = 0x" << hex << (i & UINT_MAX) << " = ";
  print_byte_in_order(i);
  cout << "\n l = 0x" << hex << (l & ULONG_MAX) << " = ";
  print_byte_in_order(l);
  cout << fixed << setprecision(9);
  cout << "\n f = " << showpos << f << " = ";
  print_byte_in_order(f);
  cout << "\n d = " << showpos << d << " = ";
  print_byte_in_order(d);
  cout << "\nld = " << showpos << ld << " = ";
  print_byte_in_order(ld);
  return 0;
}
