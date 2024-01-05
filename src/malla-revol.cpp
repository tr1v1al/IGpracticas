// Nombre: Eduardo, Apellidos: Rodríguez Cao, Titulación: DGIIM.
// email: edurodcao@correo.ugr.es, DNI o pasaporte: 54359083D

// *********************************************************************
// **
// ** Asignatura: INFORMÁTICA GRÁFICA
// ** 
// ** Mallas indexadas (implementación)
// ** Copyright (C) 2016-2023 Carlos Ureña
// **
// ** Implementación de las clases 
// **    + MallaRevol: malla indexada de triángulos obtenida por 
// **      revolución de un perfil (derivada de MallaInd)
// **    + MallaRevolPLY: malla indexada de triángulos, obtenida 
// **      por revolución de un perfil leído de un PLY (derivada de MallaRevol)
// **    + algunas clases derivadas de MallaRevol
// **
// ** This program is free software: you can redistribute it and/or modify
// ** it under the terms of the GNU General Public License as published by
// ** the Free Software Foundation, either version 3 of the License, or
// ** (at your option) any later version.
// **
// ** This program is distributed in the hope that it will be useful,
// ** but WITHOUT ANY WARRANTY; without even the implied warranty of
// ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// ** GNU General Public License for more details.
// **
// ** You should have received a copy of the GNU General Public License
// ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
// **
// *********************************************************************

#include "ig-aux.h"
#include "lector-ply.h"
#include "malla-revol.h"

using namespace std ;

// *****************************************************************************




// Método que crea las tablas de vértices, triángulos, normales y cc.de.tt.
// a partir de un perfil y el número de copias que queremos de dicho perfil.
void MallaRevol::inicializar
(
   const std::vector<glm::vec3> & perfil,     // tabla de vértices del perfil original
   const unsigned               num_copias  // número de copias del perfil
)
{
   using namespace glm ;
   
   // Practica 4: normales
   vector<vec3> m, n;   // Normales de aristas y vertices del perfil respectivamente
   vec3 aux3;
   vec4 aux4;
   // Matrix de rotacion de normales de aristas
   mat4 rot = rotate(radians(-90.0f), vec3(+0.0, +0.0, +1.0));
   int sz = perfil.size(); // Tamaño

   // Normales de aristas del perfil (sz-1 aristas)
   for (int i = 1; i < sz; ++i) {
      aux3 = perfil[i] - perfil[i-1];
      aux4 = {aux3.x, aux3.y, aux3.z, 0};
      aux3 = rot*aux4;
      aux3 = length(aux3) != 0 ? normalize(aux3) : aux3;
      m.push_back(aux3);
   }
   // Normales de vertices del perfil (sz vertices)
   n.push_back(m[0]);
   for (int i = 0; i < sz-2; ++i) {
      aux3 = m[i]+m[i+1];
      aux3 = length(aux3) != 0 ? normalize(aux3) : aux3;
      n.push_back(aux3);
   }
   n.push_back(m[sz-2]);
   
   // Practica 4: texturas
   vector<float> t;
   float sum = 0.0f;

   t.push_back(sum);
   for (int i = 1; i < sz; ++i) {
      // Calculamos el divisor para normalizar 
      // (longitud de la polilinea)
      sum += distance(perfil[i], perfil[i-1]);
      // y el vector de distancias a lo largo del perfil
      // desde el vertice 0 hasta i
      t.push_back(sum);
   }
   
   // Normalizamos t
   for (int i = 1; i < sz; ++i) {
      t[i] /= sum;
   }

   // COMPLETAR: práctica 2: implementar algoritmo de creación de malla de revolución
   //
   // Escribir el algoritmo de creación de una malla indexada por revolución de un 
   // perfil, según se describe en el guion de prácticas.
   //
   // ............................... 

   float angle;            // Ángulo de rotación
   int k = 0;

   // Vértices
   vertices.clear();

   // Recorro instancias
   for (unsigned int i = 0; i < num_copias; ++i) {
      // Calculo ángulo de rotación en grados y su matriz
      angle = 360.0*i/(num_copias-1);
      rot = rotate(radians(angle), vec3(0, +1.0, 0));

      // Recorro perfil
      for (int j = 0; j < sz; ++j) {
         // Calculo el punto rotado, cambiando de notación vec3 a vec4
         // para poder multiplicar por la matriz de rotación.
         aux3 = perfil[j];
         aux4 = {aux3.x, aux3.y, aux3.z, 1};
         aux3 = rot * aux4;
         vertices.push_back(aux3);

         // Obtengo normales
         aux3 = n[j];
         aux4 = {aux3.x, aux3.y, aux3.z, 0};
         aux3 = rot * aux4;
         nor_ver.push_back(aux3);

         // Obtengo texturas, (1-t) ya que invertimos verticalmente
         cc_tt_ver.push_back({float(i)/(num_copias-1), 1-t[j]});
      }
   }

   // Triángulos
   // Recorro instancias
   for (unsigned int i = 0; i < num_copias-1; ++i) {
      // Recorro perfil
      for (int j = 0; j < sz-1; ++j) {
         // Índice del punto del perfil de la instancia dada
         k = i*sz+j;
         // Añado triángulos
         triangulos.push_back(vec3(k, k+sz, k+sz+1));
         triangulos.push_back(vec3(k, k+sz+1, k+1));
      }
   }
}

// -----------------------------------------------------------------------------
// constructor, a partir de un archivo PLY

MallaRevolPLY::MallaRevolPLY
(
   const std::string & nombre_arch,
   const unsigned      nperfiles
)
{
   ponerNombre( std::string("malla por revolución del perfil en '"+ nombre_arch + "'" ));
   // COMPLETAR: práctica 2: crear la malla de revolución
   // Leer los vértice del perfil desde un PLY, después llamar a 'inicializar'
   // ...........................
   vector<glm::vec3> aux;
   LeerVerticesPLY(nombre_arch, aux);
   inicializar(aux, nperfiles);
}

Cilindro::Cilindro(const int num_verts_per, const unsigned nperfiles) {
   ponerIdentificador(200);
   vector<glm::vec3> perfil;
   float dist = 1.0/(num_verts_per-1);
   perfil.push_back(glm::vec3(+1.0, 0, 0));
   for (int i = 1; i < num_verts_per; ++i) {
      perfil.push_back(glm::vec3(+1.0, perfil[i-1].y+dist, 0));
   }
   inicializar(perfil, nperfiles);
}

Cono::Cono(const int num_verts_per, const unsigned nperfiles) {
   ponerIdentificador(201);
   vector<glm::vec3> perfil;
   float dist = 1.0/(num_verts_per-1);
   perfil.push_back(glm::vec3(+1.0, 0, 0));
   for (int i = 1; i < num_verts_per; ++i) {
      perfil.push_back(glm::vec3(perfil[i-1].x-dist, perfil[i-1].y+dist, 0));
   }
   inicializar(perfil, nperfiles);   
}

Esfera::Esfera(const int num_verts_per, const unsigned nperfiles) {
   ponerIdentificador(202);
   using namespace glm;
   float angle;
   vector<glm::vec3> perfil;
   vec3 curr;
   vec4 rot_point, aux;
   
   // Rotamos alrededor del eje z
   angle = 180.0/(num_verts_per-1);
   mat4 rot_mat = rotate(radians(angle), vec3(0, 0, +1.0));
   perfil.push_back(glm::vec3(0, -1.0, 0));
   
   // Generamos perfil rotando un punto
   for (int i = 1; i < num_verts_per; ++i) {
      curr = perfil[i-1];
      aux = {curr.x, curr.y, curr.z, 1};
      rot_point = rot_mat * aux;
      curr = {rot_point.x, rot_point.y, rot_point.z};
      perfil.push_back(curr);
   }
   inicializar(perfil, nperfiles);   
}



