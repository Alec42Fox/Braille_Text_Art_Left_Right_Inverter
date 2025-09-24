#include <iostream>
#include <string>
#include <iomanip>
#include <unordered_map>
#include <locale>
#include <codecvt>
#include <vector>
#include <algorithm>

using std::cout;
using std::endl;

// Function to mirror a Braille character left-to-right
char32_t mirrorBraille(char32_t c) {
    if (c < 0x2800 || c > 0x28FF) return c; // not Braille
    unsigned char v = static_cast<unsigned char>(c - 0x2800);
    unsigned char m = 0;

    // mapping dot positions (1↔4, 2↔5, 3↔6, 7↔8)
    if (v & (1 << 0)) m |= (1 << 3); // dot1 -> dot4
    if (v & (1 << 1)) m |= (1 << 4); // dot2 -> dot5
    if (v & (1 << 2)) m |= (1 << 5); // dot3 -> dot6
    if (v & (1 << 6)) m |= (1 << 7); // dot7 -> dot8

    if (v & (1 << 3)) m |= (1 << 0); // dot4 -> dot1
    if (v & (1 << 4)) m |= (1 << 1); // dot5 -> dot2
    if (v & (1 << 5)) m |= (1 << 2); // dot6 -> dot3
    if (v & (1 << 7)) m |= (1 << 6); // dot8 -> dot7

    return 0x2800 + m;
}

// Convert UTF-8 string to UTF-32
std::u32string f_utf8_to_utf32(const std::string &input) {
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
    return conv.from_bytes(input);
}

// Convert UTF-32 string back to UTF-8
std::string f_utf32_to_utf8(const std::u32string &input) {
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
    return conv.to_bytes(input);
}


int main() {
    std::cout << "Paste or type your Braille ASCII art (enter an empty line when done):\n";

    std::vector<std::string> vector_string_lines;
    std::string string_line;

    // Step 1: collect all lines
    while (true) {
        std::getline(std::cin, string_line);
        if (string_line.empty()) break; // empty line signals end of input
        vector_string_lines.push_back(string_line);
    }

    // Step 2: confirmation before processing
    std::cout << "\nInput collected. Press Enter to mirror the ASCII art...\n";
    std::getline(std::cin, string_line); // wait for Enter

    // Step 3: process all lines
    std::cout << "\nMirrored ASCII art:\n";
    for (const auto& l : vector_string_lines) {
        std::u32string u32string_line = f_utf8_to_utf32(l);
        std::reverse(u32string_line.begin(), u32string_line.end());
        for (auto &ch : u32string_line) ch = mirrorBraille(ch);
        std::cout << f_utf32_to_utf8(u32string_line) << "\n";
    }
    return 0;
}
