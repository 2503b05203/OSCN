#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

int calculateRedundantBits(int m) {
    int r = 0;
    while (pow(2, r) < (m + r + 1)) {
        r++;
    }
    return r;
}

int calculateParityBit(const vector<int>& code, int n, int parityBitPosition) {
    int parityCount = 0;
    for (int i = parityBitPosition; i <= n; i++) {
        if ((i & parityBitPosition) != 0) {
            if (i != parityBitPosition) {
                if (code[i] == 1) {
                    parityCount++;
                }
            }
        }
    }
    
    return (parityCount % 2 == 0) ? 0 : 1;
}

vector<int> generateHammingCode(string data, int m) {
    int r = calculateRedundantBits(m);
    int n = m + r; 

    vector<int> code(n + 1, 0);

    int dataBitIndex = m - 1; 
    for (int i = n; i >= 1; i--) {
        if ((i & (i - 1)) == 0) {
        } else {
            if (dataBitIndex >= 0) {
                code[i] = data[dataBitIndex] - '0'; 
                dataBitIndex--;
            }
        }
    }
    
    for (int i = 0; i < r; i++) {
        int parityBitPosition = pow(2, i);
        code[parityBitPosition] = calculateParityBit(code, n, parityBitPosition);
    }

    return code;
}

void receiveHammingCode(vector<int> receivedCode) {
    int n = receivedCode.size() - 1; 
    int r = 0;
    
    while(pow(2, r) <= n) {
        r++;
    }
    int temp_m = 0;
    int temp_r = 0;
    while (temp_m + temp_r < n) {
        if ((temp_m + temp_r + 1) == pow(2, temp_r)) {
            temp_r++;
        }
        temp_m++;
    }
    r = temp_r;

    vector<int> recalculatedParity(r);
    int errorPosition = 0;

    for (int i = 0; i < r; i++) {
        int parityBitPosition = pow(2, i);
        
        int parityCount = 0;
        for (int j = parityBitPosition; j <= n; j++) {
            if ((j & parityBitPosition) != 0) {
                if (receivedCode[j] == 1) {
                    parityCount++;
                }
            }
        }
        
        recalculatedParity[i] = (parityCount % 2 == 0) ? 0 : 1;
    }

    for (int i = 0; i < r; i++) {
        if (recalculatedParity[i] == 1) {
            errorPosition += pow(2, i);
        }
    }

    if (errorPosition == 0) {
        cout << "Correct data packet received" << endl;
    } else {
        cout << "Error detected at bit position " << errorPosition << "." << endl;
    }
}


int main() {
    string data;
    cout << "Enter the binary data string (e.g., 1011001): ";
    cin >> data;
    string original_data = data;
    
    cout << "Input data: " << original_data << endl;

    int m = data.length();
    reverse(data.begin(), data.end()); 
    int r = calculateRedundantBits(m);
    int n = m + r;

    vector<int> codeToSend = generateHammingCode(data, m);
    
    cout << "The data packet to be sent is: ";
    for (int i = 1; i <= n; i++) {
        cout << codeToSend[i] << " ";
    }
    cout << "\n" << endl;

    
    cout << "Simulation 1 (No Error):" << endl;
    receiveHammingCode(codeToSend);

    cout << "\nSimulation 2 (With Error):" << endl;
    vector<int> receivedCodeWithError = codeToSend;
    int errorBit = 3;
    if (errorBit > 0 && errorBit <= n) {
        cout << "(Injecting single-bit error at position " << errorBit << ")" << endl;
        receivedCodeWithError[errorBit] = (receivedCodeWithError[errorBit] == 0) ? 1 : 0;
        receiveHammingCode(receivedCodeWithError);
    } else {
        cout << "(Data packet too short to inject error at bit " << errorBit << ")" << endl;
    }

    return 0;
}