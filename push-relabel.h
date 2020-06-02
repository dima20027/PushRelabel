#include<iostream>
#include<fstream>
#include<algorithm>
#include"OURvec.h"

class Graph
{
public:
	Graph();
	~Graph();

	// возвращает максимальный поток от s до t
	int getMaxFlow();

	void addVertexs(string filelink)
	{
		fstream file(filelink, ios::in);
		if (!file.is_open())
		{
			throw invalid_argument("Error - file not open");
		}
		while (!file.eof())
		{
			char symb = ' ';
			Vertex dt;
				file.get(symb);
				bool verNew = true;
				for (int i = 0; i < ver.size(); i++)
				{
					if (ver[i].name == symb)
					{
						verNew = false;
						break;
					}
				}
				if (verNew == true)
				{
					dt.name = symb;

					ver.push_back(dt);
				}
				file.get(symb);
				file.get(symb);
				verNew = true;
				for (int i = 0; i < ver.size(); i++)
				{
					if (ver[i].name == symb)
					{
						verNew = false;
						break;
					}
				}
				if (verNew == true)
				{
					dt.name = symb;
					ver.push_back(dt);
				}
				while (symb != '\n' && !file.eof())
				{
					file.get(symb);
				}
		}
		file.close();

	}

	void addEdge(string flink)
	{
		if (ver.size() == 0)
		{
			throw out_of_range("Error - Vertex no creates");
		}
		fstream file(flink, ios::in);
		if (!file.is_open())
		{
			throw invalid_argument("Error - file not open");
		}
		while (!file.eof())
		{
			Edge dt;
			file.get(dt.uName);
			file.get();
			file.get(dt.vName);
			file.get();
			file >> dt.capacity;
			for (int i = 0; i < ver.size(); i++)
			{
				if (ver[i].name == dt.uName)
				{
					dt.uNumb = i;
				}
				if (ver[i].name == dt.vName)
				{
					dt.vNumb = i;
				}
			}
			edge.push_back(dt);
			file.get();
		}
		cout << endl;
		for (int i = 0; i < edge.size(); i++)
		{
			cout<< edge[i].capacity<<" ";
			cout << edge[i].flow << " ";
			cout << edge[i].uName << "-";
			cout << edge[i].uNumb << " ";
			cout << edge[i].vName << "-";
			cout << edge[i].vNumb <<endl;
		}
	}

	struct Vertex
	{
		char name = ' ';
		int height = 0;
		int e_flow = 0;
	};

private:
	struct Edge
	{
		int flow = 0;
		int capacity = 0;
		char uName = ' ';
		int uNumb = 0;
		char vName = ' ';
		int vNumb = 0;
	};
	OURvector<Edge> edge;
	OURvector<Vertex> ver;

	bool push(int u);
	// Функция для перемаркировки вершины u
	void relabel(int u);
	// Эта функция вызывается для инициализации
	// предварительный поток
	void preflow(int s);
	// Функция для изменения края
	void updateReverseEdgeFlow(int i, int flow);

	// возвращает индекс переполненной вершины
	int overFlowVertex(OURvector<Vertex>& ver)
	{
		for (int i = 1; i < ver.size() - 1; i++)
		{
			if (ver[i].e_flow > 0)
				return i;
		}
		// -1 если переполненная вершина отсутствует
		return -1;
	}

};
Graph::Graph()
{
}
Graph::~Graph()
{
}

void Graph::preflow(int s)
{
	// Делаем h исходной вершины равным no. вершин
	// Высота остальных вершин равна 0.
	ver[s].height = ver.size();
	//
	for (int i = 0; i < edge.size(); i++)
	{
		// Если текущее ребро идет от источника
		if (edge[i].uNumb == s)
		{
			// Поток равен емкости
			edge[i].flow = edge[i].capacity;
			// Инициализируем избыточный поток для соседнего v
			ver[edge[i].vNumb].e_flow += edge[i].flow;
			// Добавить ребро из v в s в остаточном графе с
			// емкость равна 0
			Edge ed;
			ed.flow = -edge[i].flow;
			ed.capacity = 0;
			ed.uNumb = edge[i].vNumb;
			ed.uName = ver[edge[i].vNumb].name;
			ed.vNumb = s;
			ed.vName = ver[s].name;
			edge.push_back(ed);
		}
	}
}

// Обновление обратного потока для потока, добавленного в ih Edge
void Graph::updateReverseEdgeFlow(int i, int flow)
{
	int u = edge[i].vNumb;
	int v = edge[i].uNumb;
	for (int j = 0; j < edge.size(); j++)
	{
		if (edge[j].vNumb == v && edge[j].uNumb == u)
		{
			edge[j].flow -= flow;
			return;
		}
	}
	// добавляем обратный край в остаточный график
	
	Edge ed;
	ed.flow = 0;
	ed.capacity = flow;
	ed.uNumb = u;
	ed.uName = ver[u].name;
	ed.vNumb = v;
	ed.vName = ver[v].name;
	edge.push_back(ed);
}
// Чтобы вытолкнуть поток из переполняющейся вершины u
bool Graph::push(int u)
{
	// Пройдите через все ребра, чтобы найти соседний (из вас)
	// к какому потоку можно подтолкнуть
	for (int i = 0; i < edge.size(); i++)
	{
		// Проверяет u текущего ребра так же, как задано
		// переполняющая вершина
		if (edge[i].uNumb == u)
		{
			// если поток равен пропускной способности, то нет толчка
			// возможно
			if (edge[i].flow == edge[i].capacity)
				continue;
			// Push возможен только если высота смежных
			// меньше высоты переполняющейся вершины
			if (ver[u].height > ver[edge[i].vNumb].height)
			{
				// Поток, который нужно протолкнуть, равен минимуму
				// оставшийся поток на краю и избыточный поток.
				int flow = min(edge[i].capacity - edge[i].flow,
					ver[u].e_flow);
				// Уменьшаем лишний поток для переполняющейся вершины
				ver[u].e_flow -= flow;
				// Увеличить избыточный поток для соседних
				ver[edge[i].vNumb].e_flow += flow;
				// Добавить остаточный поток (с емкостью 0 и отрицательным
				// течь)
				edge[i].flow += flow;
				updateReverseEdgeFlow(i, flow);
				return true;
			}
		}
	}
	return false;
}
// функция для перемаркировки вершины u
void Graph::relabel(int u)
{
	// Инициализируем минимальную высоту соседней
	int mh = INT_MAX;
	// Находим соседний с минимальной высотой
	for (int i = 0; i < edge.size(); i++)
	{
		if (edge[i].uNumb == u)
		{
			// если поток равен емкости, то нет
			// перемаркировка
			if (edge[i].flow == edge[i].capacity)
				continue;
			// Обновляем минимальную высоту
			if (ver[edge[i].vNumb].height < mh)
			{
				mh = ver[edge[i].vNumb].height;
				// Обновление высоты вас
				ver[u].height = mh + 1;
			}
		}
	}
}
// основная функция для печати максимального потока графика
int Graph::getMaxFlow()
{
	if (ver.size() == 0)
	{
		throw out_of_range("Error - Vertex no create");
	}
	if (edge.size() == 0)
	{
		throw out_of_range("Error - Edges no create");
	}
	int s = 0;
	preflow(s);
	// цикл до тех пор, пока ни одна из вершин не будет переполнена
	while (overFlowVertex(ver) != -1)
	{
		int u = overFlowVertex(ver);
		if (!push(u))
			relabel(u);
	}
	// ver.back () возвращает последнюю вершину, чья
	// e_flow будет конечным максимальным потоком
	return ver.back().e_flow;
}