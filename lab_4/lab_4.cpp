#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <Windows.h>

int main() {

    SetConsoleOutputCP(1251);
    
    // Визначення алфавіту (українська абетка, обидва регістри)
    std::string alphabet = "абвгдеєжзиіїйклмнопрстуфхцчшщьюяАБВГДЕЄЖЗИІЇЙКЛМНОПРСТУФХЦЧШЩЬЮЯ";
    int N = alphabet.length();

    // Визначення номерів літер
    std::unordered_map<char, int> letter_to_number;
    for (int i = 0; i < N; ++i) {
        letter_to_number[alphabet[i]] = i;
    }

    // Парольна фраза (ключ)
    std::string password = "плюси";

    // Формування гами
    std::vector<int> gamma;
    for (char letter : password) {
        gamma.push_back(letter_to_number[letter]);
    }

    // Відкритий текст
    std::string plaintext = "КафедраПрикладноїМатематикиНаціональногоУніверситетуЛьвівськаПолітехнікаВжеБагатоРоківЗберігаєІПримножуєБагатіНауковіЙОсвітніТрадиціїНевтомноГотуєНовіПоколінняКваліфікованихКадрівУГалузяхПрикладноїМатематикиТаІнформатикиЗбагачуючиЇхІнтелектуальноІДуховно";

    // Перетворення в нижній регістр і видалення розділових знаків і пробілів
    std::string filtered_plaintext;
    for (char ch : plaintext) {
        if (letter_to_number.find(ch) != letter_to_number.end()) {
            filtered_plaintext.push_back(ch);
        }
    }

    // Шифрування
    std::string ciphertext;
    for (size_t i = 0; i < filtered_plaintext.length(); ++i) {
        char plain_char = filtered_plaintext[i];
        int gamma_char = gamma[i % gamma.size()];  // "продовження" гами
        int encrypted_char_index = (letter_to_number[plain_char] + gamma_char) % N;
        char encrypted_char = alphabet[encrypted_char_index];
        ciphertext.push_back(encrypted_char);
    }

    // Запис криптограми у файл
    std::ofstream file("encrypted.txt");
    if (file.is_open()) {
        file << ciphertext;
        file.close();
    }

    std::cout << "Зашифрований текст: " << ciphertext << std::endl;

    // Розшифрування
    std::string decrypted_text;
    for (size_t i = 0; i < ciphertext.length(); ++i) {
        char encrypted_char = ciphertext[i];
        int gamma_char = gamma[i % gamma.size()];  // "продовження" гами
        int decrypted_char_index = (letter_to_number[encrypted_char] - gamma_char + N) % N;
        char decrypted_char = alphabet[decrypted_char_index];
        decrypted_text.push_back(decrypted_char);
    }

    std::cout << "Розшифрований текст: " << decrypted_text << std::endl;

    return 0;
}