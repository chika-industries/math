#include <iostream>
#include <fstream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>
#include <windows.h>

std::regex dividendRegex() {
    return std::regex(
        R"(\b(dividend(s)?|dividend[-\s]?policy|dividend[-\s]?per[-\s]?share|payout[-\s]?ratio|cash[-\s]?return)\b)",
        std::regex_constants::icase);
}

std::regex buybackRegex() {
    return std::regex(
        R"(\b(buy[-\s]?back(s|ed|ing)?|repurchase(d|s|ing)?|bought\s+back|share[-\s]?repurchase|repurchase[-\s]?(program|plan|authorization))\b)",
        std::regex_constants::icase);
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    // Filepath to the full annual report text

    std::string company = "PSBC";    
    std::string filename = "psbc2024_FULL.txt";
    std::string keyword;

    // Prompt user to enter a keyword to search for
    // Could be dividend, buyback, repurchase, unit case volume, net interest margin, net interest income, etc.
    // Buffet simple businesses - really 1 keyword
    // Coke: unit case volume
    // PSBC: net interest income (or margin)

    /* Choose what to do*/    
    std::regex keywordRegex = dividendRegex();  // 1. Dividend mode
    //std::regex keywordRegex = buybackRegex(); // 2. Buyback mode   



    //std::regex keywordRegex("\\b" + keyword + "\\b", std::regex_constants::icase);

    //std::cout << "Enter keyword regex to search for, e.g.: \n \
    //    dividend, buyback, repurchase, unit case volume, net interest margin, net interest income \n \
    //    buyback | buy back | repurchase | repurchased | repurchasing | bought back | buybacks \n \
    //    dividend | dividends): ";
    //std::getline(std::cin, keyword);

    // Step 1. Read entire text
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Could not open file: " << filename << std::endl;
        return 1;
    }

    // Use stringstream to read whole file efficiently
    std::ostringstream ss;
    ss << file.rdbuf();
    std::string text = ss.str();

    // Step 2: Split the text into individual sentences
    // Regex matches sequences ending with '.', '!', or '?'
    std::regex sentenceRegex(R"(([^.!?]*[.!?]))");
    std::sregex_iterator begin(text.begin(), text.end(), sentenceRegex), end;
    std::vector<std::string> sentences;
    for (auto it = begin; it != end; ++it) {
        std::string s = it->str();

        // Trim leading whitespace/newlines
        s.erase(0, s.find_first_not_of(" \n\r\t"));

        // Trim trailing whitespace/newlines
        s.erase(s.find_last_not_of(" \n\r\t") + 1);
        if (!s.empty()) sentences.push_back(s);
    }

    // Step 3: Build a case-insensitive regex for the user keyword
    //std::regex keywordRegex("\\b" + keyword + "\\b", std::regex_constants::icase);
    int snippetCount = 0;

    std::cout << "\nCompany name: [" << company << "]\n";
    // Step 4: Search through sentences for the keyword and print surrounding context
    for (size_t i = 0; i < sentences.size(); ++i) {
        if (std::regex_search(sentences[i], keywordRegex)) {
            std::cout << "\n--- " << keyword << " Snippet " << ++snippetCount << " ---\n";

            // Include previous sentence for context, if it exists
            if (i > 0) std::cout << sentences[i - 1] << " ";

            // Print the sentence containing the keyword
            std::cout << "\n\n < " << sentences[i] << " >\n\n ";

            // Include next sentence for context, if it exists
            if (i + 1 < sentences.size()) std::cout << sentences[i + 1];
            std::cout << "\n";
        }
    }

    // Inform user if no matches were found
    if (snippetCount == 0)
        std::cout << "No occurrences of '" << keyword << "' found.\n";
}
