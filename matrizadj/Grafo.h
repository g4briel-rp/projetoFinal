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
  Aresta *lerAresta();
  Aresta *primeiroListaAdj(int v);
  Aresta *proxAdj(int v);
  Aresta *retiraAresta(int v1, int v2);
  void imprime() const;
  int _numVertices() const;
  int _numOperacao();
  int _numMaquinas();
  int _numTarefas();
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

  void floydWarshall(int raiz, int destino);

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
  return new Aresta(v1, v2, peso);
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

Grafo::Aresta *Grafo::retiraAresta(int v1, int v2)
{
  if (this->mat[v1][v2] == 0)
    return NULL; // @{\it Aresta n\~ao existe}@
  else
  {
    Aresta *aresta = new Aresta(v1, v2, this->mat[v1][v2]);
    this->mat[v1][v2] = 0;
    return aresta;
  }
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

  for (int i = 0; i < this->numVertices; i++)
  {
    if (this->mat[v][i] > 0)
      grau++;
  }

  return grau;
}

bool Grafo::completo() const
{
  int aux;
  for (int i = 0; i < this->numVertices; i++)
  {
    if (aux = this->grauVertice(i) != this->numVertices - 1)
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
  cout << v;
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
  cout << v;

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
        cout << u;
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

  for (int i = 0; i < numVertices; i++)
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

void Grafo::floydWarshall(int raiz, int destino)
{
  int n = this->_numVertices();
  int antecessor[n][n];
  int d[n][n];

  for (int i = 0; i < this->_numVertices(); i++)
  {
    for (int j = 0; j < this->_numVertices(); j++)
    {
      if (i == j)
      {
        d[i][j] = 0;
        antecessor[i][j] = -1;
      }
      else
      {
        if (this->mat[i][j] != 0)
        {
          d[i][j] = this->mat[i][j];
          antecessor[i][j] = i;
        }
        else
        {
          d[i][j] = 999;
          antecessor[i][j] = -1;
        }
      }
    }
  }

  for (int k = 0; k < this->_numVertices(); k++)
  {
    for (int i = 0; i < this->_numVertices(); i++)
    {
      for (int j = 0; j < this->_numVertices(); j++)
      {
        if (i != j && (d[i][k] + d[k][j]) < d[i][j])
        {
          d[i][j] = d[i][k] + d[k][j];
          antecessor[i][j] = antecessor[k][j];
        }
      }
    }
  }

  cout << "\nAntecessor" << endl;

  cout << "   ";
  for (int i = 0; i < this->numVertices; i++)
    cout << i << "   ";
  cout << endl;
  for (int i = 0; i < this->numVertices; i++)
  {
    cout << i << "  ";
    for (int j = 0; j < this->numVertices; j++)
      cout << antecessor[i][j] << "   ";
    cout << endl;
  }

  cout << "\nD" << endl;

  cout << "   ";
  for (int i = 0; i < this->numVertices; i++)
    cout << i << "   ";
  cout << endl;
  for (int i = 0; i < this->numVertices; i++)
  {
    cout << i << "  ";
    for (int j = 0; j < this->numVertices; j++)
      cout << d[i][j] << "   ";
    cout << endl;
  }

  cout << endl
       << "Caminho:";

  this->imprimeCaminho(raiz, destino, antecessor[raiz]);

  cout << endl;
}

void Grafo::imprime2(int L, int C)
{
  cout << "   ";
  for (int i = L + 1; i <= L + C; i++)
    cout << i << "   ";
  cout << endl;
  for (int i = 1; i <= L; i++)
  {
    cout << i << "  ";
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
      grafoEsc->insereAresta(aux, a._v2(), a._peso());
      verifica.push_back(a._v2());
    }
  }

  cout << "Resultado Final: " << endl;
  grafoEsc->imprime2(this->numMaquinas, this->numTarefas);
}

Grafo::~Grafo()
{
  for (int i = 0; i < numVertices; i++)
    delete[] this->mat[i];
  delete[] this->mat;
  delete[] this->pos;
}