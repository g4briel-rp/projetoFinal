#include "Lista.h"
#include "FPHeapMinIndireto.h"
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <array>
#include <cstring>
#include <float.h>
#include <limits.h>

using namespace std;

class Grafo
{
public:
  class Aresta
  {
  private:
    int v1, v2, peso;

  public:
    Aresta(int v1, int v2, int peso)
    {
      this->v1 = v1;
      this->v2 = v2;
      this->peso = peso;
    }
    int _peso() { return this->peso; }
    int _v1() { return this->v1; }
    int _v2() { return this->v2; }
    bool operator<(const Aresta &p) const
    {
      return peso > p.peso;
    }
    ~Aresta() {}
  };

private:
  class Celula
  {
    friend class Grafo;
    friend ostream &operator<<(ostream &out, const Celula &celula)
    {
      out << "vertice:" << celula.vertice << endl;
      out << "peso:" << celula.peso << endl;
      return out;
    }

  private:
    int vertice, peso;

  public:
    Celula(int v, int p)
    {
      this->vertice = v;
      this->peso = p;
    }
    Celula(const Celula &cel) { *this = cel; }
    bool operator==(const Celula &celula) const
    {
      return this->vertice == celula.vertice;
    }
    bool operator!=(const Celula &celula) const
    {
      return this->vertice != celula.vertice;
    }
    const Celula &operator=(const Celula &cel)
    {
      this->vertice = cel.vertice;
      this->peso = cel.peso;
      return *this; // @{\it permite atribui\c{c}\~oes encadeadas}@
    }
    ~Celula() {}
  };
  Lista<Celula> *adj;
  int numVertices, numOperacao;

public:
  Grafo(istream &in);
  Grafo(int numVertices);
  Grafo(int numVertices, int numArestas);
  void insereAresta(int v1, int v2, int peso);
  bool existeAresta(int v1, int v2) const;
  bool listaAdjVazia(int v) const;
  Aresta *lerAresta();
  Aresta *primeiroListaAdj(int v);
  Aresta *proxAdj(int v);
  Aresta *retiraAresta(int v1, int v2);
  void imprime() const;
  int _numVertices() const;
  int _numOperacao();
  Grafo *grafoTransposto();
  Grafo *grafoNaoDirecionado();
  int grauVertice(int v) const;
  bool completo() const;
  bool euleriano() const;
  bool subEuleriano() const;
  void ciclo();
  void caminho();

  void buscaProfundidade();
  void visitaDFS(int v, int *cor, int *antecessor);

  bool Aciclico();
  void visitaAciclicoDFS(int v, int *cor, int *antecessor, bool *ciclo);

  vector<int> ordenacaoTopologica();
  void visitaOrdenacaoTopologicaDFS(int v, int *cor, int *antecessor, vector<int> &L);

  int numComponentes();
  void visitaDFS2(int v, int *cor, int *antecessor);

  void BuscaLargura();
  void visitaBFS(int v, int *cor, int *distancia, int *antecessor);

  void caminhoMaisCurto(int u, int v);
  void visitaBFS2(int v, int *cor, int *distancia, int *antecessor);
  void imprimeCaminho(int u, int v, int *antecessor);

  Grafo *kruskal();
  int encontreConjunto(int conjunto[], int x);
  void unirConjunto(int conjunto[], int x, int y);

  Grafo *prim(int raiz);

  void dijkstra(int raiz, int destino);

  int numeroTarefas();
  int numeroMaquinas();
  int numeroArestas();
  bool bipartidoCompleto();
  Grafo *escalonamento();

  ~Grafo();
};

Grafo::Grafo(istream &in)
{
  int v1, v2, peso, numVertices;
  in >> numVertices;

  this->adj = new Lista<Celula>[numVertices];
  this->numVertices = numVertices;
  this->numOperacao = numOperacao;

  while (in >> v1 >> v2 >> peso)
  {
    Grafo::Aresta *a = new Grafo::Aresta(v1, v2, peso);
    this->insereAresta(a->_v1(), a->_v2(), a->_peso());
    delete a;
  }
}

Grafo::Grafo(int numVertices)
{
  this->adj = new Lista<Celula>[numVertices];
  this->numVertices = numVertices;
  this->numOperacao = numOperacao;
}

Grafo::Grafo(int numVertices, int numArestas)
{
  this->adj = new Lista<Celula>[numVertices];
  this->numVertices = numVertices;
  this->numOperacao = numOperacao;
}

Grafo::Aresta *Grafo::lerAresta()
{
  cout << "Aresta:" << endl;
  cout << "  V1:";
  int v1 = 0;
  cin >> v1;
  cout << "  V2:";
  int v2 = 0;
  cin >> v2;
  cout << "  Peso:";
  int peso = 0;
  cin >> peso;
  return new Grafo::Aresta(v1, v2, peso);
}

void Grafo::insereAresta(int v1, int v2, int peso)
{
  Celula item(v2, peso);
  this->adj[v1].insere(item);
}

bool Grafo::existeAresta(int v1, int v2) const
{
  Celula item(v2, 0);
  return (this->adj[v1].pesquisa(item) != NULL);
}

bool Grafo::listaAdjVazia(int v) const { return this->adj[v].vazia(); }

Grafo::Aresta *Grafo::primeiroListaAdj(int v)
{
  // @{\it Retorna a primeira aresta que o v\'ertice v participa ou}@
  // @{\it {\bf NULL} se a lista de adjac\^encia de v for vazia}@
  Celula *item = this->adj[v]._primeiro();
  return item != NULL ? new Aresta(v, item->vertice, item->peso) : NULL;
}

Grafo::Aresta *Grafo::proxAdj(int v)
{
  // @{\it Retorna a pr\'oxima aresta que o v\'ertice v participa ou}@
  // @{\it {\bf NULL} se a lista de adjac\^encia de v estiver no fim}@
  Celula *item = this->adj[v].proximo();
  return item != NULL ? new Aresta(v, item->vertice, item->peso) : NULL;
}

Grafo::Aresta *Grafo::retiraAresta(int v1, int v2)
{
  Celula chave(v2, 0);
  Celula *item = this->adj[v1].retira(chave);
  Aresta *aresta = item != NULL ? new Aresta(v1, v2, item->peso) : NULL;
  delete item;
  return aresta;
}

void Grafo::imprime() const
{
  for (int i = 0; i < this->numVertices; i++)
  {
    cout << "Vertice " << i << ":" << endl;
    Celula *item = this->adj[i]._primeiro();
    while (item != NULL)
    {
      cout << "  " << item->vertice << " (" << item->peso << ")" << endl;
      item = this->adj[i].proximo();
    }
  }
}

int Grafo::_numVertices() const { return this->numVertices; }

int Grafo::_numOperacao() { return this->numOperacao; }

Grafo *Grafo::grafoTransposto()
{
  Grafo *grafoT = new Grafo(this->numVertices);
  for (int v = 0; v < this->numVertices; v++)
    if (!this->listaAdjVazia(v))
    {
      Aresta *adj = this->primeiroListaAdj(v);
      while (adj != NULL)
      {
        grafoT->insereAresta(adj->_v2(), adj->_v1(), adj->_peso());
        delete adj;
        adj = this->proxAdj(v);
      }
    }
  return grafoT;
}

Grafo *Grafo::grafoNaoDirecionado()
{
  Grafo *grafoND = new Grafo(this->numVertices);
  // Ex5: Implementar o grafo não direcionado
  for (int v = 0; v < this->numVertices; v++)
    if (!this->listaAdjVazia(v))
    {
      Aresta *adj = this->primeiroListaAdj(v);
      while (adj != NULL)
      {
        grafoND->insereAresta(adj->_v1(), adj->_v2(), adj->_peso());
        grafoND->insereAresta(adj->_v2(), adj->_v1(), adj->_peso());
        delete adj;
        adj = this->proxAdj(v);
      }
    }
  return grafoND;
}

int Grafo::grauVertice(int v) const
{
  int grau = 0;

  Celula *item = this->adj[v]._primeiro();

  while (item != NULL)
  {
    grau++;
    item = this->adj[v].proximo();
  }

  return grau;
}

bool Grafo::completo() const
{
  int aux;
  for (int i = 0; i < this->numVertices; i++)
  {
    if ((aux = this->grauVertice(i)) != this->numVertices - 1)
      return false;
  }
  return true;
}

bool Grafo::euleriano() const
{
  int aux;
  for (int i = 0; i < this->numVertices; i++)
  {
    if ((aux = this->grauVertice(i)) % 2 != 0)
      return false;
  }
  return true;
}

bool Grafo::subEuleriano() const
{
  int aux, conta = 0;
  for (int i = 0; i < this->numVertices; i++)
  {
    if ((aux = this->grauVertice(i)) % 2 != 0)
    {
      conta++;
    }
  }

  if (conta != 1 and conta > 2)
    return false;

  return true;
}

void Grafo::ciclo()
{
  if (this->euleriano() == true)
  {
    int aux = 0, i, maior = this->grauVertice(aux);

    for (i = 1; i < this->numVertices; i++)
    {
      if (this->grauVertice(i) > maior)
      {
        maior = this->grauVertice(i);
        aux = i;
      }
    }

    int v = aux;

    cout << "Ciclo: " << v;
    Aresta *adj = this->primeiroListaAdj(v);
    v = adj->_v2();
    this->retiraAresta(adj->_v1(), adj->_v2());
    this->retiraAresta(adj->_v2(), adj->_v1());

    while ((!this->listaAdjVazia(v)))
    {
      this->numOperacao++;
      cout << v;
      adj = this->primeiroListaAdj(v);
      v = adj->_v2();
      this->retiraAresta(adj->_v1(), adj->_v2());
      this->retiraAresta(adj->_v2(), adj->_v1());
    }

    cout << v;
  }
}

void Grafo::caminho()
{
  if (this->subEuleriano() == true)
  {
    int aux, i, inicio = -1, fim = -1;

    for (i = 0; i < this->numVertices; i++)
    {
      if (this->grauVertice(i) % 2 != 0)
      {
        inicio = i;
        break;
      }
    }

    int v = inicio;

    cout << "Caminho: " << v;
    Aresta *adj = this->primeiroListaAdj(v);
    v = adj->_v2();
    this->retiraAresta(adj->_v1(), adj->_v2());
    this->retiraAresta(adj->_v2(), adj->_v1());

    while ((!this->listaAdjVazia(v)))
    {
      this->numOperacao++;
      cout << v;
      adj = this->primeiroListaAdj(v);
      v = adj->_v2();
      this->retiraAresta(adj->_v1(), adj->_v2());
      this->retiraAresta(adj->_v2(), adj->_v1());
    }

    cout << v;
  }
}

void Grafo::buscaProfundidade()
{
  int *cor = new int[this->_numVertices()];
  int *antecessor = new int[this->_numVertices()];

  for (int i = 0; i < numVertices; i++)
  {
    cor[i] = 0;
    antecessor[i] = -1;
  }

  for (int i = 0; i < numVertices; i++)
  {
    if (cor[i] == 0)
    {
      visitaDFS(i, cor, antecessor);
    }
  }
  cout << endl;
}

void Grafo::visitaDFS(int v, int *cor, int *antecessor)
{
  int u = 0;
  cor[v] = 1;
  cout << v << " ";
  Aresta *adj = this->primeiroListaAdj(v);
  while (adj != NULL)
  {
    u = adj->_v2();
    if (cor[u] == 0)
    {
      antecessor[u] = v;
      visitaDFS(u, cor, antecessor);
    }
    adj = proxAdj(v);
  }
  cor[v] = 2;
}

void Grafo::visitaDFS2(int v, int *cor, int *antecessor)
{
  int u = 0;
  cor[v] = 1;
  Aresta *adj = this->primeiroListaAdj(v);
  while (adj != NULL)
  {
    u = adj->_v2();
    if (cor[u] == 0)
    {
      antecessor[u] = v;
      visitaDFS2(u, cor, antecessor);
    }
    adj = proxAdj(v);
  }
  cor[v] = 2;
}

bool Grafo::Aciclico()
{
  int *cor = new int[this->_numVertices()];
  int *antecessor = new int[this->_numVertices()];
  bool ciclo = false;

  for (int i = 0; i < numVertices; i++)
  {
    cor[i] = 0;
    antecessor[i] = -1;
  }

  for (int i = 0; i < numVertices; i++)
  {
    if (cor[i] == 0)
    {
      visitaAciclicoDFS(i, cor, antecessor, &ciclo);
    }
  }
  return ciclo;
}

void Grafo::visitaAciclicoDFS(int v, int *cor, int *antecessor, bool *ciclo)
{
  int u = 0;
  cor[v] = 1;
  Aresta *adj = this->primeiroListaAdj(v);
  while (adj != NULL)
  {
    u = adj->_v2();
    if (cor[u] == 1 && antecessor[v] != u)
    {
      *ciclo = true;
    }
    if (cor[u] == 0)
    {
      antecessor[u] = v;
      visitaAciclicoDFS(u, cor, antecessor, ciclo);
    }
    adj = proxAdj(v);
  }
  cor[v] = 2;
}

vector<int> Grafo::ordenacaoTopologica()
{
  vector<int> L;
  int *cor = new int[this->_numVertices()];
  int *antecessor = new int[this->_numVertices()];

  for (int i = 0; i < numVertices; i++)
  {
    cor[i] = 0;
    antecessor[i] = -1;
  }

  for (int i = 0; i < numVertices; i++)
  {
    if (cor[i] == 0)
    {
      visitaOrdenacaoTopologicaDFS(i, cor, antecessor, L);
    }
  }
  return L;
}

void Grafo::visitaOrdenacaoTopologicaDFS(int v, int *cor, int *antecessor, vector<int> &L)
{
  int u = 0;
  cor[v] = 1;
  Aresta *adj = this->primeiroListaAdj(v);
  while (adj != NULL)
  {
    u = adj->_v2();
    if (cor[u] == 0)
    {
      antecessor[u] = v;
      visitaOrdenacaoTopologicaDFS(u, cor, antecessor, L);
    }
    adj = proxAdj(v);
  }
  cor[v] = 2;
  L.push_back(v);
}

int Grafo::numComponentes()
{
  int *cor = new int[this->_numVertices()];
  int *antecessor = new int[this->_numVertices()];
  int k = 0;

  for (int i = 0; i < numVertices; i++)
  {
    cor[i] = 0;
    antecessor[i] = -1;
  }

  for (int i = 0; i < numVertices; i++)
  {
    if (cor[i] == 0)
    {
      k++;
      visitaDFS2(i, cor, antecessor);
    }
  }
  return k;
}

void Grafo::BuscaLargura()
{
  int *cor = new int[this->_numVertices()];
  int *distancia = new int[this->_numVertices()];
  int *antecessor = new int[this->_numVertices()];

  for (int i = 0; i < numVertices; i++)
  {
    cor[i] = 0;
    distancia[i] = 0;
    antecessor[i] = -1;
  }

  for (int i = 0; i < numVertices; i++)
  {
    if (cor[i] == 0)
    {
      visitaBFS(i, cor, distancia, antecessor);
    }
  }
  cout << endl;
}

void Grafo::visitaBFS(int v, int *cor, int *distancia, int *antecessor)
{
  int u = 0;
  queue<int> fileira;

  distancia[v] = 0;
  cor[v] = 1;
  fileira.push(v);
  cout << v << " ";

  while (!fileira.empty())
  {
    v = fileira.front();
    fileira.pop();
    Aresta *adj = this->primeiroListaAdj(v);
    while (adj != NULL)
    {
      u = adj->_v2();
      if (cor[u] == 0)
      {
        cout << u << " ";
        cor[u] = 1;
        distancia[u] = distancia[v] + 1;
        antecessor[u] = v;
        fileira.push(u);
      }
      adj = proxAdj(v);
    }
    cor[v] = 2;
  }
}

void Grafo::caminhoMaisCurto(int u, int v)
{
  int *cor = new int[this->_numVertices()];
  int *distancia = new int[this->_numVertices()];
  int *antecessor = new int[this->_numVertices()];

  for (int i = 0; i < numVertices; i++)
  {
    cor[i] = 0;
    distancia[i] = 0;
    antecessor[i] = -1;
  }

  for (int i = u; i < numVertices; i++)
  {
    if (cor[i] == 0)
    {
      visitaBFS2(i, cor, distancia, antecessor);
    }
  }
  cout << "Menor caminho: ";
  imprimeCaminho(u, v, antecessor);
  cout << endl;
}

void Grafo::visitaBFS2(int v, int *cor, int *distancia, int *antecessor)
{
  int u = 0;
  queue<int> fileira;

  distancia[v] = 0;
  cor[v] = 1;
  fileira.push(v);

  while (!fileira.empty())
  {
    v = fileira.front();
    fileira.pop();
    Aresta *adj = this->primeiroListaAdj(v);
    while (adj != NULL)
    {
      u = adj->_v2();
      if (cor[u] == 0)
      {
        cor[u] = 1;
        distancia[u] = distancia[v] + 1;
        antecessor[u] = v;
        fileira.push(u);
      }
      adj = proxAdj(v);
    }
    cor[v] = 2;
  }
}

void Grafo::imprimeCaminho(int u, int v, int *antecessor)
{
  if (u == v)
    cout << u;
  else if (antecessor[v] == -1)
  {
    cout << "Nao existe caminho de " << u << " para " << v;
  }
  else
  {
    imprimeCaminho(u, antecessor[v], antecessor);
    cout << v;
  }
}

int Grafo::encontreConjunto(int conjunto[], int x)
{
  if (conjunto[x] == -1)
  {
    return x;
  }

  return encontreConjunto(conjunto, conjunto[x]);
}

void Grafo::unirConjunto(int conjunto[], int x, int y)
{
  int conjuntoX = encontreConjunto(conjunto, x);
  int conjuntoY = encontreConjunto(conjunto, y);

  conjunto[conjuntoX] = conjuntoY;
}

Grafo *Grafo::kruskal()
{
  Grafo *grafoKruskal = new Grafo(this->numVertices);

  vector<Aresta> S;
  vector<Aresta> A;

  // cria 'V' conjunto
  int *conjunto = new int[this->_numVertices()];

  // inicializa todos os subconjuntos como conjuntos de um unico
  memset(conjunto, -1, sizeof(int) * this->_numVertices());

  // adiciona as arestas em A
  for (int v = 0; v < this->numVertices; v++)
  {
    if (!this->listaAdjVazia(v))
    {
      Aresta *adj = this->primeiroListaAdj(v);
      while (adj != NULL)
      {
        A.push_back(*adj);
        delete adj;
        adj = this->proxAdj(v);
      }
    }
  }

  // // imprimir as arestas
  // for (auto a : A)
  // {
  //   cout << a._peso() << " ";
  // }

  // cout << endl;

  // ordena as arestas pelo menor peso
  sort(A.begin(), A.end());

  // // imprimir as arestas
  // for (auto a : A)
  // {
  //   cout << a._peso() << " ";
  // }

  // cout << endl;

  for (auto a : A)
  {
    int u = a._v1();
    int v = a._v2();

    if (encontreConjunto(conjunto, u) != encontreConjunto(conjunto, v))
    {
      S.push_back(a);
      unirConjunto(conjunto, u, v);
    }
  }

  for (auto s : S)
  {
    // cout << s._v1() << " - " << s._v2() << endl;
    grafoKruskal->insereAresta(s._v1(), s._v2(), s._peso());
    grafoKruskal->insereAresta(s._v2(), s._v1(), s._peso());
  }

  return grafoKruskal;
}

Grafo *Grafo::prim(int raiz)
{
  Grafo *grafoPrim = new Grafo(this->numVertices);

  int *antecessor = NULL;
  double *peso = NULL;

  int n = this->_numVertices();

  peso = new double[n];
  int *vs = new int[n + 1];
  bool *itensHeap = new bool[n];
  antecessor = new int[n];

  for (int u = 0; u < n; u++)
  {
    antecessor[u] = -1;
    peso[u] = DBL_MAX;
    vs[u + 1] = u;
    itensHeap[u] = true;
  }

  peso[raiz] = 0;
  FPHeapMinIndireto q(peso, vs, n);
  q.constroi();

  while (q.vazio() != true)
  {
    int u = q.retiraMin();
    itensHeap[u] = false;
    if (!this->listaAdjVazia(u))
    {
      Aresta *adj = this->primeiroListaAdj(u);
      while (adj != NULL)
      {
        if (itensHeap[adj->_v2()] == true && adj->_peso() < peso[adj->_v2()])
        {
          antecessor[adj->_v2()] = u;
          q.diminuiChave(adj->_v2(), adj->_peso());
        }
        adj = this->proxAdj(u);
      }
    }
  }

  for (int u = 0; u < this->_numVertices(); u++)
  {
    if (antecessor[u] != -1)
    {
      // cout << "(" << antecessor[u] << "," << u << ") = " << peso[u] << endl;
      grafoPrim->insereAresta(antecessor[u], u, peso[u]);
      grafoPrim->insereAresta(u, antecessor[u], peso[u]);
    }
  }

  return grafoPrim;
}

void Grafo::dijkstra(int raiz, int destino)
{
  int *antecessor = NULL;
  double *peso = NULL;
  int n = this->_numVertices();
  int *vs = new int[n + 1];
  peso = new double[n];
  antecessor = new int[n];

  for (int v = 0; v < n; v++)
  {
    peso[v] = DBL_MAX;
    antecessor[v] = -1;
    vs[v + 1] = v;
  }

  peso[raiz] = 0;
  FPHeapMinIndireto q(peso, vs, n);
  q.constroi();

  while (q.vazio() != true)
  {
    int u = q.retiraMin();

    if (!this->listaAdjVazia(u))
    {
      Aresta *adj = this->primeiroListaAdj(u);
      while (adj != NULL)
      {
        if (peso[adj->_v2()] > (peso[adj->_v1()] + adj->_peso()))
        {
          peso[adj->_v2()] = peso[adj->_v1()] + adj->_peso();
          antecessor[adj->_v2()] = adj->_v1();
          q.diminuiChave(adj->_v2(), peso[adj->_v2()]);
        }
        adj = this->proxAdj(u);
      }
    }
  }

  cout << endl;

  for (int u = 0; u < n; u++)
  {
    if (antecessor[u] != -1)
    {
      cout << "(" << antecessor[u] << "," << u << ") = " << peso[u] << endl;
    }
  }

  cout << endl << "Caminho: ";

  this->imprimeCaminho(raiz, destino, antecessor);

  cout << endl;
}

int Grafo::numeroTarefas()
{
  int T = 0, aux = -1;

  for (int v = 0; v < this->numVertices; v++)
    if (!this->listaAdjVazia(v))
    {
      Aresta *adj = this->primeiroListaAdj(v);
      while (adj != NULL && adj->_v1() != aux)
      {
        aux = adj->_v1();
        T++;
        delete adj;
        adj = this->proxAdj(v);
      }
    }

  return T;
}

int Grafo::numeroMaquinas()
{
  return abs(this->numeroTarefas() - this->_numVertices());
}

int Grafo::numeroArestas()
{
  int A = 0;

  for (int v = 0; v < this->numVertices; v++)
    if (!this->listaAdjVazia(v))
    {
      Aresta *adj = this->primeiroListaAdj(v);
      while (adj != NULL)
      {
        A++;
        adj = this->proxAdj(v);
      }
    }
  
  return A;
}

bool Grafo::bipartidoCompleto()
{
  if (this->numeroArestas() == (this->numeroTarefas() * this->numeroMaquinas()))
    return true;
  else
    return false;
}

//indice do menor peso
int encontraMIN(vector<int> &tempos)
{
  int min = INT_MAX;

  for (int i: tempos)
  {
      if (i < min) {
          min = i;
      }
  }

  auto aux = find(tempos.begin(), tempos.end(), min);
  int index = aux - tempos.begin();

  return index;
}

//indice do maior peso
int encontraMAX(vector<int> &tempos)
{
  int max = INT_MIN;

  for (int i: tempos)
  {
      if (i > max) {
          max = i;
      }
  }

  auto aux = find(tempos.begin(), tempos.end(), max);
  int index = aux - tempos.begin();

  return index;
}

Grafo *Grafo::escalonamento()
{

  //aresta_v1  aresta_v2  aresta_peso
  //tarefas    maquinas   peso
  Grafo *grafoEsc = new Grafo(this->numVertices);
  vector<Aresta> A;

  // adiciona as arestas em A
  for (int v = 0; v < this->numeroTarefas(); v++)
  {
    if (!this->listaAdjVazia(v))
    {
      Aresta *adj = this->primeiroListaAdj(v);

      while (adj != NULL )
      {
        A.push_back(*adj);
        delete adj;
        adj = this->proxAdj(v);
      }
    }
  }

  // ordena as arestas pelo peso
  sort(A.begin(), A.end());

  // Imprimir o vector A ordenado decrescente
  // for (auto a: A)
  //   cout << a._peso() << " ";

  int maq = this->numeroMaquinas();
  vector<int> tempos;

  for (int i = 0; i < maq; i++)
  {
    tempos.push_back(0);
  }

  cout << "NumArestas:" << this->numeroArestas() << " numTarefas:" << this->numeroTarefas() << endl;

  int aux=0;
  //so roda enquanto o numero de arestas for maior que o numero de tarefas
  while(this->numeroArestas() > this->numeroTarefas()){
    for (auto a: A)
    {
      if(a._peso() != aux){//ignorando as tarefas de peso igual
        //depois que a tarefa foi inserida em uma maquina, ela não é usada em outras
        //dessa forma, ignora as tarefas de custo repetido
        aux = a._peso();
        grafoEsc->insereAresta(a._v1(), a._v2(), a._peso());//insino no novoGrafo
      }else{//tarefa que ja foi inserida em alguma maquina não deve ser inserida no novo grafo
        continue;
      }
      
      tempos[encontraMIN(tempos)] += a._peso();
    }

    for (auto t: tempos)
    {
      cout << t << " ";
    }
    
    cout << endl;
  }

  return 0;
}

Grafo::~Grafo() { delete[] this->adj; }