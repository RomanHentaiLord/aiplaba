#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

// Структура для хранения данных о прямоугольнике
struct Rectangle {
    sf::RectangleShape shape;
    sf::Color fillColor;
};

int main() {
    // Создаем окно
    sf::RenderWindow window(sf::VideoMode(800, 600), "Rectangle Drawer");

    // Вектор для хранения всех прямоугольников
    std::vector<Rectangle> rectangles;

    // Текущий прямоугольник
    Rectangle currentRect;
    bool isDrawing = false; // Флаг для проверки, рисуем ли мы сейчас

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Обработка нажатия мыши
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                isDrawing = true;
                currentRect.shape.setPosition(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
                currentRect.shape.setFillColor(sf::Color::Transparent);
                currentRect.shape.setOutlineColor(sf::Color::White);
                currentRect.shape.setOutlineThickness(1);
            }

            // Обработка отпускания мыши
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                if (isDrawing) {
                    isDrawing = false;
                    // Задаем случайный цвет для заливки прямоугольника
                    sf::Color randomColor(
                        static_cast<sf::Uint8>(rand() % 256),
                        static_cast<sf::Uint8>(rand() % 256),
                        static_cast<sf::Uint8>(rand() % 256)
                    );
                    currentRect.shape.setFillColor(randomColor);
                    currentRect.fillColor = randomColor;
                    rectangles.push_back(currentRect);
                }
            }

            // Обработка нажатия клавиши для выхода
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
        }

        // Обновление прямоугольника при движении мыши
        if (isDrawing) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::Vector2f startPos = currentRect.shape.getPosition();
            currentRect.shape.setSize(sf::Vector2f(static_cast<float>(mousePos.x) - startPos.x,
                                                  static_cast<float>(mousePos.y) - startPos.y));
        }

        // Отрисовка
        window.clear();
        for (const auto& rect : rectangles) {
            window.draw(rect.shape);
        }

        if (isDrawing) {
            window.draw(currentRect.shape);
        }

        window.display();
    }

    return 0;
}