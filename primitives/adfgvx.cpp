#include <iostream>
#include <string>

using namespace std;

int main(){
    // using matrices - same complexity as using linked list for this
    string input;
    cout << "Enter message: ";
    cin >> input;
    string key = "ADFGVX";
    int key_size = key.size();
    string adfgvx[6][6];
    int row = 0;
    int col = 0;
    for(int i = 0; i < input.size(); i++){
        if(col == 6){
            col = 0;
            row++;
        }else{
            adfgvx[row][col] = input[i];
            col++;
        }
    }
    if(input.size() < 36){
        for(int i = input.size(); i < 36; i++){
            if(col == 6){
            col = 0;
            row++;
            }else{
                adfgvx[row][col] = to_string(i - input.size());
                col++;
            }
        }
    }

    // print matrix
    for(int i = 0; i < 6; i++){
        for(int j = 0; j < 6; j++){
            cout << adfgvx[i][j] << " ";
        }
        cout << endl;
    }

    string cipher = "";
    row = 0;
    col = 0;
    for(int i = 0; i < input.size(); i ++ ){
        if(col == 6){
            col = 0;
            row++;
        }else{
            cipher = cipher + key[row] + key[col];
            col++;
        }
    }

    cout << "Ciphertext: " << cipher;

    return 0;
}