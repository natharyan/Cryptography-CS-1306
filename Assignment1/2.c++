#include <iostream>
#include <vector>

using namespace std;

int main(){
    string ciphertext = "UCR";
    vector<int> cipher;

    for (char c: ciphertext)
    {
        int cipherint = int(c) - 65;
        cout << cipherint << " ";
        cipher.push_back(cipherint);
    }

    cout << endl;

    for(int i = 0; i < cipher.size(); i++){
        cipher[i] = (cipher[i] - 2)%26;
        cout << cipher[i] << " ";
    }

    // 18 0 15
    // x = 2, 0, 19

    string plaintext = "";
    plaintext += char(2 + 65);
    plaintext += char(0 + 65);
    plaintext += char(19 + 65);

    cout << endl;

    cout << plaintext;
    
}