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
// **        + MallaInd: malla indexada de triángulos (derivada de Objeto3D)
// **        + MallaPLY: malla indexada de triángulos, leída de un PLY (derivada de MallaInd)
// **        + algunas clases derivadas de MallaInd.
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
#include "aplicacion-ig.h"
#include "malla-ind.h"   // declaración de 'ContextoVis'
#include "lector-ply.h"
#include "seleccion.h"   // para 'ColorDesdeIdent' 


// *****************************************************************************
// funciones auxiliares

// *****************************************************************************
// métodos de la clase MallaInd.

MallaInd::MallaInd()
{
   // nombre por defecto
   ponerNombre("malla indexada, anónima");
}
// -----------------------------------------------------------------------------

MallaInd::MallaInd( const std::string & nombreIni )
{
   // 'identificador' puesto a 0 por defecto, 'centro_oc' puesto a (0,0,0)
   ponerNombre(nombreIni) ;
}

//-----------------------------------------------------------------------------
// calcula la tabla de normales de triángulos una sola vez, si no estaba calculada

void MallaInd::calcularNormalesTriangulos()
{
   using namespace glm ;
   
   // si ya está creada la tabla de normales de triángulos, no es necesario volver a crearla
   const unsigned nt = triangulos.size() ;
   assert( 1 <= nt );
   if ( 0 < nor_tri.size() )
   {
      assert( nt == nor_tri.size() );
      return ;
   }

   // COMPLETAR: Práctica 4: creación de la tabla de normales de triángulos
   // ....
   for (int i = 0; i < nt; ++i) {
      vec3 a = vertices[triangulos[i][1]] - vertices[triangulos[i][0]];
      vec3 b = vertices[triangulos[i][2]] - vertices[triangulos[i][0]];
      vec3 prod = cross(a,b);
      vec3 normal = (length(prod) != 0) ? normalize(prod) : vec3(0.0f, 0.0f, 0.0f) ;
      nor_tri.push_back(normal);
   }
}


// -----------------------------------------------------------------------------
// calcula las dos tablas de normales

void MallaInd::calcularNormales()
{
   using namespace glm ;
   // COMPLETAR: en la práctica 4: calculo de las normales de la malla
   // se debe invocar en primer lugar 'calcularNormalesTriangulos'
   // .......
   calcularNormalesTriangulos();
   int vsz = vertices.size(), tsz = triangulos.size();

   // Inicializamos nor_ver
   for (int i = 0; i < vsz; ++i) {
      nor_ver.push_back({0.0f, 0.0f, 0.0f});
   }

   // Recorremos caras
   for (int i = 0; i < tsz; ++i) {
      nor_ver[triangulos[i][0]] += nor_tri[i];
      nor_ver[triangulos[i][1]] += nor_tri[i];
      nor_ver[triangulos[i][2]] += nor_tri[i];
   }

   // Normalizamos
   for (int i = 0; i < vsz; ++i) {
      if (length(nor_ver[i]) != 0) {
         nor_ver[i] = normalize(nor_ver[i]);
      }
   }
}


// --------------------------------------------------------------------------------------------

void MallaInd::visualizarGL( )
{
   // comprobar algunas precondiciones básicas
   using namespace std ;
   assert( apl != nullptr );
   Cauce * cauce = apl->cauce ; assert( cauce != nullptr );
   CError();

   // si la malla no vértices o no tiene triángulos, imprimir advertencia y salir.
   if ( triangulos.size() == 0 || vertices.size() == 0 )
   {  cout << "advertencia: intentando dibujar malla vacía '" << leerNombre() << "'" << endl << flush ;
      return ;
   }

   // COMPLETAR: práctica 1: cambiar color del cauce
   //
   // Si el objeto tiene un color asignado (se comprueba con 'tieneColor')

   if (tieneColor()) {
   //    - hacer push del color actual del cauce
      cauce->pushColor();
      
   //    - fijar el color en el cauce usando el color del objeto (se lee con 'leerColor()')
      auto col = leerColor();
      cauce->fijarColor(col.r, col.g, col.b);
   }

   // COMPLETAR: práctica 1: crear el descriptor de VAO, si no está creado
   //  Si el VAO ya está creado, (dvao no nulo), no hay que hacer nada.
   //  Si el puntero 'dvao' es nulo, crear el descriptor de VAO
   if (!dvao) {
   //   * en primer lugar se crea el descriptor de VAO, con su constructor 
   //     (se le pasa como parámetro la tabla de posiciones y el número de atributos del cauce).      
      dvao = new DescrVAO(numero_atributos_cauce, 
      new DescrVBOAtribs(ind_atrib_posiciones, vertices));

   //   * se añade el descriptor de VBO con la tabla de índices (la tabla de triángulos),
      if ( triangulos.size() > 0 ) {
         dvao->agregar( new DescrVBOInds( triangulos ));      
      }

   //   * finalmente se añaden al VAO los descriptores VBOs con tablas de atributos 
   //     que no estén vacías   
      if ( col_ver.size() > 0) {
         dvao->agregar( new DescrVBOAtribs( ind_atrib_colores, col_ver ));
      }
      
      if ( nor_ver.size() > 0) {
         dvao->agregar( new DescrVBOAtribs( ind_atrib_normales, nor_ver ));
      }
      //if ( nor_tri.size() > 0) {
      //   dvao->agregar( new DescrVBOAtribs( ind_atrib_normales, nor_tri ));
      //}
      if ( cc_tt_ver.size() > 0 ) {
         dvao->agregar( new DescrVBOAtribs( ind_atrib_coord_text, cc_tt_ver ));
      }
   
   }

   // COMPLETAR: práctica 1: visualizar el VAO usando el método 'draw' de 'DescrVAO'
   dvao->draw(GL_TRIANGLES);

   // COMPLETAR: práctica 1: restaurar color anterior del cauce 
   //
   // Si el objeto tiene un color asignado (se comprueba con 'tieneColor')
   //    - hacer 'pop' del color actual del cauce
   if (tieneColor()) {
      cauce->popColor();
   }
}


// -----------------------------------------------------------------------------
// Visualizar el objeto con OpenGL
// usa las tablas de normales, colores y coordenadas de textura, si no están vacías.
      
void MallaInd::visualizarGeomGL( )
{
   // Comprobar que el descriptor de VAO ya está creado
   // (es decir, este método únicamente se podrá invocar después de que 
   // se haya llamado a 'visualizaGL')
   
   assert( dvao != nullptr );

   // COMPLETAR: práctica 1: visualizar únicamente la geometría del objeto 
   // 
   //    1. Desactivar todas las tablas de atributos del VAO (que no estén vacías)
   if ( col_ver.size() > 0) {
      dvao->habilitarAtrib(ind_atrib_colores, false);
   }
   
   if ( nor_ver.size() > 0) {
      dvao->habilitarAtrib(ind_atrib_normales, false);
   }

   if ( cc_tt_ver.size() > 0 ) {
      dvao->habilitarAtrib(ind_atrib_coord_text, false);
   }

   //    2. Dibujar la malla (únicamente visualizará los triángulos)
   dvao->draw(GL_TRIANGLES);
   
   //    3. Volver a activar todos los atributos para los cuales la tabla no esté vacía
   if ( col_ver.size() > 0) {
      dvao->habilitarAtrib(ind_atrib_colores, true);
   }
   
   if ( nor_ver.size() > 0) {
      dvao->habilitarAtrib(ind_atrib_normales, true);
   }

   if ( cc_tt_ver.size() > 0 ) {
      dvao->habilitarAtrib(ind_atrib_coord_text, true);
   }
   // ....

}

// -----------------------------------------------------------------------------
// Visualizar las normales del objeto, si no tiene tabla de normales imprime 
// advertencia y no hace nada.

void MallaInd::visualizarNormalesGL(  )
{
   using namespace std ;
   assert( apl != nullptr );
   Cauce * cauce = apl->cauce ; assert( cauce != nullptr );

   if ( nor_ver.size() == 0 )
   {
      cout << "Advertencia: intentando dibujar normales de una malla que no tiene tabla (" << leerNombre() << ")." << endl ;
      return ;
   }  

   if( nor_ver.size() != vertices.size() )
   {
      cout << "Error visu. normales: tabla de normales no vacía y de tamaño distinto a la de vértices." << endl ;
      cout << "Nombre del objeto        : " << leerNombre() << endl ;
      cout << "Tamaño tabla vértices    : " << vertices.size() << endl ;
      cout << "Tamaño tabla de normales : " << nor_ver.size() << endl ;
      exit(1);
   }
   CError();

   // COMPLETAR: práctica 4: visualizar las normales del objeto MallaInd
   // 
   // *1* Si el puntero al descriptor de VAO de normales ('dvao_normales') es nulo, 
   //    debemos de crear dicho descriptor, con estos pasos:
   //
   //       * Para cada posición 'v_i' de un vértice en el vector 'vertices':
   //             - Leer la correspondiente normal 'n_i' del vector de normales ('nor_ver').
   //             - Añadir 'v_i' al vector 'segmentos_normales'.
   //             - Añadir 'v_i+a*n_i' al vector 'segmentos_normales'.
   //
   //       * Crear el objeto descriptor del VAO de normales, para ello se usa el vector 
   //          'segmentos_normales' y se tiene en cuenta que esa descriptor únicamente gestiona 
   //          una tabla de atributos de vértices (la de posiciones, ya que las otras no se 
   //          necesitan).
   // 
   // *2* Visualizar el VAO de normales, usando el método 'draw' del descriptor, con el 
   //       tipo de primitiva 'GL_LINES'.

   if (!dvao_normales) {
      int sz = vertices.size();
      float alpha = 0.3f;
      for (int i = 0; i < sz; ++i) {
         segmentos_normales.push_back(vertices[i]);
         segmentos_normales.push_back(vertices[i] + alpha*nor_ver[i]);
      }
      dvao_normales = new DescrVAO(numero_atributos_cauce, 
      new DescrVBOAtribs(ind_atrib_posiciones, segmentos_normales));
      segmentos_normales.clear();
   }
   dvao_normales->draw(GL_LINES);
   //  ..........

}

// -----------------------------------------------------------------------------
// visualizar el objeto en 'modo seleccion', es decir, sin iluminación y con los colores 
// basados en los identificadores de los objetos
void MallaInd::visualizarModoSeleccionGL() 
{

   using namespace std ;
   assert( apl != nullptr );
   Cauce * cauce = apl->cauce ; assert( cauce != nullptr );

   // COMPLETAR: práctica 5: visualizar la malla en modo selección 
   //
   // Se debe escribir código para visualizar únicamente la geometría, pero usando el color 
   // obtenido a partir del identificador. El código da estos pasos:
   // 
   // 1. Leer el identificador del objeto (con 'leerIdentificador'). Si el objeto tiene 
   //    identificador (es decir, si su identificador no es -1)
   //       + Hacer push del color del cauce, con 'pushColor'.
   //       + Fijar el color del cauce (con 'fijarColor') usando un color obtenido a 
   //         partir del identificador (con 'ColorDesdeIdent'). 
   // 2. Invocar 'visualizarGeomGL' para visualizar la geometría.
   // 3. Si tiene identificador: hacer pop del color, con 'popColor'.
   //
   int id = leerIdentificador();
   if (id != -1) {
      cauce->pushColor();
      glm::vec4 v = ColorDesdeIdent(id);
      cauce->fijarColor({v.x, v.y, v.z});
   }
   visualizarGeomGL();
   if (id != -1) {
      cauce->popColor();
   }
}


// ****************************************************************************
// Clase 'MallaPLY'

MallaPLY::MallaPLY( const std::string & nombre_arch )
{
   ponerNombre( std::string("malla leída del archivo '") + nombre_arch + "'" );

   // COMPLETAR: práctica 2: leer archivo PLY e inicializar la malla
   // ..........................
   LeerPLY(nombre_arch, vertices, triangulos);

   // COMPLETAR: práctica 4: invocar  a 'calcularNormales' para el cálculo de normales
   // .................
   calcularNormales();
}

// ****************************************************************************
// Clase 'Cubo'

Cubo::Cubo()
:  MallaInd( "cubo 8 vértices" )
{
   ponerIdentificador(100);
   vertices =
      {  { -1.0, -1.0, -1.0 }, // 0
         { -1.0, -1.0, +1.0 }, // 1
         { -1.0, +1.0, -1.0 }, // 2
         { -1.0, +1.0, +1.0 }, // 3
         { +1.0, -1.0, -1.0 }, // 4
         { +1.0, -1.0, +1.0 }, // 5
         { +1.0, +1.0, -1.0 }, // 6
         { +1.0, +1.0, +1.0 } // 7
      } ;

   triangulos =
      {  {0,1,3}, {0,3,2}, // X-
         {4,7,5}, {4,6,7}, // X+ (+4)

         {0,5,1}, {0,4,5}, // Y-
         {2,3,7}, {2,7,6}, // Y+ (+2)

         {0,6,4}, {0,2,6}, // Z-
         {1,5,7}, {1,7,3}  // Z+ (+1)
      } ;

   calcularNormales();
}

// ****************************************************************************
// Clase 'Tetraedro'

Tetraedro::Tetraedro()
:  MallaInd( "tetraedro 4 vértices" )
{
   ponerIdentificador(101);
   vertices =
      {  { -1.0,  0.0, -1.0/sqrt(2)}, // 0
         {  1.0,  0.0, -1.0/sqrt(2)}, // 1
         {  0.0, -1.0,  1.0/sqrt(2)}, // 2
         {  0.0,  1.0,  1.0/sqrt(2)} // 3
      } ;



   triangulos =
      {  {0,1,2}, {0,2,3},
         {0,1,3}, {1,2,3}
      } ;
   ponerColor(glm::vec3(0.5, 0.5, 0.5));
   calcularNormales();
}

// ****************************************************************************
// Clase 'CuboColores'

CuboColores::CuboColores()
:  MallaInd( "cubo 8 vértices con colores" )
{
   ponerIdentificador(102);
   vertices =
      {  { -1.0, -1.0, -1.0 }, // 0
         { -1.0, -1.0, +1.0 }, // 1
         { -1.0, +1.0, -1.0 }, // 2
         { -1.0, +1.0, +1.0 }, // 3
         { +1.0, -1.0, -1.0 }, // 4
         { +1.0, -1.0, +1.0 }, // 5
         { +1.0, +1.0, -1.0 }, // 6
         { +1.0, +1.0, +1.0 }  // 7
      } ;

   triangulos =
      {  {0,1,3}, {0,3,2}, // X-
         {4,7,5}, {4,6,7}, // X+ (+4)

         {0,5,1}, {0,4,5}, // Y-
         {2,3,7}, {2,7,6}, // Y+ (+2)

         {0,6,4}, {0,2,6}, // Z-
         {1,5,7}, {1,7,3}  // Z+ (+1)
      } ;
      
   col_ver =
      {  {    0,    0,    0 }, // 0
         {    0,    0, +1.0 }, // 1
         {    0, +1.0,    0 }, // 2
         {    0, +1.0, +1.0 }, // 3
         { +1.0,    0,    0 }, // 4
         { +1.0,    0, +1.0 }, // 5
         { +1.0, +1.0,    0 }, // 6
         { +1.0, +1.0, +1.0 }  // 7
      } ;
}

// -----------------------------------------------------------------------------------------------

EstrellaZ::EstrellaZ(unsigned n)
: MallaInd("Estrella ejercicio adicional 1 prac1") 
{
   ponerIdentificador(103);
   float x = 0, y = 0;
   vertices.push_back({+0.5, +0.5, +0.0});   // Centro
   col_ver.push_back({+1.0, +1.0, +1.0});    // Color del centro
   for (int i = 0; i < 2*n; i+=2) {
      // i*M_PI/n es el ángulo, se divide el cos y sin por 2
      // ya que el radio es la 0.5. Se añade 0.5 para trasladar
      x = float(cos(i*M_PI/n)/2 + 0.5);
      y = float(sin(i*M_PI/n)/2 + 0.5);
      vertices.push_back({x, y, +0.0});
      col_ver.push_back({x, y, +0.0});

      // Aquí dividimos por 4, para que sea estrella
      x = float(cos((i+1)*M_PI/n)/4 + 0.5);
      y = float(sin((i+1)*M_PI/n)/4 + 0.5);
      vertices.push_back({x, y, +0.0});
      col_ver.push_back({x, y, +0.0});    

      triangulos.push_back({0, i+1, i+2});
      triangulos.push_back({0, i+2, i+3});
   }
   triangulos.pop_back();
   triangulos.push_back({0, 2*n, 1});  // Último que queda
}

// -----------------------------------------------------------------------------------------------

CasaX::CasaX()
:  MallaInd( "Casa ejercicio adicional 2 prac1" )
{
   ponerIdentificador(104);
   vertices =
      {  { +0.0, +0.0, +0.0 }, // 0) base, orden antihorario
         { +0.0, +0.0, +0.5 }, // 1) tiene longitud 1 en X,
         { +1.0, +0.0, +0.5 }, // 2) 0.5 en Z
         { +1.0, +0.0, +0.0 }, // 3)
         { +0.0, +0.8, +0.0 }, // 4) tapa, orden antihorario
         { +0.0, +0.8, +0.5 }, // 5) altura 0.8 en Y
         { +1.0, +0.8, +0.5 }, // 6)
         { +1.0, +0.8, +0.0 }, // 7)
         { +0.0, +1.0, +0.25}, // 8) Techo, altura 1 en Y
         { +1.0, +1.0, +0.25}  // 9)
      } ;

   triangulos =
      {  {0, 4, 7}, {0, 7, 3},  // Paredes, antihorario
         {0, 5, 4}, {0, 1, 5},
         {1, 6, 5}, {1, 2, 6},
         {2, 3, 6}, {3, 7, 6},
         {4, 8, 9}, {4, 9, 7},  // Tejado
         {8, 5, 9}, {5, 6, 9},
         {4, 5, 8}, {6, 7, 9}
      } ; 

   col_ver =
      {  { +0.0, +0.0, +0.0 }, // 0) base, orden antihorario
         { +0.0, +0.0, +0.5 }, // 1) tiene longitud 1 en X,
         { +1.0, +0.0, +0.5 }, // 2) 0.5 en Z
         { +1.0, +0.0, +0.0 }, // 3)
         { +0.0, +0.8, +0.0 }, // 4) tapa, orden antihorario
         { +0.0, +0.8, +0.5 }, // 5) altura 0.8 en Y
         { +1.0, +0.8, +0.5 }, // 6)
         { +1.0, +0.8, +0.0 }, // 7)
         { +0.0, +1.0, +0.25}, // 8) Techo, altura 1 en Y
         { +1.0, +1.0, +0.25}  // 9)
      } ; 
}


// -----------------------------------------------------------------------------------------------

MallaTriangulo::MallaTriangulo()
:  MallaInd( "Malla triangulo ejercicio adicional 3 prac1" )
{
   ponerIdentificador(105);
   vertices = {
      {-0.5, +0.0, +0.0}, {+0.5, +0.0, +0.0}, {+0.0, sqrt(2), +0.0}
   };

   triangulos = {
      {0, 1, 2}
   };
}

// -----------------------------------------------------------------------------------------------


MallaCuadrado::MallaCuadrado()
:  MallaInd( "Malla cuadrado ejercicio adicional 3 prac1" )
{
   ponerIdentificador(106);
   vertices = {
      {-1.0, -1.0, +0.0}, {+1.0, -1.0, +0.0},
      {+1.0, +1.0, +0.0}, {-1.0, +1.0, +0.0}
   };

   triangulos = {
      {0, 1, 2}, {0, 2, 3}
   };
}

// -----------------------------------------------------------------------------------------------

MallaPiramideL::MallaPiramideL()
:  MallaInd( "Malla piramide L ejercicio adicional 3 prac1" )
{
   ponerIdentificador(107);
   vertices = {
      {+0.25, +1.0, +0.25},  // 0) apice
      {+0.0, +0.0, +0.0}, // 1) Base, antihoraria
      {+0.0, +0.0, +1.0}, // 2)
      {+0.5, +0.0, +1.0}, // 3)
      {+0.5, +0.0, +0.5}, // 4)
      {+1.0, +0.0, +0.5}, // 5)
      {+1.0, +0.0, +0.0}  // 6)
   };

   triangulos = {
      {1, 6, 2}, {2, 4, 3}, {4, 6, 5}, // base
      {0, 6, 1}, {0, 1, 2}, {0, 2, 3}, // caras, antihorario
      {0, 3, 4}, {0, 4, 5}, {0, 5, 6}
   };
}

// -----------------------------------------------------------------------------------------------

PiramideEstrellaZ::PiramideEstrellaZ(unsigned n)
:  MallaInd( "Piramide estrella Z ejercicio adicional 1 prac2" )
{
   ponerIdentificador(108);
   using namespace glm ;
   assert(n > 1);
   // Generamos la estrella con centro en el origen, al final trasladamos
   vertices.push_back({+0.5, +0.5, +0.0});   // Centro
   col_ver.push_back({+1.0, +1.0, +1.0});    // Color del centro

   // Rotamos alrededor de Z
   float angle = 360.0/(2*n);
   mat3 rot_mat = rotate(radians(angle), vec3(+0.0, +0.0, +1.0));
   vec3 curr, aux;

   vertices.push_back({+0.5, +0.0, +0.0});   // Primer vertice

   // Generamos resto de vertices
   for (int i = 1; i < 2*n; ++i) {
      curr = vertices[i];
      aux = rot_mat * curr;
      aux[0] = (i % 2 == 0) ? aux[0]*2 : aux[0]/2;
      aux[1] = (i % 2 == 0) ? aux[1]*2 : aux[1]/2;
      vertices.push_back(aux);
      triangulos.push_back({0, i, i+1});
   }
   triangulos.push_back({0, 2*n, 1});  // Último triángulo

   // Trasladamos y ponemos los colores
   for (int i = 1; i < 2*n+1; ++i) {
      vertices[i][0] += 0.5;
      vertices[i][1] += 0.5;
      col_ver.push_back(vertices[i]);
   }

   vertices.push_back({+0.5, +0.5, +0.5});   // Vertice de piramide
   col_ver.push_back({+1.0, +1.0, +1.0});

   // Triangulos de piramide
   for (int i = 1; i < 2*n; ++i) {
      triangulos.push_back({2*n+1, i, i+1});
   }
   triangulos.push_back({2*n+1, 2*n, 1});
}

// -----------------------------------------------------------------------------------------------

RejillaY::RejillaY(unsigned m, unsigned n)
:  MallaInd( "Malla piramide L ejercicio adicional 2 prac2" )
{
   ponerIdentificador(109);
   assert(n > 1);
   assert(m > 1);
   // Dimensiones de celda
   float distx = 1.0/(m-1), distz = 1.0/(n-1);
   // Generamos vertices
   for (int i = 0; i < m; ++i) {
      for (int j = 0; j < n; ++j) {
         vertices.push_back({i*distx, +0.0, j*distz});
         col_ver.push_back({i*distx, +0.0, j*distz});
      }
   }
   // Generamos triangulos
   for (int i = 1; i < m; ++i) {
      for (int j = 1; j < n; ++j) {
         triangulos.push_back({n*(i-1)+j, n*i+j-1, n*(i-1)+j-1});
         triangulos.push_back({n*(i-1)+j, n*i+j, n*i+j-1});
      }
   }
}

// -----------------------------------------------------------------------------------------------

MallaTorre::MallaTorre(unsigned n)
:  MallaInd( "Malla piramide L ejercicio adicional 3 prac2" )
{
   ponerIdentificador(110);
   assert(n > 1);
   //Generamos vertices
   for (int i = 0; i <=n; ++i) {
      vertices.push_back({-0.5, i, -0.5});
      vertices.push_back({-0.5, i, +0.5});
      vertices.push_back({+0.5, i, +0.5});
      vertices.push_back({+0.5, i, -0.5});
   }
   // Generamos triangulos
   for (int i = 0; i < n; ++i) {
      triangulos.push_back({4*i, 4*i+5, 4*i+4});
      triangulos.push_back({4*i, 4*i+1, 4*i+5});
      triangulos.push_back({4*i+1, 4*i+6, 4*i+5});
      triangulos.push_back({4*i+1, 4*i+2, 4*i+6});
      triangulos.push_back({4*i+2, 4*i+7, 4*i+6});
      triangulos.push_back({4*i+2, 4*i+3, 4*i+7});
      triangulos.push_back({4*i+3, 4*i+4, 4*i+7});
      triangulos.push_back({4*i+3, 4*i, 4*i+4});
   }
}

// -----------------------------------------------------------------------------------------------

// ****************************************************************************
// Clase 'Cubo24'

Cubo24::Cubo24()
:  MallaInd( "cubo 24 vértices" )
{
   //ponerIdentificador(111);
   // Separamos cada vertice en 3, de tal forma que
   // el cubo se convierte en 6 caras independientes, tales que
   // tienen tripletas de vértices en la misma posición,
   // pero cada vértice pertenece a un triángulo distinto de forma
   // que los vértices no son adyacentes a caras perpendiculares, 
   // permitiendo calcular las normales sin promediar mal
   // Además al separar las 6 caras podemos asignar coordinadas de 
   // textura ya que así cada vértice tiene coords únicas
   vertices =
      {  { -1.0, -1.0, -1.0 }, // 0--0
         { -1.0, -1.0, +1.0 }, // 1--1
         { -1.0, +1.0, -1.0 }, // 2--2
         { -1.0, +1.0, +1.0 }, // 3--3
         { +1.0, -1.0, -1.0 }, // 4--4
         { +1.0, -1.0, +1.0 }, // 5--5
         { +1.0, +1.0, -1.0 }, // 6--6
         { +1.0, +1.0, +1.0 }, // 7--7
         { -1.0, -1.0, -1.0 }, // 0--8
         { -1.0, -1.0, +1.0 }, // 1--9
         { -1.0, +1.0, -1.0 }, // 2--10
         { -1.0, +1.0, +1.0 }, // 3--11
         { +1.0, -1.0, -1.0 }, // 4--12
         { +1.0, -1.0, +1.0 }, // 5--13
         { +1.0, +1.0, -1.0 }, // 6--14
         { +1.0, +1.0, +1.0 }, // 7--15
         { -1.0, -1.0, -1.0 }, // 0--16
         { -1.0, -1.0, +1.0 }, // 1--17
         { -1.0, +1.0, -1.0 }, // 2--18
         { -1.0, +1.0, +1.0 }, // 3--19
         { +1.0, -1.0, -1.0 }, // 4--20
         { +1.0, -1.0, +1.0 }, // 5--21
         { +1.0, +1.0, -1.0 }, // 6--22
         { +1.0, +1.0, +1.0 }  // 7--23
      } ;

   triangulos =
      {  {0,1,3}, {0,3,2}, // X-
         {4,7,5}, {4,6,7}, // X+ 

         //{0,5,1}, {0,4,5}, // Y-
         //{2,3,7}, {2,7,6}, // Y+

         //{0,6,4}, {0,2,6}, // Z-
         //{1,5,7}, {1,7,3}  // Z+
         // Simplemente mantengo el orden de cubo 8 vértices
         // añadiendo 8 o 16 para conectar octetos de vértices
         // distintos
         {8,13,9}, {8,12,13},    // Y- (+8)
         {10,11,15}, {10,15,14}, // Y+ (+8)

         {16,22,20}, {16,18,22}, // Z- (+16)
         {17,21,23}, {17,23,19}  // Z+ (+16)
      } ;
   
   cc_tt_ver = 
   {
         // Se tiene que hacer con dibujo, mapear coordenadas
         // teniendo en cuenta el octeto de vertices que se usa para la cara
         // Invertimos verticalmente !!! cosa de librerias y no se que
         // Es decir en ordenadas intercambiar 0 y 1
         // X- y X+
         { 0.0, 1.0}, // 0--0
         { 1.0, 1.0}, // 1--1
         { 0.0, 0.0}, // 2--2
         { 1.0, 0.0}, // 3--3
         { 1.0, 1.0}, // 4--4
         { 0.0, 1.0}, // 5--5
         { 1.0, 0.0}, // 6--6
         { 0.0, 0.0}, // 7--7
         // Y- y Y+
         { 0.0, 1.0}, // 0--8
         { 0.0, 0.0}, // 1--9
         { 0.0, 0.0}, // 2--10
         { 0.0, 1.0}, // 3--11
         { 1.0, 1.0}, // 4--12
         { 1.0, 0.0}, // 5--13
         { 1.0, 0.0}, // 6--14
         { 1.0, 1.0}, // 7--15
         // Z- y Z+
         { 1.0, 1.0}, // 0--16
         { 0.0, 1.0}, // 1--17
         { 1.0, 0.0}, // 2--18
         { 0.0, 0.0}, // 3--19
         { 0.0, 1.0}, // 4--20
         { 1.0, 1.0}, // 5--21
         { 0.0, 0.0}, // 6--22
         { 1.0, 0.0}  // 7--23
   };

   calcularNormales();
}



//!!!!!!!!!!!!!!! Recordar enumeración coherente de vértices
// Desde fuera tiene que verse en sentido antihorario, cuidado al enumerar
// lo que se ve detrás