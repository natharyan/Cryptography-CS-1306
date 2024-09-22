#include <iostream>

using namespace std;

int main(){
    string ciphertext = "EVIRE";

    for(int i = 0; i < 26; i++){
        string plaintext = "";
        for(char c: ciphertext){
            plaintext += char((int(c) - 65 + i)%26 + 65);
        }
        cout << "(" << i << ")" << " " << plaintext << endl;
    }

    return 0;
}