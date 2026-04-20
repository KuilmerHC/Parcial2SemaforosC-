#include "Cruce.h"

#include <algorithm>
#include <iomanip>
#include <sstream>

Cruce::Cruce()
    : fuenteOK(false),
      rng(std::random_device{}()),
      distTiempo{2.2f, 4.8f},
      distColor{0, 6},
      timerH(0.f),
      timerV(1.4f),
      prxSpawnH(2.5f),
      prxSpawnV(2.0f)
{
    fuenteOK = fuente.openFromFile(FONT_PATH);
}

void Cruce::spawnH()
{
    float y = ROAD_CY - CAR_A / 2.f;
    vehiculos.emplace_back(sf::Vector2f(-CAR_L - 5.f, y), true, PALETA[distColor(rng)]);
}

void Cruce::spawnV()
{
    float x = ROAD_CX - CAR_A / 2.f;
    vehiculos.emplace_back(sf::Vector2f(x, -CAR_L - 5.f), false, PALETA[distColor(rng)]);
}

float Cruce::calcularGap(const Vehiculo &v, int idx) const
{
    float gap = 99999.f;
    for (int j = 0; j < static_cast<int>(vehiculos.size()); j++)
    {
        if (j == idx)
            continue;
        const auto &o = vehiculos[j];
        if (o.esH != v.esH)
            continue;

        if (v.esH)
        {
            if (o.pos.x > v.pos.x)
            {
                float d = o.pos.x - (v.pos.x + CAR_L);
                if (d < gap)
                    gap = d;
            }
        }
        else
        {
            if (o.pos.y > v.pos.y)
            {
                float d = o.pos.y - (v.pos.y + CAR_L);
                if (d < gap)
                    gap = d;
            }
        }
    }
    return gap;
}

void Cruce::actualizar(float dt)
{
    std::lock_guard<std::mutex> lock(mtx); // bloquea el mutex al entrar, lo libera al salir automáticamente

    gestor.actualizar(dt);

    timerH += dt;
    if (timerH >= prxSpawnH)
    {
        timerH = 0.f;
        prxSpawnH = distTiempo(rng);
        spawnH();
    }

    timerV += dt;
    if (timerV >= prxSpawnV)
    {
        timerV = 0.f;
        prxSpawnV = distTiempo(rng);
        spawnV();
    }

    std::vector<int> orden(vehiculos.size());
    for (int i = 0; i < static_cast<int>(orden.size()); i++)
        orden[i] = i;

    std::sort(orden.begin(), orden.end(), [this](int a, int b)
    {
        const auto &va = vehiculos[a];
        const auto &vb = vehiculos[b];
        if (va.esH != vb.esH)
            return va.esH;
        if (va.esH)
            return va.pos.x > vb.pos.x;
        return va.pos.y > vb.pos.y;
    });

    for (int idx : orden)
    {
        auto &v = vehiculos[idx];
        float lin = v.esH ? STOP_H : STOP_V;
        EstadoSemaforo sem = v.esH ? gestor.semH.getEstado() : gestor.semV.getEstado();
        bool semaforoVerde = (sem == EstadoSemaforo::VERDE);
        float gap = calcularGap(v, idx);
        v.actualizar(dt, lin, gap, semaforoVerde);
    }

    vehiculos.erase(
        std::remove_if(vehiculos.begin(), vehiculos.end(),
                       [](const Vehiculo &v)
                       { return !v.activo; }),
        vehiculos.end());
}

void Cruce::dibujarCalles(sf::RenderWindow &w) const
{
    sf::RectangleShape rh({static_cast<float>(WIN_W), ROAD_W});
    rh.setFillColor({85, 85, 85});
    rh.setPosition({0, ROAD_CY - ROAD_W / 2.f});
    w.draw(rh);

    sf::RectangleShape rv({ROAD_W, static_cast<float>(WIN_H)});
    rv.setFillColor({85, 85, 85});
    rv.setPosition({ROAD_CX - ROAD_W / 2.f, 0});
    w.draw(rv);

    auto borde = [&](sf::Vector2f p, sf::Vector2f s)
    {
        sf::RectangleShape b(s);
        b.setFillColor({210, 210, 210});
        b.setPosition(p);
        w.draw(b);
    };

    borde({0, ROAD_CY - ROAD_W / 2.f - 2}, {static_cast<float>(WIN_W), 3.f});
    borde({0, ROAD_CY + ROAD_W / 2.f - 1}, {static_cast<float>(WIN_W), 3.f});
    borde({ROAD_CX - ROAD_W / 2.f - 2, 0}, {3.f, static_cast<float>(WIN_H)});
    borde({ROAD_CX + ROAD_W / 2.f - 1, 0}, {3.f, static_cast<float>(WIN_H)});

    for (int x = 0; x < WIN_W; x += 55)
    {
        float fx = static_cast<float>(x);
        if (fx + 30 > ROAD_CX - ROAD_W / 2.f && fx < ROAD_CX + ROAD_W / 2.f)
            continue;
        sf::RectangleShape d({30.f, 3.f});
        d.setFillColor({240, 215, 0});
        d.setPosition({fx, ROAD_CY - 1.5f});
        w.draw(d);
    }

    for (int y = 0; y < WIN_H; y += 55)
    {
        float fy = static_cast<float>(y);
        if (fy + 30 > ROAD_CY - ROAD_W / 2.f && fy < ROAD_CY + ROAD_W / 2.f)
            continue;
        sf::RectangleShape d({3.f, 30.f});
        d.setFillColor({240, 215, 0});
        d.setPosition({ROAD_CX - 1.5f, fy});
        w.draw(d);
    }

    sf::RectangleShape slH({4.f, ROAD_W});
    slH.setFillColor(sf::Color::White);
    slH.setPosition({STOP_H, ROAD_CY - ROAD_W / 2.f});
    w.draw(slH);

    sf::RectangleShape slV({ROAD_W, 4.f});
    slV.setFillColor(sf::Color::White);
    slV.setPosition({ROAD_CX - ROAD_W / 2.f, STOP_V});
    w.draw(slV);

    auto flecha = [&](float ax, float ay, bool horiz)
    {
        sf::ConvexShape f(3);
        if (horiz)
        {
            f.setPoint(0, {0, -9});
            f.setPoint(1, {18, 0});
            f.setPoint(2, {0, 9});
        }
        else
        {
            f.setPoint(0, {-9, 0});
            f.setPoint(1, {0, 18});
            f.setPoint(2, {9, 0});
        }
        f.setFillColor({255, 255, 255, 90});
        f.setPosition({ax, ay});
        w.draw(f);
    };

    flecha(160, ROAD_CY, true);
    flecha(680, ROAD_CY, true);
    flecha(ROAD_CX, 145, false);
    flecha(ROAD_CX, 540, false);
}

void Cruce::dibujarHUD(sf::RenderWindow &w) const
{
    sf::RectangleShape panel({280.f, 150.f});
    panel.setFillColor({0, 0, 0, 175});
    panel.setPosition({10, 10});
    w.draw(panel);

    if (!fuenteOK)
        return;

    auto txt = [&](const std::string &s, float x, float y, unsigned sz, sf::Color c)
    {
        sf::Text t(fuente, s, sz);
        t.setFillColor(c);
        t.setPosition({x, y});
        w.draw(t);
    };

    auto colorSem = [](const Semaforo &s) -> sf::Color
    {
        switch (s.getEstado())
        {
        case EstadoSemaforo::VERDE:   return {0, 230, 0};
        case EstadoSemaforo::AMARILLO: return {255, 220, 0};
        case EstadoSemaforo::ROJO:    return {255, 70, 70};
        }
        return sf::Color::White;
    };

    txt("-- SEMAFOROS --", 20, 16, 14, {200, 200, 200});
    txt("Horizontal:", 20, 36, 13, {180, 180, 180});
    txt(gestor.semH.getNombre(), 130, 34, 17, colorSem(gestor.semH));
    txt("Vertical:", 20, 60, 13, {180, 180, 180});
    txt(gestor.semV.getNombre(), 130, 58, 17, colorSem(gestor.semV));

    std::ostringstream cr;
    cr << "Cambia en: " << std::fixed << std::setprecision(1)
       << gestor.tiempoRestante() << " s";
    txt(cr.str(), 20, 88, 13, {170, 170, 170});

    int nh = 0, nv = 0;
    for (auto &v : vehiculos)
    {
        if (v.esH) nh++;
        else       nv++;
    }

    std::ostringstream cv;
    cv << "Autos  H: " << nh << "   V: " << nv;
    txt(cv.str(), 20, 110, 13, {160, 200, 160});
    txt("ESC para salir", 20, 134, 11, {120, 120, 120});
}

void Cruce::dibujar(sf::RenderWindow &w) const
{
    std::lock_guard<std::mutex> lock(mtx); // bloquea el mutex al entrar, lo libera al salir automáticamente

    dibujarCalles(w);
    for (auto &v : vehiculos)
        v.dibujar(w);
    gestor.dibujar(w);
    dibujarHUD(w);
}