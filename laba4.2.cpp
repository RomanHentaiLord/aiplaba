#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>

// Функция для создания входного файла с 100 случайными числами
void prepareInputFile(const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Ошибка при открытии файла для записи: " << filename << std::endl;
        return;
    }
    
    std::srand(std::time(nullptr)); // Инициализация генератора случайных чисел
    for (int i = 0; i < 100; ++i) {
        int randomNumber = std::rand() % 101 - 50; // Генерация числа в диапазоне [-50, 50]
        outFile << randomNumber << '\n';
    }

    outFile.close();
}

// Функция для чтения чисел из файла
std::vector<int> readNumbersFromFile(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile) {
        std::cerr << "Ошибка при открытии файла для чтения: " << filename << std::endl;
        return {};
    }

    std::vector<int> numbers;
    int number;
    while (inFile >> number) {
        numbers.push_back(number);
    }

    inFile.close();
    return numbers;
}

// Функция для записи преобразованных чисел в выходной файл
void writeNumbersToFile(const std::string& filename, const std::vector<double>& numbers) {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Ошибка при открытии файла для записи: " << filename << std::endl;
        return;
    }

    for (double number : numbers) {
        outFile << number << '\n';
    }

    outFile.close();
}

// Основная функция для преобразования чисел
void processNumbers(const std::string& inputFilename, const std::string& outputFilename) {
    // Чтение чисел из входного файла
    std::vector<int> numbers = readNumbersFromFile(inputFilename);
    if (numbers.empty()) {
        std::cerr << "Нет данных для обработки." << std::endl;
        return;
    }

    // Вычисление среднего арифметического всех чисел
    double sum = 0;
    for (int number : numbers) {
        sum += std::abs(number);
    }
    double average = sum / numbers.size();

    if (average == 0) {
        std::cerr << "Среднее арифметическое равно 0, деление невозможно." << std::endl;
        return;
    }

    // Преобразование чисел
    std::vector<double> transformedNumbers;
    for (int number : numbers) {
        if (std::abs(number) % 2 == 1) { // Проверка на нечетность по модулю
            transformedNumbers.push_back(number / average);
        } else {
            transformedNumbers.push_back(number); // Четные остаются без изменений
        }
    }

    // Запись преобразованных чисел в выходной файл
    writeNumbersToFile(outputFilename, transformedNumbers);
}

int main() {
    const std::string inputFilename = "input.txt";
    const std::string outputFilename = "output.txt";

    // Подготовка входного файла
    prepareInputFile(inputFilename);

    // Обработка чисел и запись в выходной файл
    processNumbers(inputFilename, outputFilename);

    std::cout << "Обработка завершена. Проверьте файлы: " << inputFilename << " и " << outputFilename << std::endl;

    return 0;
}
