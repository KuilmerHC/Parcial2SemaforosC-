#pragma once 

#include <SFML/Graphics.hpp>
#include "Constants.h"


class Vehiculo
{
public:
    sf::Vector2f pos; /* su posición en la pantalla (coordenadas X, Y) */
    bool esH; /* indica si el vehículo va en dirección horizontal (true) o vertical (false) */
    float vel; /* su velocidad en píxeles por segundo que esconstante para cada vehículo */
    sf::Color color;
    bool activo; /* indica si el vehículo está activo*/
    bool esperando; /* indica si el vehículo está esperando en la línea de detención */

    Vehiculo(sf::Vector2f p, bool h, sf::Color c);

    float frente() const;
    sf::FloatRect bounds() const; /*devuelve el rectángulo que ocupa el vehículo */
    void actualizar(float dt, float lineaStop, float gapAhead, bool semaforoVerde); 
    /*se llama cada frame y decide si el carro avanza, frena o espera según*/
    void dibujar(sf::RenderWindow &w) const;
};
