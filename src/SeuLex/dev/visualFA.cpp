#include<bits/stdc++.h>
#include"visualFA.hpp";
using namespace std;

int main()
{
    //vector<int>dC;
    //vector<edge<int>>eP;

    visualFA<int> vfa;
    vfa.addNode(0);
    vfa.addNode(3);
    vfa.addNode(4);
    vfa.addNode(8);

    vfa.addEdge(0,1,"SS(S)");
    vfa.addEdge(0,2,"SS(B)");
    vfa.addEdge(1,3,"S($end)");
    vfa.addEdge(2,4,"S(A)");
    vfa.addEdge(2,6,"SS(b)");
    vfa.addEdge(6,6,"S(b)");
    vfa.addEdge(6,5,"S(a)");
    vfa.addEdge(2,5,"SS(a)");
    vfa.addEdge(5,5,"S(a)");
    vfa.addEdge(5,7,"S(b)");
    vfa.addEdge(7,5,"S(a)");
    vfa.addEdge(7,8,"S(b)");
    vfa.addEdge(8,5,"S(a)");
    vfa.addEdge(8,6,"S(b)");

    vfa.print();
    return 0;
}