#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <array>
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
    ~Aresta() {}
  };

private:
  int **mat; // @{\it pesos do tipo inteiro}@
  int numVertices, numOperacao;
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

  int numeroTarefas();
  int numeroMaquinas();
  int numeroArestas();

  void imprime();
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

  void floydWarshall(int raiz, int destino);

  bool bipartidoCompleto();
  int encontrarMenorSomaColuna(int L);
  void escalonamento();

  ~Grafo();
};

Grafo::Grafo(istream &in)
{
  int v1, v2, peso;
  in >> this->numVertices;
  this->mat = new int *[numVertices];
  for (int i = 0; i < numVertices; i++)
    this->mat[i] = new int[numVertices];
  this->pos = new int[numVertices];

  for (int i = 0; i < this->numVertices; i++)
  {
    for (int j = 0; j < this->numVertices; j++)
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
  this->numVertices = numVertices;
  for (int i = 0; i < this->numVertices; i++)
  {
    for (int j = 0; j < this->numVertices; j++)
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
  while ((this->pos[v] < this->numVertices) &&
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

int Grafo::numeroTarefas()
{
  int T = 0, aux = -1;

  for (int v = 1; v < this->numVertices; v++)
    if (!this->listaAdjVazia(v))
    {
      Aresta *adj = this->primeiroListaAdj(v);
      while (adj != NULL && adj->_v1() != aux)
      {
        aux = adj->_v1();
        T++;
        adj = this->proxAdj(v);
      }
    }

  // cout << "aqui: " << T << endl;
  return T;
}

int Grafo::numeroMaquinas()
{
  return abs(this->numeroTarefas() - this->_numVertices());
}

void Grafo::imprime()
{
  cout << "   ";
  for (int i = this->numeroTarefas() + 1; i <= this->numeroTarefas() + this->numeroMaquinas(); i++)
    cout << i << "   ";
  cout << endl;
  for (int i = 1; i <= this->numeroTarefas(); i++)
  {
    cout << i << "  ";
    for (int j = this->numeroTarefas() + 1; j <= this->numeroTarefas() + this->numeroMaquinas(); j++)
      cout << this->mat[i][j] << "   ";
    cout << endl;
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

int Grafo::numeroArestas()
{
  int A = 0, linhas = this->numeroTarefas(), colunas = this->numeroTarefas() + 1;

  for (int i = 1; i <= linhas; i++)
  {
    for (int j = colunas; j <= this->numVertices; j++)
    {
      // cout << "Aresta [" << i << "][" << j << "] -> peso (" << this->mat[i][j] << ")" << endl;
      A++;
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

int Grafo::encontrarMenorSomaColuna(int L)
{
  int numLinhas = L;
  int numColunas = this->numVertices;

  // Inicializar a menor soma como a soma da primeira coluna
  int menorSoma = 0;
  for (int i = 0; i <= numLinhas; i++)
  {
    menorSoma += this->mat[i][numLinhas + 1];
  }

  int indiceMenorSoma = numLinhas + 1;

  // Percorrer as colunas e calcular a soma de cada uma
  for (int j = numLinhas + 1; j <= numColunas; j++)
  {
    int somaColuna = 0;

    for (int i = 1; i <= numLinhas; i++)
    {
      somaColuna += this->mat[i][j];
    }

    // Verificar se a soma da coluna atual é menor que a menor soma encontrada até agora
    if (somaColuna < menorSoma)
    {
      menorSoma = somaColuna;
      indiceMenorSoma = j;
    }
  }

  return indiceMenorSoma;
}

void Grafo::escalonamento()
{
  int aux = -1, L = this->numeroTarefas();
  Grafo *grafoEsc = new Grafo(this->numVertices);

  for (int v = 1; v <= L; v++)
  {
    aux = grafoEsc->encontrarMenorSomaColuna(L);

    cout << endl;

    if (this->existeAresta(v, aux))
    {
      cout << "v: " << v << " || aux: " << aux << " || peso: " << this->mat[v][aux] << endl;
      cout << "existe" << endl;
      grafoEsc->insereAresta(v, aux, this->mat[v][aux]);
      grafoEsc->imprime();
    }
    else
    {
      cout << "não existe" << endl;
    }
    cout << endl;
  }

  cout << "Número de Tarefas: " << grafoEsc->numeroTarefas() << endl;
  cout << "Número de Máquinas: " << grafoEsc->numeroMaquinas() << endl;
  cout << "Número de Arestas: " << grafoEsc->numeroArestas() << endl;
  cout << endl;
  grafoEsc->imprime();
}

Grafo::~Grafo()
{
  for (int i = 0; i < numVertices; i++)
    delete[] this->mat[i];
  delete[] this->mat;
  delete[] this->pos;
}