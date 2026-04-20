#include "Cruce.h"
#include <SFML/Graphics.hpp>
#include <optional>

int main()
{    //Crea la ventana gráfica
    sf::RenderWindow window(
        sf::VideoMode({WIN_W, WIN_H}),
        "Simulacion de Cruce Semaforico",
        sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

    Cruce cruce; //Crea el objeto Cruce
    sf::Clock reloj;
        //Se repite 60 veces por segundo mientras la ventana esté abierta
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent()) // revisa si el usuario hizo algo
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            if (event->is<sf::Event::KeyPressed>() &&
                event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape)
                window.close();
        }

        float dt = reloj.restart().asSeconds(); // reinicia el reloj y devuelve el tiempo transcurrido desde el último frame
        if (dt > 0.05f)
            dt = 0.05f; //o limita a 0.05 segundos

        cruce.actualizar(dt); //le pasa el delta time a actualizar para que avance la simulación

        window.clear(sf::Color(55, 95, 55));
        cruce.dibujar(window);
        window.display();
    }

    return 0;
}
