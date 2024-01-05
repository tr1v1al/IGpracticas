// Nombre: Eduardo, Apellidos: Rodríguez Cao, Titulación: DGIIM.
// email: edurodcao@correo.ugr.es, DNI o pasaporte: 54359083D

// *********************************************************************
// **
// ** Asignatura: INFORMÁTICA GRÁFICA
// ** 
// ** Gestión de grafos de escena (implementación)
// ** Copyright (C) 2016-2023 Carlos Ureña
// **
// ** Implementación de: 
// **     + Clase 'NodoGrafoEscena' (derivada de 'Objeto3D')
// **     + Clase 'EntradaNGE' (una entrada de un nodo del grafo de escena)
// **     + Tipo enumerado 'TipoEntNGE' (tipo de entradas del nodo del grafo de escena)
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
#include "grafo-escena.h"
#include "aplicacion-ig.h"
#include "seleccion.h"   // para 'ColorDesdeIdent' 

using namespace glm;

// *********************************************************************
// Entrada del nodo del Grafo de Escena

// ---------------------------------------------------------------------
// Constructor para entrada de tipo sub-objeto

EntradaNGE::EntradaNGE( Objeto3D * pObjeto )
{
   assert( pObjeto != nullptr );
   tipo   = TipoEntNGE::objeto ;
   objeto = pObjeto ;
}
// ---------------------------------------------------------------------
// Constructor para entrada de tipo "matriz de transformación"

EntradaNGE::EntradaNGE( const glm::mat4 & pMatriz )
{
   tipo    = TipoEntNGE::transformacion ;
   matriz  = new glm::mat4() ; // matriz en el heap, puntero propietario
   *matriz = pMatriz ;
}

// ---------------------------------------------------------------------
// Constructor para entrada de tipo "matriz de transformación"

EntradaNGE::EntradaNGE( Material * pMaterial )
{
   assert( pMaterial != nullptr );
   tipo     = TipoEntNGE::material ;
   material = pMaterial ;
}

// -----------------------------------------------------------------------------
// Destructor de una entrada

EntradaNGE::~EntradaNGE()
{
   /**  no fnciona debido a que se hacen copias (duplicados) de punteros
   if ( tipo == TipoEntNGE::transformacion )
   {
      assert( matriz != NULL );
      delete matriz ;
      matriz = NULL ;
   }
   * **/
}

// *****************************************************************************
// Nodo del grafo de escena: contiene una lista de entradas
// *****************************************************************************

NodoGrafoEscena::NodoGrafoEscena()
{

}

// -----------------------------------------------------------------------------
// Visualiza usando OpenGL

void NodoGrafoEscena::visualizarGL(  )
{
   using namespace std ;
   assert( apl != nullptr );

    // comprobar que hay un cauce y una pila de materiales y recuperarlos.
   Cauce *          cauce           = apl->cauce ;           assert( cauce != nullptr );
   PilaMateriales * pila_materiales = apl->pila_materiales ; assert( pila_materiales != nullptr );

   // COMPLETAR: práctica 3: implementar la visualización del nodo
   //
   // Se deben de recorrer las entradas y llamar recursivamente de visualizarGL, pero 
   // teniendo en cuenta que, al igual que el método visualizarGL de las mallas indexadas,
   // si el nodo tiene un color, debemos de cambiar el color del cauce (y hacer push/pop). 
   // Además, hay que hacer push/pop de la pila de modelado. 
   // Así que debemos de dar estos pasos:
   //

   // COMPLETAR: práctica 4: añadir gestión de los materiales cuando la iluminación está activada    
   //
   // Si 'apl->iluminacion' es 'true', se deben de gestionar los materiales:
   //
   //   1. al inicio, hacer 'push' de la pila de materiales (guarda material actual en la pila)
   //   2. si una entrada es de tipo material, activarlo usando a pila de materiales
   //   3. al finalizar, hacer 'pop' de la pila de materiales (restaura el material activo al inicio)

   // ......

   // 1. Si el objeto tiene un color asignado (se comprueba con 'tieneColor')
   if (tieneColor()) {
   //     - hacer push del color actual del cauce (con 'pushColor') y después
      cauce->pushColor();
   //     - fijar el color en el cauce (con 'fijarColor'), usando el color del objeto 
   // (se lee con 'leerColor()')
      auto col = leerColor();
      cauce->fijarColor(col);
   }
   
   //   1. al inicio, hacer 'push' de la pila de materiales (guarda material actual en la pila)
   if (apl->iluminacion) {
      pila_materiales->push();
   }

   // 2. Guardar copia de la matriz de modelado (con 'pushMM'), 
   cauce->pushMM();

   // 3. Para cada entrada del vector de entradas:
   int sz = entradas.size();
   for (int i = 0; i < sz; ++i) {
      switch (entradas[i].tipo)
      {

      case TipoEntNGE::objeto :
   //     - si la entrada es de tipo objeto: llamar recursivamente a 'visualizarGL' 
         entradas[i].objeto->visualizarGL();        
         break;

   //     - si la entrada es de tipo transformación: componer la matriz (con 'compMM')      
      case TipoEntNGE::transformacion :
         cauce->compMM(*entradas[i].matriz);
         break;
      case TipoEntNGE::material :
   //   2. si una entrada es de tipo material, activarlo usando a pila de materiales
         if (apl->iluminacion) {
            pila_materiales->activar(entradas[i].material);
         }
         break;
      case TipoEntNGE::noInicializado :
         
         break;   
      }
   }


   // 4. Restaurar la copia guardada de la matriz de modelado (con 'popMM')
   cauce->popMM();

   // 5. Si el objeto tiene color asignado:
   //     - restaurar el color original a la entrada (con 'popColor')
   if (tieneColor()) {
      cauce->popColor();
   }

   //   3. al finalizar, hacer 'pop' de la pila de materiales (restaura el material activo al inicio)
   if (apl->iluminacion) {
      pila_materiales->pop();
   }

}

// *****************************************************************************
// visualizar pura y simplemente la geometría, sin colores, normales, coord. text. etc...

void NodoGrafoEscena::visualizarGeomGL(  )
{
   using namespace std ;
   // comprobar que hay un cauce 
   assert( apl != nullptr );
   Cauce * cauce = apl->cauce; assert( cauce != nullptr );
  
   // COMPLETAR: práctica 3: implementar la visualización del nodo (ignorando colores)
   //
   // Este método hace un recorrido de las entradas del nodo, parecido a 'visualizarGL', pero más simple,
   // Se dan estos pasos:
   //
   // 1. Guardar copia de la matriz de modelado (con 'pushMM'), 
   cauce->pushMM();
   // 2. Para cada entrada del vector de entradas:
   int sz = entradas.size();
   for (int i = 0; i < sz; ++i) {
      switch (entradas[i].tipo)
      {
      case TipoEntNGE::objeto:
   //         - Si la entrada es de tipo objeto: llamar recursivamente a 'visualizarGeomGL'.      
         entradas[i].objeto->visualizarGeomGL();
         break;
      case TipoEntNGE::transformacion:
   //         - Si la entrada es de tipo transformación: componer la matriz (con 'compMM').     
         cauce->compMM(*entradas[i].matriz);
         break;
      case TipoEntNGE::material:
         
         break;
      case TipoEntNGE::noInicializado:
         
         break;
      }
   }

   //   3. Restaurar la copia guardada de la matriz de modelado (con 'popMM')
   // .......
   cauce->popMM();
}

// -----------------------------------------------------------------------------
// Visualizar las normales de los objetos del nodo

void NodoGrafoEscena::visualizarNormalesGL(  )
{
   using namespace std ;

   // comprobar que hay un cauce 
   assert( apl != nullptr );
   Cauce * cauce = apl->cauce; assert( cauce != nullptr );
  

   // COMPLETAR: práctica 4: visualizar las normales del nodo del grafo de escena
   //
   // Este método hace un recorrido de las entradas del nodo, parecido a 'visualizarGL', teniendo 
   // en cuenta estos puntos:
   //
   // - usar push/pop de la matriz de modelado al inicio/fin (al igual que en visualizatGL)
   // - recorrer las entradas, llamando recursivamente a 'visualizarNormalesGL' en los nodos u objetos hijos
   // - ignorar el color o identificador del nodo (se supone que el color ya está prefijado antes de la llamada)
   // - ignorar las entradas de tipo material, y la gestión de materiales (se usa sin iluminación)
   // .......

   cauce->pushMM();
   int sz = entradas.size();
   for (int i = 0; i < sz; ++i) {
      switch (entradas[i].tipo)
      {
      case TipoEntNGE::objeto:
   //         - Si la entrada es de tipo objeto: llamar recursivamente a 'visualizarNormalesGL'.      
         entradas[i].objeto->visualizarNormalesGL();
         break;
      case TipoEntNGE::transformacion:
   //         - Si la entrada es de tipo transformación: componer la matriz (con 'compMM').     
         cauce->compMM(*entradas[i].matriz);
         break;
      case TipoEntNGE::material:
         
         break;
      case TipoEntNGE::noInicializado:
         
         break;
      }
   }
   cauce->popMM();
}

// -----------------------------------------------------------------------------
// visualizar el objeto en 'modo seleccion', es decir, sin iluminación y con los colores 
// basados en los identificadores de los objetos
void NodoGrafoEscena::visualizarModoSeleccionGL()
{
   using namespace std ;
   assert( apl != nullptr );
   Cauce * cauce = apl->cauce ; assert( cauce != nullptr );

   // COMPLETAR: práctica 5: visualizar este nodo en modo selección.
   //
   // Se debe escribir código para dar estos pasos:
   // 
   // 2. Leer identificador (con 'leerIdentificador'), si el identificador no es -1 
   //      + Guardar una copia del color actual del cauce (con 'pushColor')
   //      + Fijar el color del cauce de acuerdo al identificador, (usar 'ColorDesdeIdent'). 
   // 3. Guardar una copia de la matriz de modelado (con 'pushMM')
   // 4. Recorrer la lista de nodos y procesar las entradas transformación o subobjeto:
   //      + Para las entradas subobjeto, invocar recursivamente a 'visualizarModoSeleccionGL'
   //      + Para las entradas transformación, componer la matriz (con 'compMM')
   // 5. Restaurar la matriz de modelado original (con 'popMM')   
   // 6. Si el identificador no es -1, restaurar el color previo del cauce (con 'popColor')
   //
   // ........
   int id = leerIdentificador();
   if (id != -1) {
      cauce->pushColor();
      glm::vec4 v = ColorDesdeIdent(id);
      cauce->fijarColor({v.x, v.y, v.z});
   }
   
   cauce->pushMM();
   int sz = entradas.size();
   for (int i = 0; i < sz; ++i) {
      switch (entradas[i].tipo)
      {
      case TipoEntNGE::objeto:
   //         - Si la entrada es de tipo objeto: llamar recursivamente a 'visualizarModoSeleccionGL'.      
         entradas[i].objeto->visualizarModoSeleccionGL();
         break;
      case TipoEntNGE::transformacion:
   //         - Si la entrada es de tipo transformación: componer la matriz (con 'compMM').     
         cauce->compMM(*entradas[i].matriz);
         break;
      case TipoEntNGE::material:
         
         break;
      case TipoEntNGE::noInicializado:
         
         break;
      }
   }
   cauce->popMM();

   if (id != -1) {
      cauce->popColor();
   }
}

// -----------------------------------------------------------------------------
// Añadir una entrada (al final).
// genérica (de cualqiuer tipo de entrada)

unsigned NodoGrafoEscena::agregar( const EntradaNGE & entrada )
{
   // COMPLETAR: práctica 3: agregar la entrada al nodo, devolver índice de la entrada agregada
   // ........
   entradas.push_back(entrada);
   return (entradas.size() - 1) ; // sustituir por lo que corresponda ....

}
// -----------------------------------------------------------------------------
// construir una entrada y añadirla (al final)
// objeto (copia solo puntero)

unsigned NodoGrafoEscena::agregar( Objeto3D * pObjeto )
{
   return agregar( EntradaNGE( pObjeto ) );
}
// ---------------------------------------------------------------------
// construir una entrada y añadirla (al final)
// matriz (copia objeto)

unsigned NodoGrafoEscena::agregar( const glm::mat4 & pMatriz )
{
   return agregar( EntradaNGE( pMatriz ) );
}
// ---------------------------------------------------------------------
// material (copia solo puntero)
unsigned NodoGrafoEscena::agregar( Material * pMaterial )
{
   return agregar( EntradaNGE( pMaterial ) );
}

// devuelve el puntero a la matriz en la i-ésima entrada
glm::mat4 * NodoGrafoEscena::leerPtrMatriz( unsigned indice )
{
   // COMPLETAR: práctica 3: leer un puntero a una matriz en una entrada de un nodo
   //
   // Devolver el puntero a la matriz en la entrada indicada por 'indice'. 
   // Debe de dar error y abortar si: 
   //   - el índice está fuera de rango 
   //   - la entrada no es de tipo transformación
   //   - el puntero a la matriz es nulo 
   //
   // Sustituir 'return nullptr' por lo que corresponda.
   //
   assert(indice < entradas.size());
   assert(entradas[indice].tipo == TipoEntNGE::transformacion);
   assert(entradas[indice].matriz != nullptr);

   return entradas[indice].matriz ;
}
// -----------------------------------------------------------------------------
// si 'centro_calculado' es 'false', recalcula el centro usando los centros
// de los hijos (el punto medio de la caja englobante de los centros de hijos)

void NodoGrafoEscena::calcularCentroOC()
{
   using namespace std ;
   using namespace glm ;

   // COMPLETAR: práctica 5: calcular y guardar el centro del nodo
   //    en coordenadas de objeto (hay que hacerlo recursivamente)
   //   (si el centro ya ha sido calculado, no volver a hacerlo)
   // ........
   
   // Caso base: centro calculado
   if (centro_calculado) {
      return;
   }

   int total = 0, sz = entradas.size();
   mat4 mat(1.0f);   // Matriz identidad
   vec3 centro = {0.0, 0.0, 0.0}, aux3;
   vec4 aux4;

   for (int i = 0; i < sz; ++i) {
      switch (entradas[i].tipo)
      {
      case TipoEntNGE::objeto:
         entradas[i].objeto->calcularCentroOC();
         aux3 = entradas[i].objeto->leerCentroOC();
         aux4 = mat * vec4{aux3.x, aux3.y, aux3.z, 1};
         centro += vec3{aux4.x, aux4.y, aux4.z};
         ++total;
         break;
      case TipoEntNGE::transformacion:
         mat *= (*entradas[i].matriz);
         break;
      case TipoEntNGE::material:
         
         break;
      case TipoEntNGE::noInicializado:
         
         break;
      }
   }

   ponerCentroOC(centro/float(total));
   centro_calculado = true;
}
// -----------------------------------------------------------------------------
// método para buscar un objeto con un identificador y devolver un puntero al mismo

bool NodoGrafoEscena::buscarObjeto
(
   const int          ident_busc, // identificador a buscar
   const glm::mat4 &  mmodelado,  // matriz de modelado
   Objeto3D       **  objeto,     // (salida) puntero al puntero al objeto
   glm::vec3 &        centro_wc   // (salida) centro del objeto en coordenadas del mundo
)
{
   using namespace std ;
   using namespace glm ;
   
   assert( 0 < ident_busc );

   // COMPLETAR: práctica 5: buscar un sub-objeto con un identificador
   // Se deben de dar estos pasos:

   // 1. calcula el centro del objeto, (solo la primera vez)
   // ........
   calcularCentroOC();

   // 2. si el identificador del nodo es el que se busca, ya está (terminar)
   // ........
   vec3 aux3;
   mat4 mat = mmodelado;
   int sz = entradas.size();

   if (leerIdentificador() == ident_busc) {
      *objeto = this;
      aux3 = leerCentroOC();
      centro_wc = mat*vec4(aux3.x, aux3.y, aux3.z, 1.0);
      return true;
   } else {
   // 3. El nodo no es el buscado: buscar recursivamente en los hijos
   //    (si alguna llamada para un sub-árbol lo encuentra, terminar y devolver 'true')
   // ........
      for (int i = 0; i < sz; ++i) {
         switch (entradas[i].tipo)
         {
         case TipoEntNGE::objeto:
            // Si se encuentra, objeto y centro_wc estaran asignados
            if (entradas[i].objeto->
            buscarObjeto(ident_busc, mat, objeto, centro_wc)) {
               return true;
            }
            break;
         case TipoEntNGE::transformacion:
            mat *= (*entradas[i].matriz);
            break;
         case TipoEntNGE::material:
            
            break;
         case TipoEntNGE::noInicializado:
            
            break;
         }
      }
   }

   // ni este nodo ni ningún hijo es el buscado: terminar
   return false ;
}

// *********************************************************************

GrafoEstrellaX::GrafoEstrellaX(unsigned n) {
   ponerNombre("Grafo estrella X ejer adicional 1 p3");
   ponerIdentificador(300);
   assert(n > 1);

   // Rotación dependiende del parámetro
   unsigned ind1 = agregar( rotate(radians(0.0f), vec3(1.0, 0.0, 0.0)) );

   // Estrella
   NodoGrafoEscena * estrella = new NodoGrafoEscena();
   estrella->agregar(rotate(radians(90.0f), vec3(+0.0, +1.0, +0.0))); // Rotamos alrededor de Y, antihorario
   estrella->agregar(scale(vec3(+1.3*2, +1.3*2, +1.0))); // Escalado con centro en origen
   estrella->agregar(translate(vec3(-0.5, -0.5, +0.0)));
   estrella->agregar(new EstrellaZ(n));
   agregar(estrella);

   // Conos
   NodoGrafoEscena * cono = new NodoGrafoEscena();
   cono->agregar(translate(vec3(+0.0, +0.0, -1.3)));
   cono->agregar(rotate(radians(90.0f), vec3(-1.0, +0.0, +0.0))); // Rotamos alrededor de X, horario
   cono->agregar(scale(vec3(+0.14, +0.15, +0.14)));
   cono->agregar(new Cono(10, 20));

   float angle = 360.0/n;    
   for (int i = 0; i < n; ++i) {
      agregar(rotate(radians(angle), vec3(+1.0, +0.0, +0.0)));
      agregar(cono);
   }

   // Guardamos matriz
   pm_rot_alpha = leerPtrMatriz(ind1);
}

unsigned GrafoEstrellaX::leerNumParametros() const {
   return NUM_PARAMS;
}

void GrafoEstrellaX::actualizarEstadoParametro(const unsigned iParam, const float t_sec) {
   switch (0)
   {
   case 0:
      fijarAnguloGiro(angulo*t_sec);
      break;
   
   default:
      break;
   }
}

void GrafoEstrellaX::fijarAnguloGiro(const float alpha){
   *pm_rot_alpha = rotate(radians(alpha), vec3(1.0, 0.0, 0.0));
}

// *********************************************************************

GrafoCubos::GrafoCubos() {
   ponerIdentificador(301);
   ponerNombre("Grafo estrella X ejer adicional 1 p3");
   // Rejilla
   NodoGrafoEscena * rejilla = new NodoGrafoEscena();
   rejilla->agregar(translate(vec3(-0.5, +0.5, -0.5)));
   rejilla->agregar(new RejillaY(rejillas_m, rejillas_n));

   // Cubo
   NodoGrafoEscena * cubo = new NodoGrafoEscena();
   unsigned ind1 = cubo->agregar(rotate(radians(0.0f), vec3(+0.0, +1.0, +0.0)));
   cubo->agregar(translate(vec3(+0.0, +0.8, +0.0)));
   cubo->agregar(scale(vec3(+0.15, +0.3, +0.15)));
   cubo->agregar(new Cubo());
   
   // Agregamos 4 caras
   for (int i = 0; i < 4; ++i) {
      agregar(rotate(radians(90.0f), vec3(+1.0, +0.0, +0.0))); // Rotamos X
      agregar(cubo);
      agregar(rejilla);
   }
   // Y las 2 restantes
   agregar(rotate(radians(90.0f), vec3(+0.0, +0.0, +1.0))); // Rotamos Z
   agregar(cubo);
   agregar(rejilla);
   agregar(rotate(radians(180.0f), vec3(+0.0, +0.0, +1.0)));
   agregar(cubo);
   agregar(rejilla);

   // Guardamos matriz
   pm_rot_alpha = cubo->leerPtrMatriz(ind1);
}

unsigned GrafoCubos::leerNumParametros() const {
   return NUM_PARAMS;
}

void GrafoCubos::actualizarEstadoParametro(const unsigned iParam, const float t_sec) {
   switch (0)
   {
   case 0:
      fijarAnguloGiro(angulo*t_sec);
      break;
   
   default:
      break;
   }
}

void GrafoCubos::fijarAnguloGiro(const float alpha){
   *pm_rot_alpha = rotate(radians(alpha), vec3(+0.0, +1.0, +0.0));
}

// *********************************************************************

NodoGrafoCubo24::NodoGrafoCubo24() {
   ponerIdentificador(302);
   ponerNombre("NodoGrafoCubo24 p4");
   agregar(new Material(new Textura("window-icon.jpg"), 1.0f, 1.0f, 1.0f, 10.0f));
   agregar(new Cubo24());
}

