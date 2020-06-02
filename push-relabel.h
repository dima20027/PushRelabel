#include<iostream>
#include<fstream>
#include<algorithm>
#include"OURvec.h"

class Graph
{
public:
	Graph();
	~Graph();

	// ���������� ������������ ����� �� s �� t
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
	// ������� ��� �������������� ������� u
	void relabel(int u);
	// ��� ������� ���������� ��� �������������
	// ��������������� �����
	void preflow(int s);
	// ������� ��� ��������� ����
	void updateReverseEdgeFlow(int i, int flow);

	// ���������� ������ ������������� �������
	int overFlowVertex(OURvector<Vertex>& ver)
	{
		for (int i = 1; i < ver.size() - 1; i++)
		{
			if (ver[i].e_flow > 0)
				return i;
		}
		// -1 ���� ������������� ������� �����������
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
	// ������ h �������� ������� ������ no. ������
	// ������ ��������� ������ ����� 0.
	ver[s].height = ver.size();
	//
	for (int i = 0; i < edge.size(); i++)
	{
		// ���� ������� ����� ���� �� ���������
		if (edge[i].uNumb == s)
		{
			// ����� ����� �������
			edge[i].flow = edge[i].capacity;
			// �������������� ���������� ����� ��� ��������� v
			ver[edge[i].vNumb].e_flow += edge[i].flow;
			// �������� ����� �� v � s � ���������� ����� �
			// ������� ����� 0
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

// ���������� ��������� ������ ��� ������, ������������ � ih Edge
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
	// ��������� �������� ���� � ���������� ������
	
	Edge ed;
	ed.flow = 0;
	ed.capacity = flow;
	ed.uNumb = u;
	ed.uName = ver[u].name;
	ed.vNumb = v;
	ed.vName = ver[v].name;
	edge.push_back(ed);
}
// ����� ���������� ����� �� ��������������� ������� u
bool Graph::push(int u)
{
	// �������� ����� ��� �����, ����� ����� �������� (�� ���)
	// � ������ ������ ����� �����������
	for (int i = 0; i < edge.size(); i++)
	{
		// ��������� u �������� ����� ��� ��, ��� ������
		// ������������� �������
		if (edge[i].uNumb == u)
		{
			// ���� ����� ����� ���������� �����������, �� ��� ������
			// ��������
			if (edge[i].flow == edge[i].capacity)
				continue;
			// Push �������� ������ ���� ������ �������
			// ������ ������ ��������������� �������
			if (ver[u].height > ver[edge[i].vNumb].height)
			{
				// �����, ������� ����� �����������, ����� ��������
				// ���������� ����� �� ���� � ���������� �����.
				int flow = min(edge[i].capacity - edge[i].flow,
					ver[u].e_flow);
				// ��������� ������ ����� ��� ��������������� �������
				ver[u].e_flow -= flow;
				// ��������� ���������� ����� ��� ��������
				ver[edge[i].vNumb].e_flow += flow;
				// �������� ���������� ����� (� �������� 0 � �������������
				// ����)
				edge[i].flow += flow;
				updateReverseEdgeFlow(i, flow);
				return true;
			}
		}
	}
	return false;
}
// ������� ��� �������������� ������� u
void Graph::relabel(int u)
{
	// �������������� ����������� ������ ��������
	int mh = INT_MAX;
	// ������� �������� � ����������� �������
	for (int i = 0; i < edge.size(); i++)
	{
		if (edge[i].uNumb == u)
		{
			// ���� ����� ����� �������, �� ���
			// ��������������
			if (edge[i].flow == edge[i].capacity)
				continue;
			// ��������� ����������� ������
			if (ver[edge[i].vNumb].height < mh)
			{
				mh = ver[edge[i].vNumb].height;
				// ���������� ������ ���
				ver[u].height = mh + 1;
			}
		}
	}
}
// �������� ������� ��� ������ ������������� ������ �������
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
	// ���� �� ��� ���, ���� �� ���� �� ������ �� ����� �����������
	while (overFlowVertex(ver) != -1)
	{
		int u = overFlowVertex(ver);
		if (!push(u))
			relabel(u);
	}
	// ver.back () ���������� ��������� �������, ���
	// e_flow ����� �������� ������������ �������
	return ver.back().e_flow;
}