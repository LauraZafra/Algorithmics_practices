#include <iostream>
#include <fstream>
using namespace std;

//Capacidad de la mochila
const int M = 12;
//Número de objetos
const int n = 6;

void lecturaObjetos(const char fichero[], int b[], int w[], int n){
    ifstream fi;
    //Abro fichero
    fi.open(fichero);

    if (fi) { //si no hay errores
        cout << endl << "Leyendo el fichero " << fichero << endl;
        int beneficio, peso;


        b[0] = 0;
        w[0] = 0;

        //Leo cada pareja de valores
        for (int i=1; i<n; i++){
            fi >> beneficio >> peso;
            b[i] = beneficio;
            w[i] = peso;
        }
    }
    else cout << "Error en la lectura del fichero" << endl;

    //Cierro fichero
    fi.close();
}

int Mochila(int n, int M, int b[], int w[]){
    int T[n][M];

    //Inicialización
    //Si no hay elementos
    for (int j = 0; j<M; j++){
        T[0][j]=0;
    }
    //Si la capacidad es 0
    for (int i = 0; i<n; i++){
        T[i][0]=0;
    }


    for(int j=1; j<M; j++){
        for(int i=1; i<n; i++){
            //Si la capacidad disponible es menor que el peso del objeto que se considera
            if(j<w[i]){
                T[i][j]=T[i-1][j];
            }
            else{
                int izda= T[i-1][j];
                int dcha= b[i] + T[i-1][j-w[i]];

                if(izda>dcha){
                    //No se coge el objeto considerado
                    T[i][j]=izda;
                }
                else{
                    //Se coge el objeto considerado
                    T[i][j]=dcha;
                }
            }
        }
    }

    return T[n-1][M-1];
}

int main() {

    //Características de los objetos
    int b[n]; //valor
    int w[n]; //peso

    //Función de lectura
    lecturaObjetos("ejemplocorto.txt", b, w, n);

    //Algoritmo de la mochila
    cout << "El beneficio máximo que se puede obtener es: ";
    cout << Mochila(n, M, b, w) << endl;
}