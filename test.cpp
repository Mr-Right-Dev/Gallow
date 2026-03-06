#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <fstream>
#include <algorithm>

class Word {
	public:
		std::string word;
		std::string catergory;
		std::string wordClean;
};

std::vector<Word> words;

std::vector<std::string> splitString(std::string str, char splitChar) {
	std::vector<std::string> splittedString;

	std::string cache;
	for (char c : str) {
		if (c == splitChar) {
			// std::cout << "Found." << std::endl;
			if (cache == "") {
				continue;
			}

			splittedString.emplace_back(cache);
			cache = "";
			continue;
		}

		cache = cache + c;
	}
	if (cache != "") {
		splittedString.emplace_back(cache);
	}

	// std::cout << splittedString.size() << std::endl;
	return splittedString;
}

int loadToMemoryWords() {
	std::ifstream inFile("words.txt");

	if (!inFile.is_open()) {
		std::cout << "Fail to open file containing words." << std::endl;
		return 1;
	}

	std::cout << "Reading words." << std::endl;

	std::string line;
	while (std::getline(inFile, line)) {
		if (line[0] == '#') {
			continue;
		}
		if (line == "") {
			continue;
		}
		Word wordObj;
		
		std::vector<std::string> parsed = splitString(line, ',');
		wordObj.word = parsed.at(0);

		std::string temp = parsed.at(0);
		std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
		wordObj.wordClean = temp;

		wordObj.catergory = parsed.at(1);

		words.emplace_back(wordObj);
	}
	std::cout << "Read " << words.size() << " word(s)." << std::endl;

	inFile.close();
	return 0;
}

int genRandomInt(int min, int max) {
	int number = std::rand();
	return (number % max) + min;
}	

Word randomWord() {
	return words.at(genRandomInt(0, words.size()));
}

void clearTerminal() {
	#if defined(_WIN32) || defined(_WIN64)
		std::system("cls");
	#elif defined(__linux__) || defined(__APPLE__)
		std::system("clear");
	#else
		for (int i = 0; i < 50; i++) {
			std::cout << std::endl;
		}
	#endif
}

void renderGallow(int state) {
	std::cout << "+---+ " << std::endl;
	std::cout << "|   | " << std::endl;

	if (state > 0) {
		std::cout << "|   O" << std::endl;
	} else {
		std::cout << "| " << std::endl;
	}

	if (state == 2) {
		std::cout << "|   |" << std::endl;
	} else if (state == 3) {
		std::cout << "|  /|" << std::endl;
	} else if (state >= 4) {
		std::cout << "|  /|\\" << std::endl;	
	} else {	
		std::cout << "| " << std::endl;
	}

	if (state == 5) {
		std::cout << "|  / " << std::endl;
	} else if (state >= 6) {
		std::cout << "|  / \\" << std::endl;
	} else {
		std:: cout << "| " << std::endl;
	}

	std::cout << "| " << std::endl;
	std::cout << "%=======%" << std::endl;
}

bool isPresent(std::vector<char> vector, char target) {
	return std::find(vector.begin(), vector.end(), target) != vector.end();
}


void render(Word wordObj, int failed, std::vector<char> attemptedLetters) {
	clearTerminal();

	renderGallow(failed);

	std::string showingWord;
	for (char c : wordObj.wordClean) {
		// std::cout << c << std::endl;
		if (isPresent(attemptedLetters, c)) {
			showingWord = showingWord + c + " ";
		} else {
			showingWord = showingWord + "* ";
		}
	}

	std::cout << std::endl << "   > " << showingWord << std::endl << std::endl;
	std::cout << "| " << wordObj.catergory << " |" << std::endl;

	int temp = 0;
	for (char c : attemptedLetters) {
		if (wordObj.wordClean.find(c) != std::string::npos) {
			std::cout << "\033[32m" << c << " " << "\033[0m";
		} else {
			std::cout << "\033[31m" << c << " " << "\033[0m";
		}
		temp++;
		if (temp % 5 == 0) {
			std::cout << std::endl;	
		}
	}

	std::cout << std::endl << "| " << "\033[34m" << "Lifes: " << 6-failed << "\033[0m" << " |" << std::endl;
}

void newGame() {
	int failed = 0;
	Word word = randomWord();
	std::vector<char> attemptedLetters;	
	//std::cout << word.word << std::endl;
	
	while (true) {
		render(word, failed, attemptedLetters);

		if (failed == 6) {
			std::cout << "\033[31m" << "Dead." << "\033[0m" << std::endl;
			break;
		}

		int totalFound = 0;
		for (char c : attemptedLetters) {
			if (word.wordClean.find(c) != std::string::npos) {
				totalFound++;
			}
		}

		if (totalFound >= word.wordClean.size()) {
			std::cout << "\033[32m" << "SUCCESS!!" << "\033[0m" << std::endl;
			break;
		}

		std::string input;
		std::cout << "> ";
		std::getline(std::cin, input);

		std::transform(input.begin(), input.end(), input.begin(), ::tolower);

		if (input.length() > 1) {
			if (input == word.wordClean) {
				std::cout << "\033[32m" << "SUCCESS!!" << "\033[0m" << std::endl;
				break;
			} else {
				failed++;
			}
			continue;
		}

		if (input == "") {
			continue;
		}

		const char * c = input.data();
		const char charInpt = c[0];
		if (isPresent(attemptedLetters, charInpt)) {
			continue;
		}
		attemptedLetters.emplace_back(charInpt);

		if (word.wordClean.find(charInpt) == std::string::npos) {
			failed++;
		}

		
	}
}

int main() {
	// Feeds the random lib with an seed.
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	if (loadToMemoryWords()) {
		return 1;	
	};

	while (true) {
		newGame();

		std::string more;
		std::cout << "Do you want an new game? (Y/n) ";
		std::getline(std::cin, more);

		std::transform(more.begin(), more.end(), more.begin(), ::tolower);

		if (more == "no" || more == "n" || more == "nao" || more == "não") {
			break;
		}
	}

	return 0;
}
