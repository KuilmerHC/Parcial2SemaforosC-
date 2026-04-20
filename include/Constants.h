#pragma once /* esto es simplemente para evitar errores, lo que hace es que le dice al compilador que este archivo solo
existe una sola vez a pesar de ser llamado en multiples ocasiones */

#include <SFML/Graphics.hpp>
#include <string>

/* Simple uso de la libreria grafica SMFL y la libreria de str */

inline constexpr int WIN_W = 900;
inline constexpr int WIN_H = 700;
/*Define el ancho y alto de la ventana en píxeles (900x700). */

inline constexpr float ROAD_CX = 450.f;
inline constexpr float ROAD_CY = 350.f;
/*  Es el centro de la intersección en la pantalla. Esta ubicado precisamente en la mitad de la ventana*/

inline constexpr float ROAD_W = 82.f;
/* El ancho de las calles en píxeles*/

inline constexpr float STOP_H = ROAD_CX - ROAD_W / 2.f - 6.f;
inline constexpr float STOP_V = ROAD_CY - ROAD_W / 2.f - 6.f;
/*Las lineas en donde los carros se detienen antes del semaforo*/

inline constexpr float CAR_L = 46.f;
inline constexpr float CAR_A = 26.f;
/*Largo y anchos de los carros*/
inline constexpr float MIN_GAP = 6.f;

inline const sf::Color PALETA[7] = {
    sf::Color(210, 55, 55),
    sf::Color(55, 115, 215),
    sf::Color(50, 175, 60),
    sf::Color(215, 165, 30),
    sf::Color(170, 50, 175),
    sf::Color(50, 175, 200),
    sf::Color(230, 120, 40),
};

#ifdef _WIN32
inline const std::string FONT_PATH = "C:\\Windows\\Fonts\\arial.ttf";
#else
inline const std::string FONT_PATH = "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf";
#endif
