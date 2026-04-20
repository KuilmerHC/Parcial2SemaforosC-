#pragma once

#include <SFML/Graphics.hpp>
#include <random>
#include <vector>
#include "GestorSemaforos.h"
#include "Vehiculo.h"

class Cruce
{
public:
    GestorSemaforos gestor;
    std::vector<Vehiculo> vehiculos; /*  la lista de todos los carros y 
                                        que es dinámica, los carros se agregan y eliminan constantemente.*/
    sf::Font fuente; /*Tipografía para escribir texto dento del HUD*/
    bool fuenteOK; /*Para evitar errores creamos este booleano que verifica si la fuente cargó bien o no*/

    std::mt19937 rng; /*Numeros aleatorios*/
    std::uniform_real_distribution<float> distTiempo; /*Tiempos aleatorios entre 2.2 y 4.8*/
    std::uniform_int_distribution<int> distColor;/*Numero aleatorio entre 0 y 6 para que se elija un color de la paleta*/

    float timerH;
    float timerV;
    /*Cronometros que cuentan desde el ultimo auto*/
    float prxSpawnH;
    float prxSpawnV;
    /*Tiempo aleatorio que debe pasar antes de que el próximo carro aparezca*/

    Cruce();

    void actualizar(float dt);
    void dibujarCalles(sf::RenderWindow &w) const;
    void dibujarHUD(sf::RenderWindow &w) const;
    void dibujar(sf::RenderWindow &w) const;

private:
    void spawnH(); /*Crea un nuevo carro en la Horizontal*/
    void spawnV(); /*Crea un nuevo carro en la Vertical*/
    float calcularGap(const Vehiculo &v, int idx) const; /*Calcula el espacio que hay entre los carros para que no se estrellen*/
};
