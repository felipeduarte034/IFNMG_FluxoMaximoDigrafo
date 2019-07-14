#include <iostream>
#include <vector>
#include <string>

/*
    EQUIPE:
    Luiz Felipe Duarte
    João Kevin
*/

/**
 * BRANCO - nodo ainda não encontrado
 * CINZA  - nodo encontrado
 * PRETO  - todos os vizinhos verificados
 */
#define BRANCO 0
#define CINZA 1
#define PRETO 2

#define CAP 1001

//INFINITO --> 1.061.109.567 em decimal
#define INF 0x3f3f3f

//N --> vertices de 0 até N-1 [0,N)
//M --> m linhas - numero de arcos
//s --> vertice de origem
//t --> vertice de destino

using namespace std;
class Aresta
{
public:
	int origem;
	int destino;
	int peso;
	void set(int _origem, int _destino, int _peso)
	{
		origem  = _origem;
		destino = _destino;
		peso    = _peso;
	}
	void print()
	{
		cout << "(" << origem << "," << destino << "," << peso << ")" << endl;
	}
};

class Grafo
{
public:
	int n; //quantidade de VERTICES no grafo
	int m; //quantidade de ARESTAS no grafo
	Aresta* arestas;
	vector<vector<int>> adj; //lista de adjacentes
	vector<int> container; //lista nodos

	Grafo(int quant_vertices, int quant_arestas)
	{
		n = quant_vertices;
		m = quant_arestas;
		arestas = new Aresta[m];
		for (int i = 0; i < n; i++)
		{
			adj.push_back(vector<int>());
		}
	}
	void criarAresta(int x, int y)
	{
		adj[x].push_back(y);
	}
	int getPeso(int orig, int dest)
	{
		for (int i = 0; i < m; ++i)
		{
			if(orig==arestas[i].origem && dest==arestas[i].destino)
				return arestas[i].peso;
		}
		return -1;
	}
	void setPeso(int _origem, int _destino, int _peso)
	{
		for (int i=0; i<m; ++i)
		{
			if(_origem==arestas[i].origem && _destino==arestas[i].destino)
			{
				arestas[i].peso = _peso;
				return;
			}
		}
	}
	void printArestas()
	{
		for(int i=0; i<m; i++)
		{
			arestas[i].print();
		}
	}
	void printContainer()
	{
		cout << "container: ";
		for(int c : container)
		{
			cout << c << ",";
		}
		cout<<endl;
	}
	void printAdj()
	{
		int counter=0;
		for(vector<int> v : adj)
		{
			cout << counter++ << "--> ";
			for(int x : v)
			{
				cout << x << ", ";
			}
			cout << endl;
		}
	}
};
class Info
{
public:
	int x;
	int y;
	int w;
	Info(int _x, int _y, int _w)
	{
		x = _x;
		y = _y;
		w = _w;
	}
	void print()
	{
		cout << "(" << x << "," << y << "," << w << ")" << endl;
	}
};

//------------------------------------------------------------------------------
void printArr(int dist[], int n) 
{ 
    printf("Vertex   Distance from Source\n"); 
    for (int i = 0; i < n; ++i) 
        printf("%d \t\t %d\n", i, dist[i]); 
}

void printPP(int *v, int n) 
{ 
    cout << ": [pai:filho] ";
    for (int i=0; i<n; ++i) 
        cout << "[" << v[i] << ":" << i << "] ";
    cout << endl;
}

void printVec(vector<int> v) 
{ 
    cout << "[ ";
    for (int i : v) 
        cout << i << " -> ";
    cout << "]" << endl;
}

bool BFS(Grafo *g, int s, int t, vector<int> &parent) //busca em largura no grafo
{
	std::vector<int> fila(g->n);
	std::vector<int> cor(g->n,0);
	int ini,fim;
	ini=fim=0;
	cor[s]=CINZA;
	fila[fim++]=s;

	parent[s] = -1;
	//cout << "BFS --> ";
	while(ini<fim)
	{
		int v=fila[ini++];
		for(int i=0;i<g->adj[v].size();++i)
		{
			int w=g->adj[v][i];
			if(cor[w]==BRANCO && g->getPeso(v,w) > 0)
			{
				//cout << w << ",";
				cor[w]=CINZA;
				fila[fim++]=w;

				parent[w] = v;
			}
		}
		cor[v]=PRETO;
		//cout << endl;
	}
	//cout << " cor[t]: " << cor[t] << endl;
	return (cor[t] != BRANCO); //o destino foi visitado, existe fluxo disponivel no caminho.
}

int MaxFlow(Grafo *g, int s, int t)
{
	vector<int> parent(g->n,-1);
	int maxflow=0;
	while(BFS(g,s,t,parent)) //enquanto tiver fluxo disponivel até t.
	{
		//cout << "\n    parent: "; printVec(parent); //break;

        int gargalo = CAP;
        int ori=parent[t];
        int des=t;
        while(ori != -1) //recupera o valor do gargalo do caminho obtido
        {
        	int p = g->getPeso(ori,des);
        	gargalo = min(gargalo, p);
        	des = ori;
        	ori = parent[ori];
        }
        //cout << "    gargalo: " << gargalo << endl;

        ori=parent[t];
        des=t;
        while(ori != -1) //subtrai o gargalo no fluxo do caminho obtido
        {
        	int p = g->getPeso( ori, des );
        	g->setPeso(ori,des,p - gargalo);
        	des = ori;
        	ori = parent[ori];
        }
        maxflow += gargalo; //atualiza o valor do fluxo máximo;
	}
	return maxflow;
}

//------------------------------------------------------------------------------

Info *DecodificaInstrucao(string line)
{
	int x = 0;
	int y = 0;
	int w = 0;

    size_t found = line.find(" "); 
    if (found != string::npos) 
    {
    	x = atoi(line.substr(0, found).c_str());
    } 
  
    char arr[] = " ";
    int aux = found; 
    found = line.find(arr, found+1); 
    if (found != string::npos) 
    {
    	y = atoi(line.substr(aux, found).c_str());
		w = atoi(line.substr(found).c_str());
    }
    else
    	y = atoi(line.substr(aux).c_str());

	return new Info(x, y, w);
}

int main(int argc, char const *argv[])
{
	char line[50];
	int counter = 0; //contador de linhas lidas do arquivo de entrada;
	Grafo *g;
	Info *info = NULL;
	int m=0;//numero de linhas

	//int vert_origem=0,vert_destino=0;
	vector<vector<int>> path; //a primeira coluna do path armazena o peso final do caminho, os demais elementos de cada linha gera um caminho.

	while (scanf("%[^\n]\n", line) == 1)
	{
		//cout << line << endl;
		counter++;
		if (counter == 1)
		{
			info = DecodificaInstrucao(line);
			//info->print();
			m = info->y;
			g = new Grafo(info->x, m);
			continue;
		}
		info = DecodificaInstrucao(line);
		//info->print();
		g->arestas[counter-2].set(info->x, info->y, info->w);
		g->criarAresta(info->x, info->y);

		if(counter == m+1)//+1 devido a primeira linha ser parametro e a ultima linha ser vertice de origem e destino
		{
			info = DecodificaInstrucao(line);
			//vert_origem = info->x;
			//vert_destino = info->y;
			//cout << "origem:"<<vert_origem<<" - destino:"<<vert_destino<<endl;
			break;
		}
	}

	/*CODE*/
	int resp = MaxFlow(g,0,g->n-1);
	cout << resp << endl;
	return 0;
}