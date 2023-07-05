#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated"
#include "./matrizadj/Grafo.h"
#pragma GCC diagnostic pop
#include <fstream>
using namespace std;

int main(int argc, char **argv)
{
  ifstream in("./entradas_v1/problema5_1.txt");
  // ifstream in("input2.txt");
  Grafo *grafo = new Grafo(in);
  grafo->imprime();
  cout << endl;
  cout << "Número de Máquinas: " << grafo->_numMaquinas() << endl;
  cout << "Número de Tarefas: " << grafo->_numTarefas() << endl;
  cout << "Número de Arestas: " << grafo->numeroArestas() << endl;
  cout << "É bipartido completo: " << grafo->bipartidoCompleto() << endl;

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
