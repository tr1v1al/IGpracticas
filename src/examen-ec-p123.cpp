#include "examen-ec-p123.h"
using namespace std;

P1MallaPiramide::P1MallaPiramide()
:  MallaInd( "Examen p1" )
{
   vertices = {
      {+0.0, +0.0, +0.0},  // 0) centro
      {+1.0, +0.0, +1.0}, // 1) Base, antihoraria
      {+1.0, +0.0, -1.0}, // 2)
      {-1.0, +0.0, -1.0}, // 3)
      {-1.0, +0.0, +1.0}, // 4)
      {+0.0, +2.0, +0.0} // 5)
   };

   triangulos = {
      {0, 2, 1}, {0, 3, 2}, {0, 4, 3}, {0, 1, 4},
      {1, 5, 4}, {1, 2, 5}, {3, 5, 2}, {4, 5, 3}
   };
    
    col_ver = {
      {+0.6, +0.0, +0.1},  // 0) centro
      {+1.0, +1.0, +0.0}, // 1) Base, antihoraria
      {+1.0, +1.0, +0.0}, // 2)
      {+1.0, +1.0, +0.0}, // 3)
      {+1.0, +1.0, +0.0}, // 4)
      {+0.0, +1.0, +1.0} // 5)        
    };
}

P2Barrido::P2Barrido(unsigned m, unsigned n)
:  MallaInd( "Examen p2" )
{
    assert(m > 2);
    assert(n > 2);
    // Perfil
    using namespace glm;
    float angle;
    vector<glm::vec3> perfil;
    vec3 curr, aux_tr;
    vec4 rot_point, aux;

    // Rotamos alrededor del eje z
    angle = 180.0/(m-1);
    mat4 rot_mat = rotate(radians(angle), vec3(0, 0, +1.0));
    perfil.push_back(glm::vec3(0, -1.0, 0));

    // Generamos perfil rotando un punto
    for (int i = 1; i < m; ++i) {
        curr = perfil[i-1];
        aux = {curr.x, curr.y, curr.z, 1};
        rot_point = rot_mat * aux;
        curr = {rot_point.x, rot_point.y, rot_point.z};
        perfil.push_back(curr);
    }

    // Recorro instancias
    int sz = perfil.size();

    for (int i = 0; i < sz; ++i) {
        perfil[i].z += 1.0;
    }   
    float despl_z;
    for (unsigned int i = 0; i < n; ++i) {
        // Calculo ángulo de rotación en grados y su matriz
        despl_z = -2.0*i/n;
        mat4 trans_mat = translate(vec3(+0.0, +0.0, despl_z));

        // Recorro perfil
        for (int j = 0; j < sz; ++j) {
            curr = perfil[j];
            aux = {curr.x,curr.y,curr.z,1};
            rot_point = trans_mat * aux;
            curr = {rot_point.x, rot_point.y, rot_point.z};
            // Añado punto
            vertices.push_back(curr);
        }
    }

    // Triángulos
    // Recorro instancias
    int k = 0;
    for (unsigned int i = 0; i < n-1; ++i) {
        // Recorro perfil
        for (int j = 0; j < sz-1; ++j) {
            // Índice del punto del perfil de la instancia dada
            k = i*sz+j;
            // Añado triángulos
            triangulos.push_back(vec3(k, k+sz, k+sz+1));
            triangulos.push_back(vec3(k, k+sz+1, k+1));
        }
        triangulos.push_back(vec3(i*sz, (i+1)*sz, (i+1)*sz-1));
        triangulos.push_back(vec3((i+1)*sz, (i+2)*sz-1, (i+1)*sz-1));
    }
}


P3Mesa::P3Mesa() {
    using namespace glm;
    ponerNombre("Examen p3");
    NodoGrafoEscena * pata = new NodoGrafoEscena();
    pata->agregar(translate(vec3(+0.5, +0.3, +0.5)));
    pata->agregar(scale(vec3(+0.05, +0.3, +0.05)));
    pata->agregar(new Cubo());

    NodoGrafoEscena * tabla = new NodoGrafoEscena();
    tabla->agregar(translate(vec3(+0.0, +0.65, +0.0)));
    tabla->agregar(scale(vec3(+0.55, +0.05, +0.55)));
    tabla->agregar(new Cubo());

    NodoGrafoEscena * patas = new NodoGrafoEscena();
    unsigned int ind_esc = patas->agregar(scale(vec3(+1.0, +1.0, +1.0)));
    patas->agregar(translate(vec3(+0.0, +0.0, -1.0)));
    patas->agregar(pata);
    patas->agregar(translate(vec3(+0.0, +0.0, +1.0)));
    patas->agregar(pata);
    patas->agregar(translate(vec3(-1.0, +0.0, +0.0)));
    patas->agregar(pata);
    patas->agregar(translate(vec3(+0.0, +0.0, -1.0)));
    patas->agregar(pata);

    agregar(tabla);
    agregar(patas);
    pm_scale = patas->leerPtrMatriz(ind_esc);
}

unsigned P3Mesa::leerNumParametros() const {
   return NUM_PARAMS;
}

void P3Mesa::actualizarEstadoParametro(const unsigned iParam, const float t_sec) {
   switch (iParam)
   {
   case 0:
      //fijarEscalado();
      break;
   case 1:
      //fijarAnguloGiro(angulo*t_sec);
      fijarEscalado(1+0.5*sin(t_sec));
      break;
   default:
      break;
   }
}

void P3Mesa::fijarAnguloGiro(const float alpha){
   //*pm_rot_alpha = rotate(radians(alpha), vec3(+0.0, +1.0, +0.0));
}

void P3Mesa::fijarTranslacion(const float alpha){
   //*pm_rot_alpha = rotate(radians(alpha), vec3(+0.0, +1.0, +0.0));
}

void P3Mesa::fijarEscalado(const float alpha){
    using namespace glm;
   *pm_scale = scale(vec3(+1.0, alpha, +1.0));
}

