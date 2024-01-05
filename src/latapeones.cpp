#include "latapeones.h"

Lata::Lata(string textura) {

    // Materiales y Texturas
    Textura * tex_pcue = new Textura(textura);
    // Todo es difuso especular, no se especifican los coeficientes
    // asi que pongo un poco de ambiental y bastante de difuso y especular
    Material * mat_cuerpo = new Material(tex_pcue, 0.2, 1.0, 1.0, 100.0);
    Material * mat_inf = new Material(0.2, 1.0, 1.00, 100.0);
    Material * mat_sup = new Material(0.2, 1.0, 1.00, 100.0);

    // Objetos
    MallaRevolPLY * cuerpo = new MallaRevolPLY("lata-pcue.ply", 100);
    MallaRevolPLY * inf = new MallaRevolPLY("lata-pinf.ply", 100);
    MallaRevolPLY * sup = new MallaRevolPLY("lata-psup.ply", 100);

    // Nodos
    NodoGrafoEscena * nodo_cuerpo = new NodoGrafoEscena();
    nodo_cuerpo->agregar(mat_cuerpo);
    nodo_cuerpo->agregar(cuerpo);

    NodoGrafoEscena * nodo_inf = new NodoGrafoEscena();
    nodo_inf->agregar(mat_inf);
    nodo_inf->agregar(inf);

    NodoGrafoEscena * nodo_sup = new NodoGrafoEscena();
    nodo_sup->agregar(mat_sup);
    nodo_sup->agregar(sup);

    agregar(nodo_cuerpo);
    agregar(nodo_inf);
    agregar(nodo_sup);
}

Peon::Peon() {
    using namespace glm;
    MallaRevolPLY * peon = new MallaRevolPLY("peon.ply", 30);
    unsigned ind = agregar(translate(vec3(+0.0, +0.0, +0.0)));
    agregar(scale(vec3(+0.2, +0.2, +0.2)));
    agregar(peon);
    mat = leerPtrMatriz(ind);
}

bool Peon::cuandoClick(const glm::vec3 & centro_wc) {
    using namespace glm;
    *mat = translate(vec3(+0.0, +0.0, +0.2*total));
    ++total;
    return true;
}

LataPeones::LataPeones() {

    using namespace glm;

    // Identificadores
    int id_lata_cola = 1, id_peon_madera = 2;
    int id_peon_blanco = 3, id_peon_negro = 4;

    // Materiales y Texturas
    // Puramente difuso
    Material * mat_blanco = new Material(0.0, 1.0, 0.0, 1.0);
    // Especular con poca reflexividad difusa
    Material * mat_negro = new Material(1.0, 0.2, 0.9, 5.0);
    // No se especifica, lo pongo a maximo, textura generada
    // automaticamente con TexturaXY
    Textura * tex_madera = new TexturaXY("text-madera.jpg");
    Material * mat_madera = new Material(tex_madera, 1.0, 1.0, 1.0, 100.0);

    // Objetos
    //MallaRevolPLY * peon = new MallaRevolPLY("peon.ply", 30);

    // Nodos
    NodoGrafoEscena * lata_cola = new Lata("lata-coke.jpg");
    lata_cola->ponerNombre("Lata de Coca-Cola");
    lata_cola->ponerIdentificador(id_lata_cola);

    NodoGrafoEscena * peon_madera = new Peon();
    peon_madera->ponerNombre("Peón de madera");
    peon_madera->ponerIdentificador(id_peon_madera);

    NodoGrafoEscena * peon_blanco = new Peon();
    peon_blanco->ponerNombre("Peón blanco");
    peon_blanco->ponerIdentificador(id_peon_blanco);

    NodoGrafoEscena * peon_negro = new Peon();
    peon_negro->ponerNombre("Peón negro");
    peon_negro->ponerIdentificador(id_peon_negro);

    NodoGrafoEscena * nodo_peon_madera = new NodoGrafoEscena();
    nodo_peon_madera->agregar(translate(vec3(+0.0, +0.3, +0.8)));
    nodo_peon_madera->agregar(mat_madera);
    nodo_peon_madera->agregar(peon_madera);

    NodoGrafoEscena * nodo_peon_blanco = new NodoGrafoEscena();
    nodo_peon_blanco->agregar(translate(vec3(+0.45, +0.3, +0.8)));
    nodo_peon_blanco->agregar(mat_blanco);
    nodo_peon_blanco->ponerColor({1,1,1});
    nodo_peon_blanco->agregar(peon_blanco);

    NodoGrafoEscena * nodo_peon_negro = new NodoGrafoEscena();
    nodo_peon_negro->agregar(translate(vec3(+0.9, +0.3, +0.8)));
    nodo_peon_negro->agregar(mat_negro);
    nodo_peon_negro->ponerColor({0,0,0});
    nodo_peon_negro->agregar(peon_negro);

    agregar(lata_cola);
    agregar(nodo_peon_madera);
    agregar(nodo_peon_blanco);
    agregar(nodo_peon_negro);
}

VariasLatasPeones::VariasLatasPeones() {
    using namespace glm;
    int id_lata_pepsi = 5, id_lata_ugr = 6;
    NodoGrafoEscena * lata_pepsi = new Lata("lata-pepsi.jpg");
    lata_pepsi->ponerNombre("Lata de Pepsi");
    lata_pepsi->ponerIdentificador(id_lata_pepsi);

    NodoGrafoEscena * lata_ugr = new Lata("window-icon.jpg");
    lata_ugr->ponerNombre("Lata de UGR");
    lata_ugr->ponerIdentificador(id_lata_ugr);

    agregar(new LataPeones());
    agregar(translate(vec3(0.9, 0.0, 0.0)));
    agregar(lata_pepsi);
    agregar(translate(vec3(0.9, 0.0, 0.0)));
    agregar(lata_ugr);
}