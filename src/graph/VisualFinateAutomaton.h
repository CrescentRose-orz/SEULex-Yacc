#include<iostream>
#include<fstream>
#include<string>
#include<cstdio>
using namespace std;

class Graph
{
public:
    int *circle=new int[10000];
    int *doublecircle=new int[10000];
    int i=0,j=0;
    Dot dot;
    Edge *edge=new Edge[10000];
    Graph();
    void GoToArray(int s,int n)
    {
        if(s==0)
        {
            doublecircle[j]=n;
            j++;
        }
        else
        {
            circle[i]=n;
            i++;
        }

    }

    void Display()
    {
        cout<<"digraph finite_state_machine {"<<endl<<"rankdir=LR;"<<endl;
        cout<<"size = \""<<i+j<<","<<i<<"\""<<endl;

        cout<<"node [shape = doublecircle]; ";
        for(int m=0;m<i;m++)
        {
            if(m!=i-1)
            {
              cout<<doublecircle[m]<<", ";
            }
        }
        cout<<endl;

        cout<<"node [shape = circle];"<<endl;

        for(int k=0;k<sizeof(edge);k++)
        {
            cout<<edge[k].from.name<<" -> "<<edge[k].go.name<<" [Label =\" "<<edge[k].Label<<"\" ]"<<endl;
        }
        cout<<"}"<<endl;
    }

    friend class Dot;
    friend class Edge;
};

class Dot
{
public:
    int status;//显示是不是起点和终点
    int name;
    Dot();
    Dot(int s,int n)
    {
        status=s;name=n;
        if(status==0)
        {
            Graph::doublecircle[GRaph::j]=name;
            j++;
        }
        else
        {
            Graph::circle[i]=name;
            i++;
        }
    }
    
    friend class Edge;
    friend class Graph;
    
};

class Edge
{
public:
    string Label;
    Dot from;
    Dot go;
    Edge();
    Edge(string l,Dot f,Dot g)
    {
        Label=l;
        from=f;
        go=g;
    }

    friend class Dot;
};

