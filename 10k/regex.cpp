#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <sstream>
#include <string>
#include <vector>
#include <windows.h>

struct CompanyFile {
    std::string name;
    std::string filename;
};

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

std::regex moatRegex() {
    return std::regex(
        R"(\b(competitive\s+advantage|economic\s+moat|barrier(s)?\s+to\s+entry|pricing\s+power|brand\s+(strength|recognition)|network\s+effect(s)?|cost\s+advantage|switching\s+cost(s)?)\b)",
        std::regex_constants::icase);
}

std::regex capitalAllocationRegex() {
    return std::regex(
        R"(\b(capital\s+allocation|reinvestment|return\s+on\s+capital|return\s+on\s+invest(ed|ment)\s+capital|roi(c|e)|internal\s+rate\s+of\s+return|reinvestment\s+opportunit(y|ies))\b)",
        std::regex_constants::icase);
}

std::regex leverageRegex() {
    return std::regex(
        R"(\b(leverage(d|s)?|debt\s+(to|\/)\s+equity|interest\s+coverage|net\s+debt|gearing\s+ratio|liquidity\s+risk|refinancing\s+risk|credit\s+facility|covenant(s)?)\b)",
        std::regex_constants::icase);
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    std::string keyword;
    
    // Filepath to the full annual report text

    std::vector<CompanyFile> companies = {
        {"Coca Cola",  "KO2024_FULL.txt"},
        {"Mitsui O S K", "MitsuiOSK_FULL.txt"},
        {"VISA Inc.", "2024Visa_FULL.txt"},
        {"Marubeni", "2024Marubeni_FULL.txt"},        
        {"Berkshire Hathaway", "BRK2024_FULL.txt"}
    };
    
    std::string company = companies[1].name;
    std::string filename = companies[1].filename;
    
    // Prompt user to enter a keyword to search for
    // Could be dividend, buyback, repurchase, unit case volume, net interest margin, net interest income, etc.
    // Buffet simple businesses - really 1 keyword
    // Coke: unit case volume
    // PSBC: net interest income (or margin)

    enum class Mode { Dividend, Buyback, CapitalAllocation, Leverage };

    // *** Choose what to do ***
    Mode currentMode = Mode::Leverage;

    std::regex myRegex;
    switch (currentMode) {
        case Mode::Dividend: myRegex = dividendRegex(); break;
        case Mode::Buyback: myRegex = buybackRegex(); break;
        case Mode::CapitalAllocation: myRegex = capitalAllocationRegex(); break;
        case Mode::Leverage: myRegex = leverageRegex(); break;
    }

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
    //std::regex myRegex("\\b" + keyword + "\\b", std::regex_constants::icase);
    int snippetCount = 0;

    std::cout << "\nCompany name: [" << company << "]\n";
    // Step 4: Search through sentences for the keyword and print surrounding context
    for (size_t i = 0; i < sentences.size(); ++i) {
        if (std::regex_search(sentences[i], myRegex)) {
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
