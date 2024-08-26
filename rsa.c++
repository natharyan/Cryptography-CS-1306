#include <iostream>
#include <cmath>
#include <cstdlib>
#include <string>
#include <vector>
#include <random>

using namespace std;

bool isprime(int p){
    if (p == 2) return true;
    if (p < 2 || p % 2 == 0) return false;
    for (int i = 3; i <= sqrt(p); i += 2) {
        if (p % i == 0) return false;
    }
    return true;
}

int genprime(){
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(2, 999);

    int p;
    do {
        p = dis(gen);
    } while (!isprime(p));

    return p;
}

int calcphi(int p, int q){
    return (p-1)*(q-1);
}

int gcd(int a, int b){
    if (b == 0) return a;
    return gcd(b, a % b);
}

int gencoprime(int phin) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(2, phin - 1);

    int e;
    do {
        e = dis(gen);
    } while (gcd(e, phin) != 1);

    return e;
}

int extendedeuclideangcd(int a, int b, int* x, int* y)
{
    if (a == 0) {
        *x = 0, *y = 1;
        return b;
    }
    int x1, y1;
    int gcd = extendedeuclideangcd(b % a, a, &x1, &y1);

    *x = y1 - (b / a) * x1;
    *y = x1;

    return gcd;
}

int modinv(int A, int M)
{
    int x, y;
    int g = extendedeuclideangcd(A, M, &x, &y);
    if (g != 1)
        return -1;
    else {
        // m is added to handle negative x
        int res = (x % M + M) % M;
        return res;
    }
}

int modexp(int a, int b, int n){
    long long res = 1;
    long long x = a % n;
    while (b > 0) {
        if (b % 2 == 1) {
            res = (res * x) % n;
        }
        x = (x * x) % n;
        b /= 2;
    }
    return (int)res;
}


int main(){
    int p = genprime(), q = genprime();
    int phin = calcphi(p,q);
    int e = gencoprime(phin);
    int d = modinv(e,phin);
    int m = 97;
    cout << "RSA" << endl;
    cout << "with integers:" << endl;
    cout << "m: " << m << endl;
    cout << "p: " << p << ", q: " << q << endl;
    cout << "public key: ";
    cout << "e: " << e << ", " << "n: " << p*q << endl;
    int c = modexp(m,e,p*q);
    cout << "c: " << c << endl;
    int m2 = modexp(c,d,p*q);
    cout << "decrypted: " << m2 << endl << endl;

    cout << "--------------------------------" << endl << endl;
    cout << "with characters:" << endl;
    string m1 = "attack at dawn";
    cout << "m: " << m1 << endl;
    vector<int> c1;
    for (int i = 0; i < m1.length(); i++){
        int c = modexp(int(m1[i]),e,p*q);
        c1.push_back(c);
    }
    cout << "c: " ;
    for(int v : c1){
        cout << v << " ";
    }
    cout << endl;
    string m3 = "";
    for (int i = 0; i < c1.size(); i++){
        int m2 = modexp(c1[i],d,p*q);
        m3 += char(m2);
    }
    cout << "decrypted: " << m3 << endl;
    return 0;
}
