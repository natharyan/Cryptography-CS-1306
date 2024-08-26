#include <iostream>
#include <cmath>
#include <cstdlib>
#include <random>
#include <string>

using namespace std;

bool isprime(int n)
{
    if (n <= 1)
        return false;
    if (n <= 3)
        return true;
    if (n % 2 == 0 || n % 3 == 0)
        return false;
    for (int i = 5; i * i <= n; i = i + 6)
    {
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
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

int primitiveroot(int p){
    vector<int> fact;
    int phi = p-1,  n = phi;
    for (int i=2; i*i<=n; ++i)
        if (n % i == 0) {
            fact.push_back (i);
            while (n % i == 0)
                n /= i;
        }
    if (n > 1)
        fact.push_back (n);

    for (int res=2; res<=p; ++res) {
        bool ok = true;
        for (size_t i=0; i<fact.size() && ok; ++i)
            ok &= modexp(res, phi / fact[i], p) != 1;
        if (ok)  return res;
    }
    return -1;
}

class Alice{
    int p, g, x, A;
    public:
    Alice(){
        this->p = genprime();
        this->g = primitiveroot(p);
        this->x = (rand() % (p - 1)) + 1;
        this->A = modexp(g, x, p);
    }
    int getA(){
        return A;
    }
    int getp(){
        return p;
    }
    int getg(){
        return g;
    }
    int geta(){
        return x;
    }
    int getsharedkey(int B){
        return modexp(B, x, p);
    }
};

class person{
    int p, g, b, B;
    public:
    person(int A,int p,int g){
        this->p = p;
        this->b = (rand() % (p - 1)) + 1;
        this->g = g;
        this->B = modexp(g, b, p);
    }
    int getB(){
        return B;
    }
    int getb(){
        return b;
    }
    int getsharedkey(int A){
        return modexp(A, b, p);
    }
};

int encrypt(int m, int k,int p){
    return (m*k) % p;
}

int main(){
    Alice alice;
    int p = alice.getp();
    int g = alice.getg();
    int A = alice.getA();
    person bob(A, p, g);
    person alaska(A, p, g);
    cout << "Alice's public key: " << endl << "A: " << A << " g: " << g << " p: " << p << endl << endl;
    cout << "Alice and Bob key: " << endl;
    cout << "Bob generates key: " << bob.getsharedkey(A) << endl;
    cout << "Alice generates key: " << alice.getsharedkey(bob.getB()) << endl;
    cout << "Bob sends Alice: ";
    cout << endl;
    int m1int = 97;
    string m1 = "user: Bob, password: 1234";
    vector<int> c1;
    cout << "string: ";
    for (int i = 0; i < m1.size(); i++){
        int m = encrypt(m1[i], bob.getsharedkey(A), p);
        c1.push_back(m);
        cout << char(m) << " ";
    }
    cout << endl;
    int encm1int = encrypt(m1int, bob.getsharedkey(A), p);
    cout << m1int << " encrypted as " << "integer: " << encm1int << endl;
    int ink1 = modinv(bob.getsharedkey(A),p);
    string m1d = "";
    for (int i = 0; i < c1.size(); i++){
        m1d += char(encrypt(c1[i], ink1, p));
    }
    cout << "keys: " << "k: " << bob.getsharedkey(A) << " k-1: " << ink1 << " p: " << p << endl;
    cout << "Alice decrypts: " << endl;
    cout << "string: " << m1d << endl;
    cout << "integer: " << encrypt(encm1int, ink1, p) << endl << endl;
    cout << "Alice and Alaska key: " << endl;
    cout << "Alaska generates key: " << alaska.getsharedkey(A) << endl;
    cout << "Alice generates key: " << alice.getsharedkey(alaska.getB()) << endl;
    cout << "Alaska sends Alice: " << endl;
    int m2int = 97;
    string m2 = "user: Alaska, password: 123456";
    vector<int> c2;
    cout << "string: ";
    for (int i = 0; i < m2.size(); i++){
        int m = encrypt(m2[i], alaska.getsharedkey(A), p);
        c2.push_back(m);
        cout << char(m) << " ";
    }
    cout << endl;
    int encm2int = encrypt(m2int, alaska.getsharedkey(A), p);
    cout << m2int << " encrypted as " << "integer: " << encm2int << endl;
    int ink2 = modinv(alaska.getsharedkey(A),p);
    string m2d = "";
    for (int i = 0; i < c2.size(); i++){
        m2d += char(encrypt(c2[i], ink2, p));
    }
    cout << "keys: " << "k: " << alaska.getsharedkey(A) << " k-1: " << ink2 << " p: " << p << endl;
    cout << "Alice decrypts: " << endl;
    cout << "string: " << m2d << endl;
    cout << "integer: " << encrypt(encm2int, ink2, p) << endl;
    return 0;
}
