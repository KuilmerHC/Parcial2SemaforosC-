#include "GestorSemaforos.h"

GestorSemaforos::GestorSemaforos()
    : semH({390.f, 285.f}, EstadoSemaforo::VERDE),
      semV({482.f, 378.f}, EstadoSemaforo::ROJO),
      fase(0),
      timer(0.f)
{
}

float GestorSemaforos::duracionFase() const
{
    return (fase == 0 || fase == 2) ? T_VERDE : T_AMARILLO;
}

float GestorSemaforos::tiempoRestante() const
{
    return duracionFase() - timer;
}

bool GestorSemaforos::hPuede() const
{
    return semH.getEstado() == EstadoSemaforo::VERDE;
}

bool GestorSemaforos::vPuede() const
{
    return semV.getEstado() == EstadoSemaforo::VERDE;
}

void GestorSemaforos::actualizar(float dt)
{
    timer += dt;
    if (timer < duracionFase())
        return;

    timer = 0.f;
    fase = (fase + 1) % 4;

    switch (fase)
    {
    case 0:
        semH.setEstado(EstadoSemaforo::VERDE);
        semV.setEstado(EstadoSemaforo::ROJO);
        break;
    case 1:
        semH.setEstado(EstadoSemaforo::AMARILLO);
        semV.setEstado(EstadoSemaforo::ROJO);
        break;
    case 2:
        semH.setEstado(EstadoSemaforo::ROJO);
        semV.setEstado(EstadoSemaforo::VERDE);
        break;
    case 3:
        semH.setEstado(EstadoSemaforo::ROJO);
        semV.setEstado(EstadoSemaforo::AMARILLO);
        break;
    }
}

void GestorSemaforos::dibujar(sf::RenderWindow &w) const
{
    semH.dibujar(w);
    semV.dibujar(w);
}
