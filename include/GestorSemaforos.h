#pragma once

#include <SFML/Graphics.hpp>
#include "Semaforo.h"

class GestorSemaforos
{
public:
    Semaforo semH;
    Semaforo semV;

    int fase; /*indica en qué parte del ciclo está el semáforo. Va de 0 a 3:
                 0=H.Verde, 1=H.Amarillo, 2=V.Verde, 3=V.Amarillo*/
    float timer; /*cuenta el tiempo que lleva el semáforo en la fase actual*/

    static constexpr float T_VERDE = 6.f;
    static constexpr float T_AMARILLO = 2.f;

    GestorSemaforos();

    float duracionFase() const;
    float tiempoRestante() const;
    bool hPuede() const;
    bool vPuede() const;
    void actualizar(float dt);
    void dibujar(sf::RenderWindow &w) const;/* Dibuja los dos semáforos en la ventana llamando al método dibujar de cada uno. */
};
