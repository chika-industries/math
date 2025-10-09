// 10-K --regex--> AI analysis

#include <iostream>
#include <fstream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

int main() {    
    std::string filename = "KO2024_FULL.txt";
    std::string keyword;

    // Ask user for keyword - generic or specific
    // Coke: unit case volume
    std::cout << "Enter keyword to search for (e.g., dividend, buyback, repurchase, unit case volume): ";
    std::getline(std::cin, keyword);

    // Step 1. Read entire text
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Could not open file: " << filename << std::endl;
        return 1;
    }
    std::ostringstream ss;
    ss << file.rdbuf();
    std::string text = ss.str();

    // Step 2. Split into sentences
    std::regex sentenceRegex(R"(([^.!?]*[.!?]))");
    std::sregex_iterator begin(text.begin(), text.end(), sentenceRegex), end;
    std::vector<std::string> sentences;
    for (auto it = begin; it != end; ++it) {
        std::string s = it->str();
        s.erase(0, s.find_first_not_of(" \n\r\t"));
        s.erase(s.find_last_not_of(" \n\r\t") + 1);
        if (!s.empty()) sentences.push_back(s);
    }

    // Step 3. Create dynamic regex for keyword
    std::regex keywordRegex("\\b" + keyword + "\\b", std::regex_constants::icase);
    int snippetCount = 0;

    for (size_t i = 0; i < sentences.size(); ++i) {
        if (std::regex_search(sentences[i], keywordRegex)) {
            std::cout << "\n--- " << keyword << " Snippet " << ++snippetCount << " ---\n";

            if (i > 0) std::cout << sentences[i - 1] << " ";
            std::cout << sentences[i] << " ";
            if (i + 1 < sentences.size()) std::cout << sentences[i + 1];
            std::cout << "\n";
        }
    }

    if (snippetCount == 0)
        std::cout << "No occurrences of '" << keyword << "' found.\n";
}
