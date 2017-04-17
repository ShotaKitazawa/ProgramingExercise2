#include<iostream>
#include<iomanip>

using namespace std;

int main(){
        double n, a, b;
        cout << "input the number n (n < 20): "; cin >> n;
        for (double i = 0; i <= n; i++){
                cout << "n=" << setw(2) << i;
                for (double j = n; j > i; j--){
                        cout << setw(3) << " ";
                }
                for (double j = 0; j <= i; j++){
                        a = 1;
                        b = 1;
                        for (double k = j; k > 0 ; k--) a *= i - k + 1;
                        for (double k = j; k > 0; k--) b *= k;
                        cout << setw(6) << a/b;
                }
                cout << endl;
        }
}
