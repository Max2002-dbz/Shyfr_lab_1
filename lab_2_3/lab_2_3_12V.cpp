#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <Windows.h>
#include <sstream>
#include <map>

using namespace std;

vector<vector<int>> generateTable(string alphabet) {
    vector<int> letterDeep = { 14, 7, 2, 5, 2, 1, 3, 5, 1, 1, 2, 6, 5, 1, 1, 4, 3, 3, 7, 9, 3, 5, 4, 5, 3, 1, 1, 1, 1, 1, 1, 2, 1, 2 };
    int letterAmount = 34;
    vector<vector<int>> table;

    for (int i = 0; i < letterAmount; ++i) {
        table.push_back(vector<int>(letterDeep[i], 0));
    }

    int value = 100;
    for (int i = 0; i < letterAmount; ++i) {
        for (int j = 0; j < letterDeep[i]; ++j) {
            table[i][j] = value++;
        }
    }

    cout << "\n������� ����������� �����" << endl;
    for (int i = 0; i < letterAmount; ++i) {
        cout << "\"" << alphabet[i] << "\" ";
        for (int element : table[i]) {
            cout << element << " ";
        }
        cout << endl;
    }
    cout << endl;

    return table;
}

pair<string, bool> encrypt(string text, string alphabet, vector<vector<int>>& table) {
    string result = "";
    for (char c : text) {
        auto it = find(alphabet.begin(), alphabet.end(), c);
        if (it != alphabet.end()) {
            int charIndex = distance(alphabet.begin(), it);
            result += to_string(table[charIndex][rand() % table[charIndex].size()]);
        }
        else {
            return make_pair(result, false);
        }
    }
    return make_pair(result, true);
}

string decrypt(string encryptedText, string alphabet, vector<vector<int>>& table) {
    int index = 0;
    string result = "";
    while (index < encryptedText.length()) {
        string number = encryptedText.substr(index, 3);
        if (all_of(number.begin(), number.end(), ::isdigit)) {
            int num = stoi(number);
            for (int i = 0; i < table.size(); ++i) {
                auto it = find(table[i].begin(), table[i].end(), num);
                if (it != table[i].end()) {
                    result += alphabet[i];
                }
            }
        }
        else {
            result += number;
        }
        index += 3;
    }
    return result;
}

void createFrequencyTables(string alphabet, vector<vector<int>>& table, string inputText, string encryptedText) {
    cout << "\n������� �� �������� ������� � ��������� �����" << endl;
    for (char letter : alphabet) {
        cout << "\"" << letter << "\" " << count(inputText.begin(), inputText.end(), letter) << "\n";
    }
    cout << "\n������� �� �������� ������� � ����������" << endl;
    vector<int> allSymbols;
    std::map<std::string, int> substringCount;

    for (const auto& row : table) {
        allSymbols.insert(allSymbols.end(), row.begin(), row.end());
    }
    sort(allSymbols.begin(), allSymbols.end());

    for (const auto& symbol : allSymbols) {
        substringCount[to_string(symbol)] = 0;
    }

    int chunkSize = 3;

    for (int i = 0; i < encryptedText.length(); i += chunkSize) {
        std::string substring = encryptedText.substr(i, chunkSize);
        substringCount[substring]++;
    }
    for (int symbol : allSymbols) {
        cout << "\"" << symbol << "\" " << substringCount[to_string(symbol)] << "\n";
    }
    cout << endl;
}


std::string ReadFromFile(std::string_view filename)
{
    std::ifstream inputFile(filename.data());

    if (!inputFile.is_open()) {
        std::cerr << "�� ������� ������� ���� " << filename << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << inputFile.rdbuf();

    return buffer.str();
}

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    string inputFileName = "input_ua.txt";
    string outputFileName = "output_ua.txt";

    // ����������, �� ���� ������ ���� ����� ����������� ������� �� ������.
    string uaText = ReadFromFile(inputFileName);


    transform(uaText.begin(), uaText.end(), uaText.begin(), ::tolower);

    string ukAlphabet = " ��������賿��������������������";
    vector<vector<int>> table = generateTable(ukAlphabet);

    auto [encryptedText, encryptionSuccess] = encrypt(uaText, ukAlphabet, table);

    if (encryptionSuccess) {
        string decryptedText = decrypt(encryptedText, ukAlphabet, table);

        cout << "����������� ����� � ����� ����������:" << endl;
        cout << uaText << endl;
        cout << "\n������������ �����:" << endl;
        cout << encryptedText << endl;
        cout << "\n������������ �����:" << endl;
        cout << decryptedText << endl;

        ofstream outputFile(outputFileName);
        if (outputFile.is_open()) {
            outputFile << decryptedText;
            outputFile.close();
            cout << "\n����� ������ �������� � ���� " << outputFileName << endl;
        }
        else {
            cout << "\n�� ������� ������� ���� ��� ������: " << outputFileName << endl;
        }

        cout << "\n�������� ����������� �������� ������:" << endl;

        if (uaText == decryptedText) {
            cout << "������ ����� ��������� �������� ��������!" << endl;
        }
        else {
            cout << "������ ����� �� ��������� �������� �� ��������!" << endl;
        }
        createFrequencyTables(ukAlphabet, table, uaText, encryptedText);
    }
    else {
        cout << "\n�����, � �����, ���� �� ���������� �����������, ������� ��������� �������."
            << "\n����������� ������������ ���� ���� ����������� ������� �� ������" << endl;
    }

    return 0;
}