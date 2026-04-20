#include "Vehiculo.h"

Vehiculo::Vehiculo(sf::Vector2f p, bool h, sf::Color c)
    : pos(p), esH(h), vel(105.f), color(c), activo(true), esperando(false)
{
}

float Vehiculo::frente() const
{
    return esH ? pos.x + CAR_L : pos.y + CAR_L;
}

sf::FloatRect Vehiculo::bounds() const
{
    return esH ? sf::FloatRect({pos.x, pos.y}, {CAR_L, CAR_A})
               : sf::FloatRect({pos.x, pos.y}, {CAR_A, CAR_L});
}

void Vehiculo::actualizar(float dt, float lineaStop, float gapAhead, bool semaforoVerde)
{
    float f = frente();
    bool porAuto = gapAhead < MIN_GAP;
    bool antesLinea = f <= lineaStop;
    bool semaforoBloquea = !semaforoVerde && antesLinea;

    float avance = vel * dt;
    float maxAvanceSemaforo = avance;
    if (semaforoBloquea)
    {
        maxAvanceSemaforo = lineaStop - f;
        if (maxAvanceSemaforo < 0.f)
            maxAvanceSemaforo = 0.f;
    }

    float maxAvanceAuto = gapAhead - MIN_GAP;
    if (maxAvanceAuto < 0.f)
        maxAvanceAuto = 0.f;

    float movimiento = std::min(avance, std::min(maxAvanceSemaforo, maxAvanceAuto));

    if (movimiento > 0.f)
    {
        if (esH)
            pos.x += movimiento;
        else
            pos.y += movimiento;
        esperando = false;
    }
    else
    {
        esperando = porAuto || semaforoBloquea;
    }

    if (esH && pos.x > WIN_W + 70)
        activo = false;
    if (!esH && pos.y > WIN_H + 70)
        activo = false;
}

void Vehiculo::dibujar(sf::RenderWindow &w) const
{
    if (!activo)
        return;

    float cw = esH ? CAR_L : CAR_A;
    float ch = esH ? CAR_A : CAR_L;

    sf::RectangleShape body({cw, ch});
    body.setFillColor(color);
    body.setOutlineColor({0, 0, 0, 160});
    body.setOutlineThickness(1.5f);
    body.setPosition(pos);
    w.draw(body);

    sf::RectangleShape vidrio(sf::Vector2f(16.f, 16.f));
    vidrio.setFillColor({170, 225, 255, 210});
    if (esH)
        vidrio.setPosition({pos.x + 25, pos.y + 5});
    else
        vidrio.setPosition({pos.x + 5, pos.y + 25});
    w.draw(vidrio);

    sf::RectangleShape faro(esH ? sf::Vector2f(5.f, 7.f) : sf::Vector2f(7.f, 5.f));
    faro.setFillColor({255, 255, 160});
    if (esH)
    {
        faro.setPosition({pos.x + CAR_L - 5, pos.y + 3});
        w.draw(faro);
        faro.setPosition({pos.x + CAR_L - 5, pos.y + CAR_A - 10});
        w.draw(faro);
    }
    else
    {
        faro.setPosition({pos.x + 3, pos.y + CAR_L - 5});
        w.draw(faro);
        faro.setPosition({pos.x + CAR_A - 10, pos.y + CAR_L - 5});
        w.draw(faro);
    }

    if (esperando)
    {
        sf::RectangleShape freno(esH ? sf::Vector2f(5.f, 7.f) : sf::Vector2f(7.f, 5.f));
        freno.setFillColor({255, 50, 50});
        if (esH)
        {
            freno.setPosition({pos.x, pos.y + 3});
            w.draw(freno);
            freno.setPosition({pos.x, pos.y + CAR_A - 10});
            w.draw(freno);
        }
        else
        {
            freno.setPosition({pos.x + 3, pos.y});
            w.draw(freno);
            freno.setPosition({pos.x + CAR_A - 10, pos.y});
            w.draw(freno);
        }
    }
}
