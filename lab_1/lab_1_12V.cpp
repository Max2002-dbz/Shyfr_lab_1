
#include <iostream>
#include <Windows.h>
#include <fstream>
#include <sstream>

const std::string ukr = "אבגדהוו÷זחט³¿יךכלםמןנסעףפץצקרשü‏ÿÀÁÂÃÄÅ×ÆÇÈ²¯ÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÜÞ‗ ";
const std::string eng = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ,.!?-:;()[]{}\"'`<>|/@#$%^&*~+=_ ";


const enum language
{
	uk, en
};

std::string Encrypt(std::string_view input, std::uint8_t n, std::string_view lang)
{
	std::string output;

	size_t size_input = input.size();
	size_t size_lang = lang.size();

	if (size_input == 0)
	{
		return "";
	}

	output.resize(size_input);

	for (size_t i = 0; i < size_input; i++)
	{
		size_t position = lang.find(input[i]);
		output[i] += lang[(position + n) % size_lang];
	}

	return output;
}

std::string Decrypt(std::string_view input, std::uint8_t n, std::string_view lang)
{
	std::string output;

	size_t size_input = input.size();
	size_t size_lang = lang.size();

	if (size_input == 0)
	{
		return "";
	}

	output.resize(size_input);

	for (size_t i = 0; i < size_input; i++)
	{
		int position = lang.find(input[i]);
		output[i] += lang[position - n < 0 ? size_lang + (position - n) : position - n];
	}

	return output;
}

std::string ReadFromFile(std::string_view filename)
{
	std::ifstream inputFile(filename.data());

	if (!inputFile.is_open()) {
		std::cerr << "Íו גהאכמסÿ ג³הךנטעט פאיכ " << filename << std::endl;
		return "";
	}

	std::stringstream buffer;
	buffer << inputFile.rdbuf();

	return buffer.str();
}

int main()
{
	SetConsoleOutputCP(1251);

	std::string filename_ukr = "ukr_text.txt";
	std::string filename_eng = "eng_text.txt";

	size_t n = 30;

	// ukr
	std::string text = ReadFromFile("ukr_text.txt");
	std::cout << " Ç פאיכף ukr_text.txt:\n";
	if (!text.empty())
	{
		std::string result;
		std::cout << (result = Encrypt(text, n, ukr)) << std::endl;
		std::cout << Decrypt(result, n, ukr) << std::endl;
	}
	else
	{
		std::cout << " Ôאיכ ןףסעטי\n";
	}

	std::cout << "\n";

	// eng
	text = ReadFromFile("eng_text.txt");
	std::cout << " Ç פאיכף eng_text.txt:\n";
	if (!text.empty())
	{
		std::string result;
		std::cout << (result = Encrypt(text, n, eng)) << std::endl;
		std::cout << Decrypt(result, n, eng) << std::endl;
	}
	else
	{
		std::cout << " Ôאיכ ןףסעטי\n";
	}

	std::cout << "\n";

}