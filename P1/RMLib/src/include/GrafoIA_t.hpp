#pragma once
#include <utility>
#include <dll_t.hpp>
#include <NodeIA_t.hpp>
#include <EstadoIA_t.hpp>
#include <dll_node_t.hpp>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;


class GrafoIA_t {

private:

 vector<NodeIA_t> grafo_;
 vector<NodeIA_t> caminoSolucion_;
 int numeroNodos_;

 set<NodeIA_t> generados_; //aqui uso set para que siempre se ponga el primero el node de menor
                //f(n)
 set<NodeIA_t> inspeccionados_;

 bool solucionEncontrada_;

public:



    GrafoIA_t();
    GrafoIA_t(char nombrefichero[]);

    ~GrafoIA_t();

    bool actualizar(char nombrefichero[]);
    bool empy();
    void limpiar();    
    void limpiarArbol();
    bool aplicarHeuristica(char nombrefichero[]);
    bool aEstrella(int nodoOrigen, int nodoDestino);
    void generarSucesores(NodeIA_t &valor);
    bool existeNodo(int valor);
    ostream &mostrarCaminoSolucion(ostream &os);
    void crearCaminoSolucion(NodeIA_t &valor);
    friend ostream & operator << (ostream & os, GrafoIA_t &valor);

};



GrafoIA_t::GrafoIA_t():
    numeroNodos_(0),
    solucionEncontrada_(false)
{

}

GrafoIA_t::GrafoIA_t(char nombrefichero[])
{
       actualizar(nombrefichero);
}


GrafoIA_t::~GrafoIA_t()
{
    limpiar();
}


//muestra el grafo como un árbol.

ostream & operator << (ostream & os, GrafoIA_t & valor)
{

   for (int i=0; i < valor.grafo_.size(); i++)
   {
               os << endl << "El nodo : " << valor.grafo_[i].estado_.id_ << "es padre de : " << endl;

               for(int j=0; j< valor.grafo_[i].LS_.size();j++)
                   os << "Nodo : "<< valor.grafo_[i].LS_[j].first << " Costo: " <<valor.grafo_[i].LS_[j].second << endl;

    }


   return os;

}




bool GrafoIA_t::actualizar(char nombrefichero[])
{
    int i, j;
    double temp;
    ifstream fichero_grafo;
    NodeIA_t nodo;    
    pair<int,double> sucesor;

    limpiar(); //por si acaso ya se hab�a abierto alg�n fichero, libero las posibles bloques de memoria reservados.

     fichero_grafo.open(nombrefichero);

    if (fichero_grafo.is_open())
    {
        //version nodos entrelazados entre si

         fichero_grafo >> numeroNodos_;

         for (int i=1; i <= numeroNodos_;i++)
         {
                     nodo.estado_.id_=i;
                     nodo.padre_=i;
                     grafo_.push_back(nodo);  //creo todos los nodos de entrada.
          }

         for (int i=1; i < numeroNodos_;i++)
         {
             for (int j=0; j < numeroNodos_-i;j++)
            {
                fichero_grafo >> temp; //leo el costo

                if (temp!=-1) //si es igual a -1, valor escogido para infinito, o sea, inalcanzable.
                {
                    sucesor.first=i+j+1; //numero de nodo sucesor
                    sucesor.second=temp; //costo para llegar a ese nodo
                    //sucesor.second=NULL; //inicialmente el puntero hacia el sucesor es  nulo, este valor lo modifica solo el arbol de búsqueda.
                    grafo_[i-1].LS_.push_back(sucesor);

                    sucesor.first=i;     //indico al sucesor que este nodo es predecesor
                    grafo_[i+j].LS_.push_back(sucesor);

                }

            }

           }


         fichero_grafo.close();
        return true;

    }
      else return false;

}


bool GrafoIA_t::empy()
{

    if (numeroNodos_==0)
        return 1;
    else return 0;
}


bool  GrafoIA_t::aplicarHeuristica(char nombrefichero[])
{

    int temp;
    ifstream fichero_grafo;

    if (numeroNodos_==0)  //REGRESO PORQUE NO SE HA CARGADO TODAVÍA NINGUN GRAFO.
        return false;

        fichero_grafo.open(nombrefichero);

       if (fichero_grafo.is_open())
       {
           fichero_grafo >> temp;
            if (temp!=numeroNodos_)
            {
                fichero_grafo.close();
                return false;
            }
            for (int i=1; i <= numeroNodos_;i++)
                    fichero_grafo >> grafo_[i-1].valorHeuristico_;


            fichero_grafo.close();
           return true;

       }
       else return false;
}



bool GrafoIA_t::existeNodo(int valor)
{
   set<NodeIA_t>::iterator itNodo;
    bool encontrado=false;
    itNodo=generados_.begin();

    //busco nodo en generados
    while ((itNodo!=generados_.end()) && (encontrado==false))
          {
                if (itNodo->estado_.id_==valor)
                        encontrado=true;
                itNodo++;
           }

    itNodo=inspeccionados_.begin();

    //busco nodo en inspeccionados
    while ((itNodo!=inspeccionados_.end()) && (encontrado==false))
    {
            if (itNodo->estado_.id_==valor)
                encontrado=true;
          itNodo++;
    }

    return encontrado;
}


ostream & GrafoIA_t::mostrarCaminoSolucion(ostream &os)
{

    ///Aqui tengo que recorrer el vector caminoSolucion_ desde el final hacia el principio.

    for (int i=caminoSolucion_.size(); i >0;i--)
        os <<  "Nodo => " << caminoSolucion_[i-1].estado_ << " - Coste g(n)=> " << caminoSolucion_[i-1].costoCamino_ << " - Valor Heuristico => " << caminoSolucion_[i-1].valorHeuristico_ << endl;


return os;

}

void GrafoIA_t::crearCaminoSolucion(NodeIA_t &valor)
{
    NodeIA_t aux;
    set<NodeIA_t>::iterator itNodo;
    bool encontrado;

    caminoSolucion_.clear();
    caminoSolucion_.push_back(valor);    //introduzco primero nodo objetivo.    

    aux=valor;
    encontrado=false;

    while (aux.padre_!=0)
    {
        itNodo=inspeccionados_.begin();

        //busco nodo en inspeccionados
        while ((itNodo!=inspeccionados_.end()) && (encontrado==false))
        {
                if (itNodo->estado_.id_==aux.padre_)

                {
                       encontrado=true;
                       aux=*itNodo;
                       inspeccionados_.erase(itNodo); //borro este nodo de la lista.
                }

                else
                    itNodo++;
        }


         if (encontrado)
         {
             caminoSolucion_.push_back(aux);
             encontrado=false;

         }


    }

    inspeccionados_.clear(); //lebero ya  toda la memoria del set que no voy a utilizar.


}


void GrafoIA_t::generarSucesores(NodeIA_t &valor)
{

    for (int i=0; i < valor.LS_.size();i++)
    {
        if (!existeNodo(valor.LS_[i].first))
        {

            //si el nodo no existe en generados ni inspeccionados guardo al nodo sucesor como
            //predecesor y lo agreo a la lista de generados.

            set<NodeIA_t>::iterator itNodo;
            NodeIA_t nodoNuevo;

            nodoNuevo=grafo_[valor.LS_[i].first-1];
            nodoNuevo.costoCamino_=valor.costoCamino_+valor.LS_[i].second;
            nodoNuevo.costoCaminoMasHeuristico_=nodoNuevo.costoCamino_+nodoNuevo.valorHeuristico_;
            nodoNuevo.padre_=valor.estado_.id_;
            generados_.insert(nodoNuevo);

        }
    }
}


bool GrafoIA_t::aEstrella(int nodoOrigen, int nodoDestino)
{
 set<NodeIA_t>::iterator itNodo;
 NodeIA_t aux;

    if (numeroNodos_==0)  //REGRESO PORQUE NO SE HA CARGADO TODAVÍA NINGUN GRAFO.
        return false;

       limpiarArbol();


         aux=grafo_[nodoOrigen-1]; //nodo raiz
         aux.padre_=0; //este cero indica que es el nodo raiz, para cuando haya que volver sobre sus pasos para mostrar el camino obtenido

            generados_.insert(aux); //inserto origen en generados.
    do{

            if (generados_.empty())
                return false;

        itNodo=generados_.begin();
        aux=*itNodo;
        generados_.erase(itNodo); //lo quito de generados.

        inspeccionados_.insert(aux);  //lo agrego a inspeccionados.


        if (aux.estado_.id_==nodoDestino)
              {
                solucionEncontrada_=true;
                crearCaminoSolucion(aux);
                generados_.clear(); //libero esta memoria, pusto que el trazo es ya solo con nodos que estarán en inspeccionados.

               }

        else
            generarSucesores(aux);


      }  while (!solucionEncontrada_);

return solucionEncontrada_;

}

void GrafoIA_t::limpiar()
{
    grafo_.clear();
    limpiarArbol(); //limpia lo que tenga que ver solo con la busqueda y creacion del arbol de busqueda.
    numeroNodos_=0;
    solucionEncontrada_=false;
}

void GrafoIA_t::limpiarArbol()
{
    generados_.clear();
    inspeccionados_.clear();
    caminoSolucion_.clear();
    solucionEncontrada_=false;
}
