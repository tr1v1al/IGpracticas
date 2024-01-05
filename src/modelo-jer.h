// Nombre: Eduardo, Apellidos: Rodríguez Cao, Titulación: DGIIM.
// email: edurodcao@correo.ugr.es, DNI o pasaporte: 54359083D

#ifndef MODELO_JER_H
#define MODELO_JER_H

#include "grafo-escena.h"

using namespace std;

class Apex : public NodoGrafoEscena {
    public:
        Apex();
};

class Turntable : public NodoGrafoEscena {
    public:
        Turntable();
};

class Jib : public NodoGrafoEscena {
    public:
        Jib();
};

class CounterJib : public NodoGrafoEscena {
    public:
        CounterJib();
};

class Mast : public NodoGrafoEscena {
    public:
        Mast();
};

class Hook : public MallaInd
{
   public:
      Hook();
};


class TowerCrane : public NodoGrafoEscena {
    private:
    const unsigned NUM_PARAMETERS = 3;
    glm::mat4 * rot_matrix = nullptr;
    glm::mat4 * trolley_matrix = nullptr;
    glm::mat4 * cable_matrix = nullptr;
    glm::mat4 * hook_matrix = nullptr;
    float angular_velocity = 90;
    float trolley_velocity = 4;
    float cable_velocity = 4;

    public:
    TowerCrane();

    unsigned leerNumParametros() const;

    void actualizarEstadoParametro(const unsigned iParam, const float t_sec);

    void setAngle(const float alpha);

    void setTrolleyTranslation(const float delta);

    void setCableScale(const float delta);

    void setHookTranslation(const float delta);
};

#endif // MODELO_JER_H