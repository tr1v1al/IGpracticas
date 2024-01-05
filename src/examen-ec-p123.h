#ifndef EXAMEN_EC_P123_HPP
#define EXAMEN_EC_P123_HPP
#include "malla-ind.h"
#include "grafo-escena.h"

class P1MallaPiramide : public MallaInd
{
   public:
      P1MallaPiramide();
};

class P2Barrido : public MallaInd
{
   public:
      P2Barrido(unsigned m, unsigned n);
};

class P3Mesa : public NodoGrafoEscena {
   private:
      const unsigned NUM_PARAMS = 2;
      glm::mat4 * pm_rot_alpha = nullptr;
      glm::mat4 * pm_trans = nullptr;
      glm::mat4 * pm_scale = nullptr;
      float angulo = 0.8;
      unsigned escalado = 1.3;
   
   public:
      P3Mesa();

      unsigned leerNumParametros() const;

      void actualizarEstadoParametro(const unsigned iParam, const float t_sec);

      void fijarAnguloGiro(const float alpha);

      void fijarEscalado(const float alpha);

      void fijarTranslacion(const float alpha);
};

#endif