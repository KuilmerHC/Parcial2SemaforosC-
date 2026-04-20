#include "Semaforo.h"

Semaforo::Semaforo(sf::Vector2f p, EstadoSemaforo e)
    : estado(e), pos(p)
{
}

void Semaforo::setEstado(EstadoSemaforo e)
{
    estado = e;
}

EstadoSemaforo Semaforo::getEstado() const
{
    return estado;
}

std::string Semaforo::getNombre() const
{
    switch (estado)
    {
    case EstadoSemaforo::VERDE:
        return "VERDE";
    case EstadoSemaforo::AMARILLO:
        return "AMARILLO";
    case EstadoSemaforo::ROJO:
        return "ROJO";
    }
    return "";
}

sf::Color Semaforo::getColor() const
{
    switch (estado)
    {
    case EstadoSemaforo::VERDE:
        return sf::Color(0, 230, 0);
    case EstadoSemaforo::AMARILLO:
        return sf::Color(255, 215, 0);
    case EstadoSemaforo::ROJO:
        return sf::Color(230, 0, 0);
    }
    return sf::Color::White;
}

void Semaforo::dibujar(sf::RenderWindow &w) const
{
    sf::RectangleShape poste({6.f, 55.f});
    poste.setFillColor(sf::Color(55, 55, 55));
    poste.setPosition({pos.x - 3.f, pos.y + 42.f});
    w.draw(poste);

    sf::RectangleShape caja({36.f, 96.f});
    caja.setFillColor(sf::Color(25, 25, 25));
    caja.setOutlineColor(sf::Color(80, 80, 80));
    caja.setOutlineThickness(2.f);
    caja.setPosition({pos.x - 18.f, pos.y - 48.f});
    w.draw(caja);

//estructura local llamada Luz
    struct Luz
    {
        float oy; //desplazamiento vertical dentro de la caja
        EstadoSemaforo match; // a qué estado corresponde esa luz
        sf::Color apagado;
        sf::Color encendido;
    };

    Luz luces[3] = {
        {-34.f, EstadoSemaforo::ROJO, {70, 0, 0, 255}, {230, 0, 0, 255}},
        {-8.f, EstadoSemaforo::AMARILLO, {70, 60, 0, 255}, {255, 215, 0, 255}},
        {18.f, EstadoSemaforo::VERDE, {0, 60, 0, 255}, {0, 230, 0, 255}},
    };


    //Recorre las 3 luces y dibuja cada una como un círculo de radio 12
    for (auto &l : luces)
    {
        sf::CircleShape c(12.f);
        c.setFillColor(estado == l.match ? l.encendido : l.apagado); // Si el estado coincide con el match encendido sino apagado
        c.setPosition({pos.x - 12.f, pos.y + l.oy});
        w.draw(c);
    }
}
