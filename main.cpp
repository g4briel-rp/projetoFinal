#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated"
#include "./matrizadj/Grafo.h"
#pragma GCC diagnostic pop
#include <fstream>
using namespace std;

int main(int argc, char **argv)
{
  ifstream in("input.txt");
  Grafo *grafo = new Grafo(in);
  // grafo->imprime();

  // cout << endl;

  // Grafo *grafoND;
  // grafoND = grafo->grafoNaoDirecionado();
  // grafoND->imprime();

  cout << grafo->numeroTarefas() << endl;

  cout << grafo->numeroMaquinas() << endl;

  cout << grafo->numeroArestas() << endl;

  cout << grafo->bipartidoCompleto() << endl;

  // if (grafo->bipartidoCompleto())
  // {
  //   Grafo *result = grafo->escalonamento();
  //   result->imprime();
  // }else
  // {
  //   cout << "Não é um grafo bipartido completo" << endl;
  // }
}
