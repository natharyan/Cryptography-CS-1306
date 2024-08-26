#include <iostream>
#include <map>
#include <cmath>
#include <vector>

using namespace std;

int main(){
    //key
    string key = "CARGO";
    int key_size = key.size();
    //length of input
    string text; cout << "enter text: "; cin >> text;
    map<char, vector<char> > transpositionMatrix;
    int row = 0;
    int shift;
    cout << "character shift: ";
    cin >> shift;

    // create transposition matrix
    int i = 0;
    while(i < text.size())
    {
        transpositionMatrix[key[(row*shift + i)%key_size]].push_back(text[i]);
        if (i%key_size == key_size-1)
        {
            ++row;
        }
        ++i;
    }

    //called till here
    // fill remaining row columns
    while(i%key_size != 0){
        transpositionMatrix[key[(row*shift + i)%key_size]].push_back(char(65 + i%6));
        ++i;
        if(i%key_size == 0){
            ++row;
        }
    }
    cout << "key: " << key << endl;
    cout << "transposition matrix:" << endl;
    //print transposition matrix
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < key_size; j++)
        {
            cout << transpositionMatrix[key[j]][i] << ' ';
        }
        cout << endl;
        
    }
    
    // sorting using multimap
    multimap<char,vector<char> > transposed;
    for(auto val: transpositionMatrix){
        transposed.insert(make_pair(val.first,val.second));
    }
    string ciphertext = "";
    cout << "transposed:" << endl;
    //print transposed matrix
    for (int i = 0; i < row; i++)
    {
        for (multimap<char, vector<char> > ::iterator val = transposed.begin(); val != transposed.end(); val++)
        {
            cout << val->second[i] << ' ';
            ciphertext += val->second[i];
        }
        cout << endl;
    }

    cout << "ciphertext: " << ciphertext << endl;
}