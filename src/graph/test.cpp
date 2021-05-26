#include<iostream>
#include "VisualFinateAutomaton.h";
using namespace std;

void main()
{
    Dot dot0(0,0),dot1(1,1),dot2(2,1),dot3(3,0),dot4(4,0),dot5(5,1),dot6(6,1),dot7(7,1),dot8(8,0);
    
    Edge e0("SS(S)",dot0,dot1);
    Edge e1("SS(B)",dot0,dot2);
    Edge e2("S($end)",dot1,dot3);
    Edge e3("S(A)",dot2,dot4);
    Edge e4("SS(b",dot2,dot6);
    Edge e5("S(b)",dot6,dot6);
    Edge e6("SS(a)",dot2,dot5);
    Edge e7("S(a)",dot6,dot5);
    Edge e8("S(a)",dot5,dot5);
    Edge e9("S(a)",dot7,dot5);
    Edge e10("S(b)",dot5,dot7);
    Edge e11("S(a)",dot8,dot5);
    Edge e12("S(b)",dot7,dot8);
    Edge e13("S(b)",dot8,dot7);
}