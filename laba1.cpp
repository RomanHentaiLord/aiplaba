#include <iostream>
#include <cmath>
#include <stdexcept>
#include <string>

// Пользовательский класс исключений
class CustomException : public std::exception {
private:
    std::string message;
    double value;
    int errorCode;

public:
    CustomException(const std::string& msg, double val, int code)
        : message(msg), value(val), errorCode(code) {}

    const char* what() const noexcept override {
        return message.c_str();
    }

    double getValue() const { return value; }
    int getErrorCode() const { return errorCode; }
};

// Функция для вычисления Z1
double calculateZ1(double a) {
    if (a == 0 || a == -1) {
        throw CustomException("Division by zero or invalid input in Z1 calculation", a, 101);
    }
    double numerator = (1 + a * a + 2);
    double denominator = (a - std::sqrt(2 * a));
    return numerator / denominator - 2 / (1 - a + a * a);
}

// Функция для вычисления Z2
double calculateZ2(double a) {
    if (a + std::sqrt(2) == 0) {
        throw CustomException("Division by zero in Z2 calculation", a, 102);
    }
    return 1 / (std::sqrt(a + std::sqrt(2)));
}

int main() {
    double a;
    std::cout << "Enter the value of a: ";
    std::cin >> a;

    try {
        double Z1 = calculateZ1(a);
        double Z2 = calculateZ2(a);

        std::cout << "Z1 = " << Z1 << std::endl;
        std::cout << "Z2 = " << Z2 << std::endl;

        if (std::abs(Z1 - Z2) < 1e-6) {
            std::cout << "Z1 and Z2 are approximately equal." << std::endl;
        } else {
            std::cout << "Z1 and Z2 are not approximately equal." << std::endl;
        }
    } catch (const CustomException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << "Value: " << e.getValue() << std::endl;
        std::cerr << "Error Code: " << e.getErrorCode() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Standard exception: " << e.what() << std::endl;
    }

    return 0;
}
