// Nombre: Eduardo, Apellidos: Rodríguez Cao, Titulación: DGIIM.
// email: edurodcao@correo.ugr.es, DNI o pasaporte: 54359083D

#include "modelo-jer.h"
using namespace glm;

Mast::Mast() {
    // Base de la torre
    NodoGrafoEscena * base = new NodoGrafoEscena();
    base->agregar(scale(vec3(+0.07, +0.025, +0.07)));
    base->agregar(new Cubo24());
    agregar(base);

    // Barras verticales
    NodoGrafoEscena * vertical = new NodoGrafoEscena();
    vertical->agregar(translate(vec3(+0.035, +0.5, +0.035)));
    vertical->agregar(scale(vec3(+0.005, +0.5, +0.005)));
    vertical->agregar(new Cubo24());

    for (int i = 0; i < 4; ++i) {
        agregar(rotate(radians(90.0f), vec3(+0.0, +1.0, +0.0)));
        agregar(vertical);
    }

    // Barras horizontales y diagonales
    NodoGrafoEscena * horizontalM = new NodoGrafoEscena();
    horizontalM->agregar(translate(vec3(+0.0, +0.0, +0.035)));
    horizontalM->agregar(scale(vec3(+0.035, +0.005, +0.005)));
    horizontalM->agregar(new Cubo24());

    NodoGrafoEscena * diagonalM = new NodoGrafoEscena();
    diagonalM->agregar(translate(vec3(+0.0, +0.05, +0.035)));
    diagonalM->agregar(rotate(radians(45.0f), vec3(+0.0, +0.0, +1.0)));
    diagonalM->agregar(scale(vec3(+0.05, +0.005, +0.005)));
    diagonalM->agregar(new Cubo24());
    
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 4; ++j) {
            agregar(rotate(radians(90.0f), vec3(+0.0, +1.0, +0.0)));
            agregar(horizontalM);
            agregar(diagonalM);
        }
        agregar(translate(vec3(+0.0, +0.1, +0.0)));
    }

    agregar(translate(vec3(+0.0, -1.0, +0.0)));

    // Plataforma
    NodoGrafoEscena * platform = new NodoGrafoEscena();
    platform->agregar(translate(vec3(+0.0, +1.02, +0.0)));
    platform->agregar(scale(vec3(+0.04, +0.025, +0.04)));
    platform->agregar(new Cubo24());
    agregar(platform);

    ponerColor(vec3(+1.0, +1.0, +0.0));
    base->ponerColor(vec3(+1.0, +0.0, +0.0));
}

Turntable::Turntable() {
    NodoGrafoEscena * cilinderTT = new NodoGrafoEscena();
    cilinderTT->agregar(translate(vec3(+0.0, +1.04, +0.0)));
    cilinderTT->agregar(scale(vec3(+0.04, +0.025, +0.04)));
    cilinderTT->agregar(new Cilindro(10, 20));
    agregar(cilinderTT);
    cilinderTT->ponerColor(vec3(+1.0, +0.0, +1.0));

    NodoGrafoEscena * cabin = new NodoGrafoEscena();
    cabin->agregar(translate(vec3(+0.02, +1.09, +0.0)));
    cabin->agregar(scale(vec3(+0.06, +0.03, +0.04)));
    cabin->agregar(new Cubo24());
    agregar(cabin);
    cabin->ponerColor(vec3(+1.0, +0.0, +0.0));
}

Jib::Jib() {

    NodoGrafoEscena * horizontal_bot = new NodoGrafoEscena();
    horizontal_bot->agregar(scale(vec3(+0.5, +0.005, +0.005)));
    horizontal_bot->agregar(new Cubo24());
    NodoGrafoEscena * horizontal_top = new NodoGrafoEscena();
    horizontal_top->agregar(translate(vec3(+0.0, +0.05, +0.0)));
    horizontal_top->agregar(scale(vec3(+0.45, +0.005, +0.005)));
    horizontal_top->agregar(new Cubo24());
    NodoGrafoEscena * horizontal_perp = new NodoGrafoEscena();
    horizontal_perp->agregar(translate(vec3(-0.4, +0.0, -0.025)));
    horizontal_perp->agregar(rotate(radians(90.0f), vec3(+0.0, +1.0, +0.0)));
    horizontal_perp->agregar(scale(vec3(+0.025, +0.005, +0.005)));
    horizontal_perp->agregar(new Cubo24());

    NodoGrafoEscena * diagonal1J = new NodoGrafoEscena();
    diagonal1J->agregar(translate(vec3(-0.47, +0.025, +0.0)));
    diagonal1J->agregar(rotate(radians(45.0f), vec3(+0.0, +0.0, +1.0)));
    diagonal1J->agregar(scale(vec3(+0.035, +0.005, +0.005)));
    diagonal1J->agregar(new Cubo24());
    NodoGrafoEscena * diagonal2J = new NodoGrafoEscena();
    diagonal2J->agregar(translate(vec3(-0.425, +0.025, +0.0)));
    diagonal2J->agregar(rotate(radians(-45.0f), vec3(+0.0, +0.0, +1.0)));
    diagonal2J->agregar(scale(vec3(+0.035, +0.005, +0.005)));
    diagonal2J->agregar(new Cubo24());
    
    NodoGrafoEscena * side = new NodoGrafoEscena();
    for (int i = 0; i < 10; ++i) {
        side->agregar(translate(vec3(+0.1, +0.0, +0.0)));
        side->agregar(diagonal1J);
        side->agregar(diagonal2J);
    }
    side->agregar(translate(vec3(-0.9, +0.0, +0.0)));
    side->agregar(horizontal_bot);
    side->agregar(horizontal_top);  
    
    // Posicion final
    agregar(translate(vec3(+1.55, +1.13, +0.025)));

    // Barras perpendiculares
    for (int i = 0; i < 11; ++i) {
        agregar(translate(vec3(-0.1, +0.0, +0.0)));
        agregar(horizontal_perp);
    }
    agregar(rotate(radians(-30.0f), vec3(+1.0, +0.0, +0.0))); 
    agregar(side);
    agregar(translate(vec3(+0.0, +0.025, -0.043)));
    agregar(rotate(radians(60.0f), vec3(+1.0, +0.0, +0.0)));
    agregar(side);
    ponerColor(vec3(+1.0, +1.0, +0.0));
}

CounterJib::CounterJib() {
    NodoGrafoEscena * horizontalCJ = new NodoGrafoEscena();
    horizontalCJ->agregar(translate(vec3(+0.0, +0.0, +0.04)));
    horizontalCJ->agregar(scale(vec3(+0.25, +0.007, +0.007)));
    horizontalCJ->agregar(new Cubo24());

    NodoGrafoEscena * diagonal1CJ = new NodoGrafoEscena();
    diagonal1CJ->agregar(translate(vec3(-0.205, +0.0, +0.0)));
    diagonal1CJ->agregar(rotate(radians(45.0f), vec3(+0.0, +1.0, +0.0)));
    diagonal1CJ->agregar(scale(vec3(+0.055, +0.005, +0.005)));
    diagonal1CJ->agregar(new Cubo24());
    NodoGrafoEscena * diagonal2CJ = new NodoGrafoEscena();
    diagonal2CJ->agregar(translate(vec3(-0.13, +0.0, +0.0)));
    diagonal2CJ->agregar(rotate(radians(-45.0f), vec3(+0.0, +1.0, +0.0)));
    diagonal2CJ->agregar(scale(vec3(+0.055, +0.005, +0.005)));
    diagonal2CJ->agregar(new Cubo24());
    
    NodoGrafoEscena * ballast = new NodoGrafoEscena();
    ballast->agregar(translate(vec3(+0.045, -0.04, +0.0)));
    ballast->agregar(scale(vec3(+0.05, +0.06, +0.03)));
    ballast->agregar(new Cubo24());
    ballast->ponerColor(vec3(+0.75, +0.75, +0.75));

    agregar(translate(vec3(-0.45, +1.125, +0.0)));
    agregar(rotate(radians(180.0f), vec3(+0.0, +1.0, +0.0)));
    agregar(ballast);
    agregar(diagonal1CJ);
    agregar(diagonal2CJ);
    agregar(translate(vec3(-0.16, +0.0, +0.0)));
    agregar(diagonal1CJ);
    agregar(diagonal2CJ);
    agregar(horizontalCJ);
    agregar(translate(vec3(+0.0, +0.0, -0.08)));
    agregar(horizontalCJ);
    ponerColor(vec3(+1.0, +1.0, +0.0));
}

Apex::Apex() {
    agregar(translate(vec3(+0.01, +1.19, +0.0)));
    
    NodoGrafoEscena * diagonalApex = new NodoGrafoEscena();
    diagonalApex->agregar(translate(vec3(-0.02, +0.075, +0.02)));
    diagonalApex->agregar(rotate(radians(45.0f), vec3(+0.0, +1.0, +0.0)));
    diagonalApex->agregar(rotate(radians(79.0f), vec3(+0.0, +0.0, +1.0)));
    diagonalApex->agregar(scale(vec3(+0.16, +0.005, +0.005)));
    diagonalApex->agregar(new Cubo24());

    for (int i = 0; i < 4; ++i) {
        agregar(rotate(radians(90.0f), vec3(+0.0, +1.0, +0.0)));
        agregar(diagonalApex);
    }

    ponerColor(vec3(+1.0, +1.0, +0.0));
}

TowerCrane::TowerCrane() {

    Material * mat_steel_espec = new Material(new Textura("steel.jpg"), 0.8, 0.4, 1.0, 100);
    Material * mat_steel_dif_auto = new Material(new TexturaXY("steel.jpg"), 0.8, 1.0, 0.2, 100);
    Material * mat_no_tex = new Material(0.8, 0.8, 0.8, 100);

    NodoGrafoEscena * hook = new NodoGrafoEscena();
    unsigned hook_ind = hook->agregar(translate(vec3(+0.0, +0.0, +0.0)));
    hook->agregar(translate(vec3(+0.0, -0.505, +0.0)));
    hook->agregar(scale(vec3(+0.01, +0.01, +0.01)));
    hook->ponerColor(vec3(+1.0, +1.0, +1.0));
    hook->agregar(new Cubo24());

    NodoGrafoEscena * cable = new NodoGrafoEscena();
    unsigned cable_ind = cable->agregar(scale(vec3(+1.0, +1.0, +1.0)));
    cable->agregar(translate(vec3(+0.0, -0.5, +0.0)));
    cable->agregar(scale(vec3(+0.005, +0.5, +0.005)));
    cable->ponerColor(vec3(+1.0, +1.0, +1.0));
    cable->agregar(new Cilindro(20, 8));

    NodoGrafoEscena * trolley = new NodoGrafoEscena();
    trolley->agregar(mat_no_tex);
    unsigned trolley_ind = trolley->agregar(translate(vec3(+0.0, +0.0, +0.0)));
    trolley->agregar(translate(vec3(+0.5, +1.12, +0.0)));
    trolley->agregar(hook);
    trolley->agregar(cable);
    trolley->agregar(scale(vec3(+0.03, +0.01, +0.025)));
    trolley->ponerColor(vec3(+1.0, +0.0, +0.0));
    trolley->agregar(new Cubo24());   
    trolley->ponerIdentificador(1001);
    trolley->ponerNombre("trolley");

    NodoGrafoEscena * mast = new NodoGrafoEscena();
    mast->agregar(mat_steel_espec);
    mast->agregar(new Mast());
    mast->ponerIdentificador(1002);
    mast->ponerNombre("mast");

    NodoGrafoEscena * apex = new NodoGrafoEscena();
    apex->agregar(mat_steel_espec);
    apex->agregar(new Apex());
    apex->ponerIdentificador(1003);
    apex->ponerNombre("apex");

    NodoGrafoEscena * counter_jib = new NodoGrafoEscena();
    counter_jib->agregar(mat_steel_espec);
    counter_jib->agregar(new CounterJib());
    counter_jib->ponerIdentificador(1004);
    counter_jib->ponerNombre("counter jib");

    NodoGrafoEscena * jib = new NodoGrafoEscena();
    jib->agregar(mat_steel_espec);
    jib->agregar(new Jib());
    jib->ponerIdentificador(1005);
    jib->ponerNombre("jib");

    NodoGrafoEscena * turntable = new NodoGrafoEscena();
    turntable->agregar(mat_steel_dif_auto);
    turntable->agregar(new Turntable());
    turntable->ponerIdentificador(1006);
    turntable->ponerNombre("turntable");

    NodoGrafoEscena * upperPart = new NodoGrafoEscena();
    unsigned upper_ind = upperPart->agregar(rotate(radians(0.0f), vec3(+0.0, +1.0, +0.0)));
    upperPart->agregar(apex);
    upperPart->agregar(counter_jib);
    upperPart->agregar(jib);
    upperPart->agregar(turntable);
    upperPart->agregar(trolley);
    upperPart->ponerIdentificador(1007);
    upperPart->ponerNombre("parte superior");

    agregar(upperPart);
    agregar(mast);
    ponerIdentificador(1000);
    ponerNombre("grua grafo original");

    // Guardamos matrices
    rot_matrix = upperPart->leerPtrMatriz(upper_ind);
    cable_matrix = cable->leerPtrMatriz(cable_ind);
    hook_matrix = hook->leerPtrMatriz(hook_ind);
    trolley_matrix = trolley->leerPtrMatriz(trolley_ind);
}

unsigned TowerCrane::leerNumParametros() const{
    return NUM_PARAMETERS;
}

void TowerCrane::actualizarEstadoParametro(const unsigned iParam, const float t_sec){
    switch (iParam)
    {
    case 0:
        setAngle(angular_velocity*t_sec);
        break;
    case 1:
        setTrolleyTranslation(0.35*sin(trolley_velocity*t_sec));
        break;
    case 2:
        setCableScale(1 + 0.5*sin(cable_velocity*t_sec));
        // Multiplicamos por 0.5 porque es la longitud del cable
        setHookTranslation(0.5*0.5*sin(cable_velocity*t_sec));
        break;    
    default:
        break;
    }
}

void TowerCrane::setAngle(const float alpha){
    *rot_matrix = rotate(radians(alpha), vec3(+0.0, +1.0, +0.0));
}

void TowerCrane::setTrolleyTranslation(const float delta){
    *trolley_matrix = translate(vec3(delta, +0.0, +0.0));
}

void TowerCrane::setCableScale(const float delta){
    *cable_matrix = scale(vec3(+1.0, delta, +1.0));
}

void TowerCrane::setHookTranslation(const float delta){
    *hook_matrix = translate(vec3(+0.0, -delta, +0.0));
}