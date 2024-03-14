#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <Windows.h>

// Генерація таблиці
std::vector<std::vector<char>> generate_table(const std::string& alphabet, int m, int n) {
    std::vector<std::vector<char>> table(m, std::vector<char>(n, '\0'));
    std::string shuffled_alphabet = alphabet;
    std::shuffle(shuffled_alphabet.begin(), shuffled_alphabet.end(), std::mt19937(std::random_device()()));
    auto iter = shuffled_alphabet.begin();
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            table[i][j] = *iter++;
        }
    }
    return table;
}

// Шифрування
std::pair<std::string, bool> encrypt(const std::string& text, const std::vector<std::vector<char>>& table) {
    std::string encrypted_text;
    bool success = true;
    for (size_t i = 0; i < text.size(); i += 2) {
        char first_letter = text[i];
        char second_letter = text[i + 1];
        int first_letter_i = -1, first_letter_j = -1, second_letter_i = -1, second_letter_j = -1;
        for (size_t row_index = 0; row_index < table.size(); ++row_index) {
            const std::vector<char>& row = table[row_index];
            auto it1 = std::find(row.begin(), row.end(), first_letter);
            auto it2 = std::find(row.begin(), row.end(), second_letter);
            if (it1 != row.end()) {
                first_letter_i = row_index;
                first_letter_j = std::distance(row.begin(), it1);
            }
            if (it2 != row.end()) {
                second_letter_i = row_index;
                second_letter_j = std::distance(row.begin(), it2);
            }
        }
        if (first_letter_i == -1 || first_letter_j == -1 || second_letter_i == -1 || second_letter_j == -1) {
            success = false;
            break;
        }
        if (first_letter_i == second_letter_i || first_letter == second_letter) {
            first_letter_j = (first_letter_j + 1) % table[0].size();
            second_letter_j = (second_letter_j + 1) % table[0].size();
        }
        else if (first_letter_j == second_letter_j) {
            first_letter_i = (first_letter_i + 1) % table.size();
            second_letter_i = (second_letter_i + 1) % table.size();
        }
        else {
            std::swap(first_letter_i, second_letter_i);
        }
        encrypted_text += table[first_letter_i][first_letter_j];
        encrypted_text += table[second_letter_i][second_letter_j];
    }
    return std::make_pair(encrypted_text, success);
}

// Дешифрування
std::pair<std::string, bool> decrypt(const std::string& text, const std::vector<std::vector<char>>& table) {
    std::string decrypted_text;
    bool success = true;
    for (size_t i = 0; i < text.size(); i += 2) {
        char first_letter = text[i];
        char second_letter = text[i + 1];
        int first_letter_i = -1, first_letter_j = -1, second_letter_i = -1, second_letter_j = -1;
        for (size_t row_index = 0; row_index < table.size(); ++row_index) {
            const std::vector<char>& row = table[row_index];
            auto it1 = std::find(row.begin(), row.end(), first_letter);
            auto it2 = std::find(row.begin(), row.end(), second_letter);
            if (it1 != row.end()) {
                first_letter_i = row_index;
                first_letter_j = std::distance(row.begin(), it1);
            }
            if (it2 != row.end()) {
                second_letter_i = row_index;
                second_letter_j = std::distance(row.begin(), it2);
            }
        }
        if (first_letter_i == -1 || first_letter_j == -1 || second_letter_i == -1 || second_letter_j == -1) {
            success = false;
            break;
        }
        if (first_letter_i == second_letter_i || first_letter == second_letter) {
            first_letter_j = (first_letter_j == 0) ? table[0].size() - 1 : first_letter_j - 1;
            second_letter_j = (second_letter_j == 0) ? table[0].size() - 1 : second_letter_j - 1;
        }
        else if (first_letter_j == second_letter_j) {
            first_letter_i = (first_letter_i == 0) ? table.size() - 1 : first_letter_i - 1;
            second_letter_i = (second_letter_i == 0) ? table.size() - 1 : second_letter_i - 1;
        }
        else {
            std::swap(first_letter_i, second_letter_i);
        }
        decrypted_text += table[first_letter_i][first_letter_j];
        decrypted_text += table[second_letter_i][second_letter_j];
    }
    return std::make_pair(decrypted_text, success);
}

// Читання з файлу
std::string read_input_file(const std::string& file_name) {
    std::ifstream file(file_name);
    std::string text;
    char ch;
    while (file.get(ch)) {
        text.push_back(ch);
    }
    return text;
}

// Запис в файл
void write_output_file(const std::string& file_name, const std::string& text) {
    std::ofstream file(file_name);
    file << text;
}

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    std::string encrypted_file_name = "encrypted_text.txt";
    std::string decrypted_file_name = "decrypted_text.txt";
    std::string uk_alphabet = "абвгґдеєжзиіїйклмнопрстуфхцчшщьюя";
    std::vector<std::vector<char>> table = generate_table(uk_alphabet, 3, 11);

    std::string original_text;
    std::cout << "Введіть текст, який будемо шифрувати: ";
    std::getline(std::cin, original_text);
    std::transform(original_text.begin(), original_text.end(), original_text.begin(), ::tolower);

    auto [encrypted_text, encryption_success] = encrypt(original_text, table);

    std::cout << "\nКлюч-таблиця для реалізації шифру Плейфера:\n";
    for (const auto& row : table) {
        for (char letter : row) {
            std::cout << letter << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "\nОригінальний текст: " << original_text << std::endl;
    if (encryption_success) {
        write_output_file(encrypted_file_name, encrypted_text);
        std::cout << "Зашифрований текст: " << encrypted_text << std::endl;

        auto [decrypted_text, decryption_success] = decrypt(encrypted_text, table);

        if (decryption_success) {
            write_output_file(decrypted_file_name, decrypted_text);
            std::cout << "Дешифрований текст: " << decrypted_text << std::endl;
        }
        else {
            std::cout << "Дешифрування неможливе.\nЗашифрований текст містить не тільки літери українського алфавіту" << std::endl;
        }
    }
    else {
        std::cout << "Допускається ввід лише літер українського алфавіту" << std::endl;
    }
    return 0;
}