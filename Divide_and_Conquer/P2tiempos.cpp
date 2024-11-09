#include <iostream>
#include <fstream> //para la lectura del fichero
#include <chrono>
using namespace std;

//constante global
const int K=10;

/*
* @brief devuelve true si el puntoA pasado como argumento domina al segundo que se pasa (puntoB)
*/
bool AdominaaB (int puntoA[K], int puntoB[K]){
    bool Adomina = true, al_menos_uno_mayor = false;

    //No tiene sentido seguir el bucle si ya se sabe que A no domina a B
    for (int i=0; i<K && Adomina; i++){
        //Si hay alguna coordenada de A < que la de B, ya A no puede dominar
        if (puntoA[i] < puntoB[i]) Adomina = false;
        //Si al_menos_uno_mayor ya es verdadero no hay que volver a actualizar
        if (!al_menos_uno_mayor && puntoA[i] > puntoB[i]) al_menos_uno_mayor = true;
    }

    //Solo devuelve true si se cumplen ambas (true)
    return (Adomina && al_menos_uno_mayor);
}


/*
* @brief devuelve true si un punto no es dominado por ningún otro del conjunto
* Un punto es noDominado si AdominaB devuelve siempre false siendo B el punto a
* comprobar y A todos los demás del conjunto
* Un punto puede ser noDominado
*/
bool noDominado (int conjunto_puntos[][K], int puntoB[K], int N){
    bool p_no_dominado=true; //se parte de que es no dominado
    //Recorremos todos los puntos comparando
    for (int i=0; i<N && p_no_dominado; i++){ //no tiene sentido seguir evaluando puntos si ya sabemos que no es no dominado
        if (AdominaaB(conjunto_puntos[i], puntoB)) p_no_dominado = false; //si es dominado
    }
    return p_no_dominado;
}



/*
* @brief devuelve los puntos no dominados
*/
void conjuntoNoDominados (int conjunto_puntos[][K], int N){
    //Comprueba si cada uno de los puntos del conjunto es dominado o no, y escribe por pantalla el identificador de los no dominados
    cout << "Los puntos no dominados son: " << endl;
    for (int i=0; i<N; i++){
        if (noDominado(conjunto_puntos, conjunto_puntos[i], N)){
            cout << i << " ";
        }
    }
    cout << endl;
}


int readFromFile(const char fichero[], int conjunto_puntos[][K], int N) {

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

    }
    else cout << "Error en la lectura del fichero" << endl;

    //Cierro fichero
    fi.close();
    return 0;
}



int main(){

    chrono::time_point<std::chrono::high_resolution_clock> t0, tf; // Para medir el tiempo de ejecución
    unsigned long duration;

    //Obtenemos el número de puntos del conjunto y lo creamos
    int N;
    cout << "Introduce cuántos puntos va a haber: " << endl;
    cin >> N;
    int conjunto_puntos[N][K];

    //Inicializar el conjunto a través de un fichero de texto puntos.txt
    readFromFile("puntos.txt", conjunto_puntos, N);

    // Comenzamos a medir el tiempo
    t0= std::chrono::high_resolution_clock::now();

    //Llamar al algoritmo principal
    conjuntoNoDominados(conjunto_puntos, N);

    // Terminamos de medir el tiempo
    tf= std::chrono::high_resolution_clock::now();

    duration= std::chrono::duration_cast<std::chrono::microseconds>(tf - t0).count();

    //Mostramos los tiempos obtenidos
    cout << "Tiempo con el tamaño N=" << N << " es: " << duration << "ms" << endl;

    return 0;
}