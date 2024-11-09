#include <iostream>
#include <fstream> //para la lectura del fichero
#include <cmath> //uso de floor en el redondeo
#include <chrono> //para calcular el tiempo
using namespace std;

//constante global
const int K=10;

/**
* @brief devuelve true si el puntoA pasado como argumento domina al segundo que se pasa (puntoB)
*/
bool AdominaaB (int * puntoA, int * puntoB){
    bool Adomina = true, al_menos_uno_mayor = false;
    if (puntoA == puntoB) return false;
    //No tiene sentido seguir el bucle si ya se sabe que A no domina a B
    for (int i=0; i<K && Adomina; i++){
        //Si hay alguna coordenada de A < que la de B ya no puede dominar
        if (puntoA[i] < puntoB[i]) Adomina = false;
        //Si al_menos_uno_mayor ya es verdadero no hay que volver a actualizar
        if (!al_menos_uno_mayor && puntoA[i] > puntoB[i]) al_menos_uno_mayor = true;
    }
    //Solo devuelve true si se cumplen ambas (true)
    return (Adomina && al_menos_uno_mayor);
}



/**
* @brief devuelve true si un punto no es dominado por ningún otro del conjunto
* Un punto es noDominado si AdominaB devuelve siempre false siendo B el punto a
* comprobar y A todos los demás del conjunto
*/
bool noDominado (int ** conjunto_puntos, int * puntoB, int inicio, int final){
    if (puntoB == NULL) return false;

    bool p_no_dominado=true; //se parte de que es no dominado
    //Recorremos todos los puntos comparando
    //no tiene sentido seguir evaluando puntos si ya sabemos que no es no dominado
    for (int i=inicio; i<final && p_no_dominado; i++){
        if (conjunto_puntos[i] != NULL && AdominaaB(conjunto_puntos[i], puntoB)) p_no_dominado = false; //si es dominado
    }
    return p_no_dominado;
}



/**
* @brief devuelve los puntos no dominados
*/
void conjuntoNoDominados (int **&conjunto_puntos, int inicio, int final){
    // Caso recursivo
    if (final-inicio>1){ //caso más probable: lo ponemos primero

        // 1. Dividir el conjunto de puntos en dos mitades (aproximadamente) iguales
        int mitad = floor((inicio+final)/2.0);
        //si N es impar el conjunto 2 tiene un elemento más que el conjunto 1


        // 2. Aplicar el algoritmo de búsqueda de puntos no dominados a cada mitad
        conjuntoNoDominados(conjunto_puntos, inicio,mitad); //los no dominados de la primera mitad
        conjuntoNoDominados(conjunto_puntos, mitad, final); //los no dominados de la segunda mitad


        // 3. Combinar los conjuntos de puntos no dominados de ambas mitades
        //Sé que los puntos de conjunto_puntos1 son no dominados entre ellos, lo mismo con conjunto_puntos2...
        //pero puede haber puntos de conjunto_puntos1 dominados por otros de conjunto_puntos2...
        //y puntos de conjunto_puntos2 dominados por otros de conjunto_puntos1


        //Comparación de cada uno de los puntos de conjunto_puntos de la primera mitad con la segunda mitad
        for (int i=0; i<mitad; i++){
            //Devuelve un bool, si es true ese punto se pone a null en nuestro vector
            //Si no se ha confirmado ya que el punto es dominado, comprobamos si está dominado por alguno del otro
            // conjunto. Si está nominado (no no Dominado = Dominado), se elimina
            if (conjunto_puntos[i] != NULL && !noDominado(conjunto_puntos, conjunto_puntos[i], mitad, final)) conjunto_puntos[i] = NULL;
        }


        //Comparación de cada uno de los puntos de conjunto_puntos de la segunda mitad con la primera mitad
        for (int i=mitad; i<final; i++){
            //Devuelve un bool, si es true ese punto se pone a null en nuestro vector
            //Si no se ha confirmado ya que el punto es dominado, comprobamos si está dominado por alguno del otro
            // conjunto. Si está nominado (no no Dominado = Dominado), se elimina
            if (conjunto_puntos[i] != NULL && !noDominado(conjunto_puntos, conjunto_puntos[i], inicio, mitad)) conjunto_puntos[i] = NULL;
        }
    }
    //Caso base: final - inicio = 1 <-- hay un solo punto, no se divide
}


void readFromFile(const char fichero[], int **conjunto_puntos, int N) {

    ifstream fi;
    //Abro fichero
    fi.open(fichero);

    if (fi) { //si no hay errores
        cout << endl << "Leyendo el fichero " << fichero << endl;
        int coordenada;
        char separador;

        //Leo cada punto
        for (int i=0; i<N; i++){
            //Leo las coordenadas de cada punto
            for (int j=0; j<K; j++){
                fi >> coordenada >> separador;
                conjunto_puntos[i][j] = coordenada;
            }
        }
        cout << endl;
    }
    else cout << "Error en la lectura del fichero" << endl;

    //Cierro fichero
    fi.close();
}



int main(){
    //Para medir tiempos
    chrono::time_point<std::chrono::high_resolution_clock> t0, tf;
    unsigned long duration;

    //Obtenemos el número de puntos del conjunto y lo creamos
    int N; //nº puntos
    int cero=0;
    cout << endl << "Introduce cuántos puntos va a haber: " << endl;
    cin >> N;
    if (N<=0){
        cout << "No hay puntos que evaluar" << endl;
        return 0;
    }

    //conjunto_puntos será un puntero que indique el conjunto de puntos entrada y de salida a la vez...
    int  * * conjunto_puntos = new int * [N];
    for (int i=0; i<N; i++) conjunto_puntos[i] = new int[K];

    //Inicializar el conjunto a través de un fichero de texto puntos.txt
    readFromFile("puntos.txt", conjunto_puntos, N);

    // Comenzamos a medir el tiempo
    t0= std::chrono::high_resolution_clock::now();

    //Llamar al algoritmo principal
    cout << "Buscando los puntos no dominados..." << endl;
    conjuntoNoDominados(conjunto_puntos, cero, N);

    // Terminamos de medir el tiempo
    tf= std::chrono::high_resolution_clock::now();

    cout << "Los puntos no dominados son:" << endl;

    for (int i=0; i<N; i++){
        if (conjunto_puntos[i] != NULL){
            cout << i << " con los elementos\t ";
            for (int j=0; j<K; j++){
                cout << conjunto_puntos[i][j] << " ";
            }
        }
        cout << endl;
    }

    //Como hemos usado un puntero, debemos liberar memoria
    for (int i=0; i<N; i++){
        delete [] conjunto_puntos[i]; //borrar cada punto
    }
    delete [] conjunto_puntos; //borrar el puntero de puntos

    //Medimos la duracion en microsegundos
    duration= std::chrono::duration_cast<std::chrono::microseconds>(tf - t0).count();

    //Mostramos los tiempos obtenidos
    cout << "Tiempo con el tamaño N=" << N << " es: " << duration << "ms" << endl;

    return 0;
}