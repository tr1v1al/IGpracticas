#ifndef LATAPEONES_H
#define LATAPEONES_H

#include "grafo-escena.h"

using namespace std;

class Lata : public NodoGrafoEscena {
    public:
        Lata(string textura);
};

class Peon : public NodoGrafoEscena{
    private:
        int total = 1;
        glm::mat4 *mat = nullptr; 
   public:
      Peon();
      virtual bool cuandoClick(const glm::vec3 & centro_wc);
};

class LataPeones : public NodoGrafoEscena {
    public:
        LataPeones();
};

class VariasLatasPeones : public NodoGrafoEscena {
    public:
        VariasLatasPeones();
};

#endif // LATAPEONES_H