#include <iostream>
#include <string>
#include <algorithm>

// Function to perform bitwise XOR between two equal-length segments
std::string xor_strings(const std::string& a, const std::string& b) {
    std::string result = "";
    // XOR starts from the second bit (index 1) of the segment
    for (size_t i = 1; i < a.length(); ++i) {
        result += (a[i] == b[i] ? '0' : '1');
    }
    return result;
}

// Function to calculate the CRC remainder (FCS). Works for both sender and receiver.
std::string calculate_remainder(std::string dividend, const std::string& generator) {
    int k = generator.length() - 1; 
    int g_len = generator.length();

    // The division process: iterate over the dividend
    for (int i = 0; i < dividend.length() - k; ++i) {
        if (dividend[i] == '1') {
            std::string segment = dividend.substr(i, g_len);
            std::string xor_result = xor_strings(segment, generator);
            
            // Update the dividend with the XOR result (starting from i+1)
            for (int j = 0; j < xor_result.length(); ++j) {
                dividend[i + 1 + j] = xor_result[j];
            }
        }
    }

    // The remainder is the last k bits of the final dividend
    return dividend.substr(dividend.length() - k);
}

// --- Main Program ---
int main() {
    
    const std::string data_stream = "10011101";
    
    const std::string generator_polynomial = "1001";
    int k = generator_polynomial.length() - 1; // k=3 CRC bits

    std::cout << "--- CRC Sender (Encoder) ---" << std::endl;
    std::cout << "Data Stream: " << data_stream << std::endl;
    std::cout << "Generator (G): " << generator_polynomial << " (k=" << k << ")" << std::endl;
    
    std::string data_with_zeros = data_stream + std::string(k, '0');
    std::string fcs = calculate_remainder(data_with_zeros, generator_polynomial);
    
    std::string transmitted_string = data_stream + fcs;

    std::cout << "Calculated Remainder (FCS): " << fcs << std::endl;
    std::cout << "Actual Bit String Transmitted: " << transmitted_string << std::endl;
    std::cout << std::string(40, '=') << std::endl;


    std::string received_string = transmitted_string; 
    received_string[2] = (received_string[2] == '0' ? '1' : '0'); 
    
    std::cout << "--- CRC Receiver (Decoder) ---" << std::endl;
    std::cout << "Simulated Error: Third bit flipped." << std::endl;
    std::cout << "Received String: " << received_string << std::endl;

    std::string receiver_remainder = calculate_remainder(received_string, generator_polynomial);

    std::cout << "Receiver Remainder: " << receiver_remainder << std::endl;

    std::string expected_remainder(k, '0');
    if (receiver_remainder == expected_remainder) {
        std::cout << "RESULT: ✅ Data integrity check PASSED (Remainder is 0). No error detected." << std::endl;
    } else {
        std::cout << "RESULT: ❌ Data integrity check FAILED (Remainder is not 0). Error detected!" << std::endl;
    }
    
    std::cout << std::string(40, '=') << std::endl;
    
    return 0;
}

