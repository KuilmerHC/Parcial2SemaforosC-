#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "Constants.h"

enum class EstadoSemaforo
{
    VERDE,
    AMARILLO,
    ROJO
};

class Semaforo
{
public:
    EstadoSemaforo estado;
    sf::Vector2f pos; /* su posición en la pantalla (coordenadas X, Y)*/

    Semaforo(sf::Vector2f p, EstadoSemaforo e);

    void setEstado(EstadoSemaforo e);
    EstadoSemaforo getEstado() const;
    std::string getNombre() const;
    sf::Color getColor() const;
    void dibujar(sf::RenderWindow &w) const;
};
