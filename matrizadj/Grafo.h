#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <array>
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
      // cout << "v1: " << this->v1 << " v2: " << this->v2 << " peso: " << this->peso << endl;
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
  int **mat; // @{\it pesos do tipo inteiro}@
  int numVertices, numMaquinas, numTarefas, numOperacao;
  int *pos; // @{\it posi\c{c}\~ao atual ao se percorrer os adjs de um v\'ertice v}@

public:
  Grafo(int numVertices);
  Grafo(istream &in);
  Grafo(int numVertices, int numArestas);
  void insereAresta(int v1, int v2, int peso);
  bool existeAresta(int v1, int v2) const;
  bool listaAdjVazia(int v) const;

  Aresta *primeiroListaAdj(int v);
  Aresta *proxAdj(int v);

  void imprime() const;
  int _numVertices() const;
  int _numOperacao();
  int _numMaquinas();
  int _numTarefas();

  void imprime2(int L, int C);
  int numeroArestas();
  bool bipartidoCompleto();
  int encontrarMenorSoma(int maq, int pro);
  void escalonamento();

  ~Grafo();
};

Grafo::Grafo(istream &in)
{
  int v1, v2, peso;
  in >> this->numMaquinas;
  in >> this->numTarefas;
  in >> this->numVertices;
  this->mat = new int *[numMaquinas + 1];
  for (int i = 0; i <= numMaquinas; i++)
    this->mat[i] = new int[numTarefas + 1];
  this->pos = new int[numTarefas + 1];

  for (int i = 0; i <= this->numMaquinas; i++)
  {
    for (int j = numMaquinas + 1; j <= this->numTarefas; j++)
      this->mat[i][j] = 0;
    this->pos[i] = -1;
  }

  while (in >> v1 >> v2 >> peso)
  {
    Grafo::Aresta *a = new Grafo::Aresta(v1, v2, peso);
    this->insereAresta(a->_v1(), a->_v2(), a->_peso());
    delete a;
  }
}

Grafo::Grafo(int numVertices)
{
  this->mat = new int *[numVertices];
  for (int i = 0; i < numVertices; i++)
    this->mat[i] = new int[numVertices];
  this->pos = new int[numVertices];
  for (int i = 0; i < numVertices; i++)
  {
    for (int j = 0; j < numVertices; j++)
      this->mat[i][j] = 0;
    this->pos[i] = -1;
  }
}

Grafo::Grafo(int numVertices, int numArestas)
{
  this->mat = new int *[numVertices];
  for (int i = 0; i < numVertices; i++)
    this->mat[i] = new int[numVertices];
  this->pos = new int[numVertices];
  this->numVertices = numVertices;
  for (int i = 0; i < this->numVertices; i++)
  {
    for (int j = 0; j < this->numVertices; j++)
      this->mat[i][j] = 0;
    this->pos[i] = -1;
  }
}

void Grafo::insereAresta(int v1, int v2, int peso)
{
  // cout << "v1: " << v1 << " v2: " << v2 << " peso: " << peso << endl;
  this->mat[v1][v2] = peso;
}

bool Grafo::existeAresta(int v1, int v2) const
{
  return (this->mat[v1][v2] > 0);
}

bool Grafo::listaAdjVazia(int v) const
{
  for (int i = 0; i < this->numVertices; i++)
    if (this->mat[v][i] > 0)
      return false;
  return true;
}

Grafo::Aresta *Grafo::primeiroListaAdj(int v)
{
  // @{\it Retorna a primeira aresta que o v\'ertice v participa ou}@
  // @{\it {\bf NULL} se a lista de adjac\^encia de v for vazia}@
  this->pos[v] = -1;
  return this->proxAdj(v);
}

Grafo::Aresta *Grafo::proxAdj(int v)
{
  // @{\it Retorna a pr\'oxima aresta que o v\'ertice v participa ou}@
  // @{\it {\bf NULL} se a lista de adjac\^encia de v estiver no fim}@
  this->pos[v]++;
  while ((this->pos[v] <= this->numVertices) &&
         (this->mat[v][this->pos[v]] == 0))
    this->pos[v]++;
  if (this->pos[v] == this->numVertices + 1)
    return NULL;
  else
    return new Aresta(v, this->pos[v], this->mat[v][this->pos[v]]);
}

void Grafo::imprime() const
{
  cout << "   ";
  for (int i = this->numMaquinas + 1; i <= this->numVertices; i++)
    cout << i << "   ";
  cout << endl;
  for (int i = 1; i <= this->numMaquinas; i++)
  {
    cout << i << "  ";
    for (int j = this->numMaquinas + 1; j <= this->numVertices; j++)
      cout << this->mat[i][j] << "   ";
    cout << endl;
  }
}

int Grafo::_numVertices() const { return this->numVertices; }

int Grafo::_numOperacao() { return this->numOperacao; }

int Grafo::_numMaquinas() { return this->numMaquinas; }

int Grafo::_numTarefas() { return this->numTarefas; }

void Grafo::imprime2(int L, int C)
{
  cout << "   ";
  // for (int i = L + 1; i <= L + C; i++)
  //   cout << i << "   ";
  cout << endl;
  for (int i = 1; i <= L; i++)
  {
    // cout << i << "  ";
    for (int j = L + 1; j <= L + C; j++)
      cout << this->mat[i][j] << "   ";
    cout << endl;
  }
}

int Grafo::numeroArestas()
{
  int A = 0, linhas = this->_numMaquinas(), colunas = this->_numTarefas();

  for (int i = 1; i <= linhas; i++)
  {
    for (int j = linhas + 1; j <= this->numVertices; j++)
    {
      // cout << "Aresta [" << i << "][" << j << "] -> peso (" << this->mat[i][j] << ")" << endl;
      if (this->mat[i][j] != 0)
        A++;
    }
  }

  return A;
}

bool Grafo::bipartidoCompleto()
{
  if (this->numeroArestas() == (this->_numMaquinas() * this->_numTarefas()))
    return true;
  else
    return false;
}

int Grafo::encontrarMenorSoma(int maq, int pro)
{
  int menorSoma = 99999;
  int indiceLinhaMenorSoma = 1;

  for (int i = 1; i <= maq; ++i)
  {
    int somaLinha = 0;

    for (int j = maq + 1; j <= maq + pro; ++j)
    {
      somaLinha += this->mat[i][j];
    }

    if (somaLinha < menorSoma)
    {
      menorSoma = somaLinha;
      indiceLinhaMenorSoma = i;
    }
  }

  return indiceLinhaMenorSoma;
}

void Grafo::escalonamento()
{
  int aux = -1;
  bool contem;
  Grafo *grafoEsc = new Grafo(this->_numVertices() + 1);

  vector<Aresta> A;
  vector<int> verifica;

  for (int v = 1; v <= this->numMaquinas; v++)
  {
    if (!this->listaAdjVazia(v))
    {
      Aresta *adj = this->primeiroListaAdj(v);

      while (adj != NULL)
      {
        // cout << "v1: " << adj->_v1() << " v2: " << adj->_v2() << " peso: " << adj->_peso() << endl;
        A.push_back(*adj);
        delete adj;
        adj = this->proxAdj(v);
      }
    }
  }

  sort(A.begin(), A.end());

  for (auto a : A)
  {
    contem = false;
    aux = grafoEsc->encontrarMenorSoma(this->numMaquinas, this->numTarefas);
    // cout << "menor indice: " << aux << endl;
    // cout << "a_v1: " << a._v1() << " a_v2: " << a._v2() << " peso: " << a._peso() << endl;

    for (auto v : verifica)
    {
      if (v == a._v2())
      {
        contem = true;
        break;
      }
    }

    if (this->existeAresta(aux, a._v2()) && contem == false)
    {
      // cout << "a_v1: " << aux << " a_v2: " << a._v2() << " peso: " << a._peso() << endl;
      grafoEsc->insereAresta(aux, a._v2(), this->mat[aux][a._v2()]);
      verifica.push_back(a._v2());
    }
  }

  cout << "Resultado Final: " << endl;
  // grafoEsc->imprime2(this->numMaquinas, this->numTarefas);

  int soma;

  for (int n = 1; n <= this->numMaquinas; n++)
  {
    soma = 0;
    for (int m = this->numMaquinas + 1; m <= this->numVertices; m++)
    {
      soma += grafoEsc->mat[n][m];
    }
    cout << "Máquina " << n << ": " << soma << endl;
  }
}

Grafo::~Grafo()
{
  for (int i = 0; i < numVertices; i++)
    delete[] this->mat[i];
  delete[] this->mat;
  delete[] this->pos;
}