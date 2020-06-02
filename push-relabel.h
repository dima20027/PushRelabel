#include<iostream>
#include<fstream>
#include<algorithm>
#include"OURvec.h"

class Graph
{
public:
	Graph();
	~Graph();
	int getMaxFlow();
	void addVertex(string filelink)
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
			for (int i = 0; i < vertex.size(); i++)
			{
				if (vertex[i].name == symb)
				{
					verNew = false;
					break;
				}
			}
			if (verNew == true)
			{
				dt.name = symb;
				vertex.push_back(dt);
			}
			file.get(symb);
			if (symb != ' ')
			{
				throw invalid_argument("Your file not correct!");
			}
			file.get(symb);
			verNew = true;
			for (int i = 0; i < vertex.size(); i++)
			{
				if (vertex[i].name == symb)
				{
					verNew = false;
					break;
				}
			}
			if (verNew == true)
			{
				dt.name = symb;
				vertex.push_back(dt);
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
		if (vertex.size() == 0)
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
			char symb;
			file.get(dt.uName);
			file.get(symb);
			if (symb != ' ')
			{
				throw invalid_argument("Your file not correct!");
			}
			file.get(dt.vName);
			file.get(symb);
			if (symb != ' ')
			{
				throw invalid_argument("Your file not correct!");
			}
			file >> dt.capacity;
			for (int i = 0; i < vertex.size(); i++)
			{
				if (vertex[i].name == dt.uName)
				{
					dt.uNumb = i;
				}
				if (vertex[i].name == dt.vName)
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
		int eFlow = 0;
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
	OURvector<Vertex> vertex;

	bool push(int uNumb);
	void relabel(int uNumb);
	void preflow(int indexStart);
	void updateReverseEdgeFlow(int i, int flow);
	int overFlowVertex(OURvector<Vertex>& vertex)
	{
		for (int i = 1; i < vertex.size() - 1; i++)
		{
			if (vertex[i].eFlow > 0)
				return i;
		}
		return -1;
	}

};
Graph::Graph()
{
}
Graph::~Graph()
{
}

void Graph::preflow(int indexStart)
{

	vertex[indexStart].height = vertex.size();
	for (int i = 0; i < edge.size(); i++)
	{
		if (edge[i].uNumb == indexStart)
		{
			edge[i].flow = edge[i].capacity;
			vertex[edge[i].vNumb].eFlow += edge[i].flow;
			Edge nEdge;
			nEdge.flow = -edge[i].flow;
			nEdge.capacity = 0;
			nEdge.uNumb = edge[i].vNumb;
			nEdge.uName = vertex[edge[i].vNumb].name;
			nEdge.vNumb = indexStart;
			nEdge.vName = vertex[indexStart].name;
			edge.push_back(nEdge);
		}
	}
}

void Graph::updateReverseEdgeFlow(int indexArr, int flow)
{
	int uNumb = edge[indexArr].vNumb;
	int vNumb = edge[indexArr].uNumb;
	for (int j = 0; j < edge.size(); j++)
	{
		if (edge[j].vNumb == vNumb && edge[j].uNumb == uNumb)
		{
			edge[j].flow -= flow;
			return;
		}
	}
	Edge nEdge;
	nEdge.flow = 0;
	nEdge.capacity = flow;
	nEdge.uNumb = uNumb;
	nEdge.uName = vertex[uNumb].name;
	nEdge.vNumb = vNumb;
	nEdge.vName = vertex[vNumb].name;
	edge.push_back(nEdge);
}

bool Graph::push(int uNumb)
{
	for (int i = 0; i < edge.size(); i++)
	{
		if (edge[i].uNumb == uNumb)
		{
			if (edge[i].flow == edge[i].capacity)
				continue;
			if (vertex[uNumb].height > vertex[edge[i].vNumb].height)
			{
				int flow = min(edge[i].capacity - edge[i].flow,
					vertex[uNumb].eFlow);//≈динственна€ строка за которую могу по€снить
				vertex[uNumb].eFlow -= flow;
				vertex[edge[i].vNumb].eFlow += flow;
				edge[i].flow += flow;
				updateReverseEdgeFlow(i, flow);
				return true;
			}
		}
	}
	return false;
}
void Graph::relabel(int uNumb)
{
	int maxHeight = INT_MAX;
	for (int i = 0; i < edge.size(); i++)
	{
		if (edge[i].uNumb == uNumb)
		{
			if (edge[i].flow == edge[i].capacity)
				continue;
			if (vertex[edge[i].vNumb].height < maxHeight)
			{
				maxHeight = vertex[edge[i].vNumb].height;
				vertex[uNumb].height = maxHeight + 1;
			}
		}
	}
}
int Graph::getMaxFlow()
{
	if (vertex.size() == 0)
	{
		throw out_of_range("Error - Vertex no create");
	}
	if (edge.size() == 0)
	{
		throw out_of_range("Error - Edges no create");
	}
	int indexStart = 0;
	preflow(indexStart);
	while (overFlowVertex(vertex) != -1)
	{
		int overFlowIndex = overFlowVertex(vertex);
		if (!push(overFlowIndex))
			relabel(overFlowIndex);
	}
	return vertex.back().eFlow;
}