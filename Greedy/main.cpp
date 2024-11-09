#include<iostream>
#include<queue>
#include <fstream>

using namespace std;
const int NODE = 8;
bool grafo[NODE][NODE];
bool temp_grafo[NODE][NODE];


/**
 * Lee desde un fichero de texto el grafo
 * @param fichero
 * @param grafo
 */
void readFromFile(const char fichero[], bool grafo[NODE][NODE]) {

    int nodo;
    ifstream fi;
    //Abro fichero
    fi.open(fichero);

    if (fi) { //si no hay errores
        cout << endl << "Leyendo el fichero " << fichero << endl;
        //Inicialización de la matriz
        for (int i=0; i<NODE; i++){
            for (int j=0; j<NODE; j++){
                fi >> nodo;
                if (nodo == 1) grafo[i][j] = true;
                else grafo[i][j] = false;
            }
        }
    }
    else{
        cout << "Error en la lectura del fichero" << endl;
    }

    //Cierro fichero
    fi.close();
}

/**
 * Función que muestra que nodo hay que coger primero
 * @return El nodo por el que se empieza el circuito
 */
int primer_vertice(){
    for(int i = 0; i<NODE; i++){
        int g = 0;
        for(int j = 0; j<NODE; j++){
            if(temp_grafo[i][j])
                g++; //encuentra el grado de cada nodo
        }
        if(g % 2 != 0) // cuando el grado de los vértices es impar
            return i; //i es un nodo de grado impar
    }
    return 0; //cuando todos los vertices tienen grado par, se empieza por el nodo 0
}

/**
 * Esta función comprueba si una arista entre el vértice u y el vértice v es un puente en el grafo.
 * Para hacer esto, se comprueba si el vértice v tiene más de una arista conectada.
 * @param u un nodo del grafo
 * @param v otro nodo del grafo
 * @return Si v tiene mñas de una arista conectada, la arista no es un puente y se devuelve false. De lo contrario, la arista es un puente y se devuelve true.
 */
bool es_puente(int u, int v){
    int g = 0; // grado del nodo
    for(int i = 0; i<NODE; i++)
        if(temp_grafo[v][i])
            g++;
    if(g>1){
        return false; //el nodo no forma un puente
    }
    return true; //el nodo forma un puente
}

/**
 * Esta función cuenta el número de aristas en el grafo.
 * Recorre todos los elementos de la matriz de adyacencia tempGraph y cuenta el número de elementos no nulos.
 * @return Número de aristas en el grafo
 */
int numero_aristas(){
    int num = 0;
    for(int i = 0; i<NODE; i++)
        for(int j = i; j<NODE; j++)
            if(temp_grafo[i][j])
                num++;
    return num;
}



/**int grado_nodo(int nodo, int grafo[NODE][NODE]) {
    int grado = 0;
    for (int i = 0; i < NODE; i++) {
        if (grafo[nodo][i]) {
            grado++;
        }
    }
    return grado;
}
int encontrar_siguiente_nodo(int actual, int grafo[NODE][NODE]) {
    int siguiente = -1;
    int max_grado = -1;
    for (int i = 0; i < NODE; i++) {
        if (grafo[actual][i] && (siguiente == -1 || max_grado < grado_nodo(i, grafo))) {
            siguiente = i;
            max_grado = grado_nodo(i, grafo);
        }
    }
    return siguiente;
}**/





/**



/**
 * Esta función realiza el recorrido de Fleury en el grafo a partir del vértice de partida inicio.
 * La función recorre todos los vértices del grafo y comprueba si hay una arista que pueda ser eliminada sin formar un puente.
 * Si se encuentra una arista que cumple estas condiciones, se elimina y se avanza al siguiente vértice.
 * Este proceso continúa hasta que no hay más aristas disponibles para eliminar.
 * Durante el proceso, la función imprime los vértices visitados para formar el recorrido de Euler.
 * @param inicio el nodo en el que se empieza el recorrido
**/
queue<int> fleury(int inicio) {
    queue<int> solucion;
    int arista = numero_aristas();
    int u = inicio;

    while (arista > 0) {
        bool hay_camino_euleriano = false;

        for (int v = 0; v < NODE; v++) {
            if (temp_grafo[u][v] && (!es_puente(u, v) || arista == 1)) {
                hay_camino_euleriano = true;
                cout << u << "--" << v << " ";
                solucion.push(u);
                solucion.push(v);
                temp_grafo[u][v] = temp_grafo[v][u] = 0;
                arista--;
                u = v;
                break;
            }
        }

        if (!hay_camino_euleriano) {
            for (int v = 0; v < NODE; v++) {
                if (temp_grafo[u][v]) {
                    cout << u << "--" << v << " ";
                    solucion.push(u);
                    solucion.push(v);
                    temp_grafo[u][v] = temp_grafo[v][u] = 0;
                    arista--;
                    u = v;
                    break;
                }
            }
        }
    }
    return solucion;
}




int main(){
    queue<int> v; //CONJUNTO VACIO
    for(int i = 0; i<NODE; i++) //copiar el grafo principal al temp_grafo
        for(int j = 0; j<NODE; j++)
            temp_grafo[i][j] = grafo[i][j];
    cout << "El circuito de Euler sería: ";
    v=fleury(primer_vertice());

    cout << endl << "Cola: ";   // Mostrar todos los elementos del CONJUNTO-solucion
    while(!v.empty()){
        cout << v.front() << " -- ";
        v.pop();
    }
    cout << endl;

}
