#include "push-relabel.h"
using namespace std;

int main()
{
    Graph mg;
    string flink;
    bool ret = true;
    while (ret == true)
    {
        cout << "Enter file link - ";
        cin >> flink;
        try
        {
            mg.addVertex(flink);
            mg.addEdge(flink);
            ret = false;
        }
        catch (invalid_argument error)
        {
            cout << error.what() << endl;
        }
        cout << endl;
    }
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