#include "Cruce.h"
#include <SFML/Graphics.hpp>
#include <thread>
#include <atomic>
#include <optional>
#include <chrono>

int main()
{
    sf::RenderWindow window(
        sf::VideoMode({WIN_W, WIN_H}),
        "Simulacion de Cruce Semaforico",
        sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);
    (void)window.setActive(false); // libera el contexto OpenGL del hilo principal
                                   // para que el hilo de render pueda tomarlo

    Cruce cruce;
    std::atomic<bool> corriendo(true); // flag compartido entre hilos

    // ── Hilo de lógica ──────────────────────────────────────────
    std::thread hiloLogica([&]()
                           {
        sf::Clock reloj;
        const float targetDt = 1.0f / 60.0f; // 60 actualizaciones por segundo
        while (corriendo)
        {
            float dt = reloj.restart().asSeconds();
            if (dt < targetDt)
            {
                std::this_thread::sleep_for(std::chrono::duration<float>(targetDt - dt));
                dt = targetDt;
            }
            else if (dt > 0.05f)
                dt = 0.05f;

            cruce.actualizar(dt); // protegido internamente por mutex
        } });

    // ── Hilo principal: eventos + render ────────────────────────
    (void)window.setActive(true); // el hilo principal toma el contexto OpenGL
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            if (event->is<sf::Event::KeyPressed>() &&
                event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape)
                window.close();
        }

        window.clear(sf::Color(55, 95, 55));
        cruce.dibujar(window); // protegido internamente por mutex
        window.display();
    }

    // ── Cierre limpio ───────────────────────────────────────────
    corriendo = false; // le avisa al hilo de lógica que pare
    hiloLogica.join(); // espera a que el hilo termine antes de salir

    return 0;
}