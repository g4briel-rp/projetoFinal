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
  cout << "Número de Tarefas: " << grafo->numeroTarefas() << endl;

  cout << "Número de Máquinas: " << grafo->numeroMaquinas() << endl;

  cout << "Número de Arestas: " << grafo->numeroArestas() << endl;

  cout << "É bipartido completo: " << grafo->bipartidoCompleto() << endl;

  cout << endl;

  grafo->imprime();

  cout << endl;

  if (grafo->bipartidoCompleto())
  {
    grafo->escalonamento();
  }
  else
  {
    cout << "Não é um grafo bipartido completo" << endl;
  }
}
