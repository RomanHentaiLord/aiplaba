#include <iostream>
#include <iomanip>

int main() {
    std::cout << "Введите символы (для выхода нажмите Ctrl+D в Linux/macOS или Ctrl+Z в Windows):" << std::endl;

    char ch;
    while (std::cin.get(ch)) { // Считываем символ
        std::cout << "Символ: " << ch 
                  << ", Шестнадцатеричный код: 0x" << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << (int)(unsigned char)ch 
                  << ", Восьмеричный код: 0" << std::oct << (int)(unsigned char)ch 
                  << std::dec << std::endl; // Возвращаем в десятичный формат для других выводов
    }

    std::cout << "Программа завершена." << std::endl;
    return 0;
}
