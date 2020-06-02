#include "push-relabel.h"
using namespace std;

int main()
{
    Graph mg;
    string flink;
    cout << "Enter file link - ";
    cin >> flink;
    try
    {
        mg.addVertexs(flink);
        mg.addEdge(flink);
    }
    catch (invalid_argument error)
    {
        cout << error.what() << endl;
    }
    cout << endl;
    try
    {
        cout << "maximum flow is " << mg.getMaxFlow();
    }
    catch (out_of_range error)
    {
        cout << error.what() << endl;
    }

    return 0;

}