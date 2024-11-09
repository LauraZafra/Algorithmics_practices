#include <iostream>
#include <fstream> //para la lectura del fichero
using namespace std;

//constante global
const int K=10;

/*
* @brief devuelve true si el puntoA pasado como argumento domina al segundo que se pasa (puntoB)
*/
bool AdominaaB (int * puntoA, int * puntoB){
    bool Adomina = true, al_menos_uno_mayor = false;

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



/*
* @brief devuelve true si un punto no es dominado por ningún otro del conjunto
* Un punto es noDominado si AdominaB devuelve siempre false siendo B el punto a
* comprobar y A todos los demás del conjunto
*/
bool noDominado (int ** conjunto_puntos, int * puntoB, int N){
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
int conjuntoNoDominados (int **conjunto_puntos, int inicio ,int final){
    // Caso recursivo
    if (final-inicio>1){ //caso más probable: lo ponemos primero
        
        // 1. Dividir el conjunto de puntos en dos mitades (aproximadamente) iguales
        int mitad = N/2;

        //Inicialización de subconjuntos parciales
        int  * * conjunto_puntos1 = new int * [mitad];
        int  * * conjunto_puntos2 = new int * [N-mitad];
        //si N es impar el conjunto 2 tiene un elemento más que el conjunto 1

        for (int i=0; i<mitad; i++){ //i va de 0 a mitad del conjunto inicial, N/2 elementos
            conjunto_puntos1[i] = conjunto_puntos[i];
        } 
        
        for (int i=mitad; i<N; i++){ //i va de mitad a final del conjunto, N-mitad elementos
            conjunto_puntos2[i-mitad] = conjunto_puntos[i];
        }
        


        // 2. Aplicar el algoritmo de búsqueda de puntos no dominados a cada mitad
        int s1 = conjuntoNoDominados(conjunto_puntos1, mitad); //los no dominados de la primera mitad
        int s2 = conjuntoNoDominados(conjunto_puntos2, N-mitad); //los no dominados de la segunda mitad
        int size_max = s1+s2; //como máximo todos los puntos van a ser no dominados entre los dos subconjuntos

        
        // 3. Combinar los conjuntos de puntos no dominados de ambas mitades
        //Sé que los puntos de conjunto_puntos1 son no dominados entre ellos, lo mismo con conjunto_puntos2...
        //pero puede haber puntos de conjunto_puntos1 dominados por otros de conjunto_puntos2...
        //y puntos de conjunto_puntos2 dominados por otros de conjunto_puntos1

        //Hay que construir un vector que contenga los puntos que no estén dominados por ningún otro,...
        //ya sea del conjunto 1 o 2. Este vector será devuelto por parámetro, por lo que el vector resultado...
        //de la combinación debe ser conjunto_puntos

        //Debemos comprobar cada punto del conjunto1 con todos el conjunto2, y cada punto del conjunto2...
        //con todos el conjunto 1. Guardamos los no dominados en un vector/puntero auxiliar (aux) y asignamos...
        //el puntero aux a conjunto_puntos para devolver este modificado

        //Crear puntero auxiliar redimensionable
        //int  * * aux = new int * [size_max]; 
        //int size=0;

        
        //Comparación de cada uno de los puntos de conjunto_puntos1 con conjunto_puntos2
        for (int i=0; i<mitad; i++){
            //Devuelve un bool, si es true hay que añadirlo al vector auxiliar
            if (noDominado(conjunto_puntos2, conjunto_puntos1[i], mitad)){
                conjunto_puntos[][] = null;
                //aux[size] = conjunto_puntos1[i];
                //for (int j=0; j<K; j++) cout << aux[size][j] << " ";
                //cout << endl;
                //size++;
            }
        }
        cout << "Fin primera mitad: conjunto1" << endl;

        //Comparación de cada uno de los puntos de conjunto_puntos2 con conjunto_puntos1
        for (int i=0; i<N-mitad; i++){
            //Devuelve un bool, si es true hay que añadirlo al vector auxiliar
            if (noDominado(conjunto_puntos1, conjunto_puntos2[i], N-mitad)){
                conjunto_puntos[][] = null;
                // aux[size] = conjunto_puntos2[i];
                // for (int j=0; j<K; j++) cout << aux[size][j] << " ";
                // cout << endl;
                // size++;
            }
        }
        cout << "Fin segunda mitad: conjunto2" << endl;

        //Acabamos con los subconjuntos y los liberamos
        delete [] conjunto_puntos1;
        delete [] conjunto_puntos2;

        //Liberar las posiciones que no se usan del auxiliar, las últimas: de size a N
        /*
        for (int i=size; i<size_max; i++){
            delete [] aux[i];
        }
        */

        /*
        //Primero hay que liberar aux completamente y ya después se puede asignar
        delete [] aux;
        conjunto_puntos = aux;
        */

        return size;
    }

    //Caso base: N=1
    else return 0;
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
                fi >> coordenada; //>> separador;
                conjunto_puntos[i][j] = coordenada;
                cout << conjunto_puntos[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
    else cout << "Error en la lectura del fichero" << endl;
    
    //Cierro fichero
    fi.close();
}



int main(){

    //Obtenemos el número de puntos del conjunto y lo creamos
    int N; //nº puntos
    cout << endl << "Introduce cuántos puntos va a haber: " << endl;
    cin >> N;
    
    //conjunto_puntos será un puntero que indique el conjunto de puntos entrada y de salida a la vez... 
    //Devolverá un conjunto <= al inicial, pero no sabemos el tamaño que tendrá, por lo que usaremos...
    //memoria dinámica
    int  * * conjunto_puntos = new int * [N];
    for (int i=0; i<N; i++) conjunto_puntos[i] = new int[K];

    //Inicializar el conjunto a través de un fichero de texto puntos.txt
    readFromFile("puntos.txt", conjunto_puntos, N);
    
    //Llamar al algoritmo principal
    cout << "Buscando los puntos no dominados..." << endl;
    int size = conjuntoNoDominados(conjunto_puntos, N);

    cout << "Los puntos no dominados son:" << endl;
    //Como hemos usado un puntero, debemos liberar memoria
    for (int i=0; i<size; i++){
        for (int j=0; j<K; j++) cout << conjunto_puntos[i][j] << " ";
        cout << endl;
        delete [] conjunto_puntos[i];
    }
    delete [] conjunto_puntos;

    return 0;
}