#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include "Header.h"
using namespace std;

/*
team
name: Yahia Ali, sec: 2, B.N.: 32
name: Omar Abd Elkareem, sec: 2, B.N.: 9
name: Waleed Mohammed, sec: 2, B.N.: 30
*/

void main() {
	//freopen("input.txt", "r", stdin);
	cout << "Circuits Project - Please enter the input correctly \n";
	cout << "Enter the total number of elements\n";
	int m;
	cin >> m;
	while (m <= 1)
	{
		cout << "Incorrect. Please enter another number\n";
		cin >> m;
	}
	cout << "Each element must connect with exactly 2 nodes \n"
		<< "For voltage source: Enter positive value at the positive node and negtive value at the other node \n"
		<< "For current source: Enter positive value at the node that current runs to and negtive value at the other node\n"
		<< "For R : Enter a positive number\n "
		<< "If any input is wrong , the program will end \n";
	cout << "Any number of elements can be inserted, but the type must be entered before the value \n  "
		<< "Enter an element type (R, J or E) followed by its order. Then enter the value after a space. \n"
		<< "then enter the value with the rules of elements and node ,final Ex : R1   50  ..so on \n, pleas don't put any number before use the numbers less than it  ";
	circuit circ;
	init(circ);
	circ.head = NULL;
	circ.num_nodes = m;
	char u;
	int d;
	for (int i = 1; i <= m; i++)
	{
		node* t = addthenode(circ.head);
		cout << "\nPlease enter all element connect to node " << i << " then enter -1\n";
		cin >> u;
		cin >> d;
		while (u != '-')
		{
			double j;
			cin >> j; //Value
			bool test = true;
			if ((u == 'R' && j <= 0) || d <= 0)
			{
				cout << "Error at this element.";
				return;
			}
			test = check_and_add(circ, u, d, t, j);
			if (!test)
			{
				cout << "error at this element";
				return;
			}
			cout << "enter the next element \n";
			cin >> u;
			cin >> d;
		}
	}

	if (!check_volt(circ))
	{
		cout << "\nThere are error at the volt , the error is from there are ideal source or there are 2 voltge source parallel ";
		return;
	}
	if (!check(circ))
	{
		cout << "\n there are error , the error is from there are element is connected with only one node ";
		return;
	}

	cout << " \n now the circuit is complete "
		<< "\n  Enter the type and location of the required response (I, V and P for current, voltage and power, respectively). For I and P, specify the element name. For V, specify the nodes numbers"
		<< "\n please enter the fist letter capital,  enter -1 for end the program\n";
	char l;
	char we[10];

	node_analysis(circ);

	cout << "Enter the required\n";
	cin >> l;
	int g;
	int s;
	while (l != '-')
	{

		if (l == 'I')
		{
			cin >> we[0];
			cin >> g;
			while (g <= 0)
			{
				cout << "\nError, there are no element or node named, enter the name again: ";
				cin >> g;
			}
			cout << "\nIf you want current from only one source enter the name of source , else enter 00";
			cin >> we[1] >> s;
			if (we[1] == '0' && s == 0)
			{
				if (we[0] == 'E')
					cout << "The current on E" << g << " = " << circ.E[g - 1].curr << " Amper" << " from node " << circ.E[g - 1].second->N << " to node " << circ.E[g - 1].first->N;
				if (we[0] == 'R')
					cout << "The current on R" << g << " = " << circ.R[g - 1].curr << " Amper" << " from node " << circ.R[g - 1].second->N << " to node " << circ.R[g - 1].first->N;
				if (we[0] == 'J')
					cout << "The current on J" << g << " = " << circ.J[g - 1].value << " Amper" << " from node " << circ.J[g - 1].second->N << " to node " << circ.J[g - 1].first->N;
			}
			else
			{
				if (we[0] == 'E')
					cout << "The current on E" << g << " = " << superposition(circ, l, we, g, s, 0) << " Amper" << " from node " << circ.E[g - 1].second->N << " to node " << circ.E[g - 1].first->N;
				if (we[0] == 'R')
					cout << "The current on R" << g << " = " << superposition(circ, l, we, g, s, 0) << " Amper" << " from node " << circ.R[g - 1].second->N << " to node " << circ.R[g - 1].first->N;
				if (we[0] == 'J')
					cout << "The current on J" << g << " = " << superposition(circ, l, we, g, s, 0) << " Amper" << " from node " << circ.J[g - 1].second->N << " to node " << circ.J[g - 1].first->N;
			}
		}
		if (l == 'V')
		{
			cin >> g >> d;
			cout << "\nIf you want voltage from only one source enter the name of source, else enter 00";
			cin >> we[1] >> s;
			if (we[1] == '0' && s == 0)
			{
				node* temp = circ.head;
				double V1, V2;
				while (temp)
				{
					if (temp->N == g) V1 = temp->volt;
					if (temp->N == d) V2 = temp->volt;
					temp = temp->next;
				}
				cout << "The voltage between " << g << " & " << d << " = " << V1 - V2 << " volt";
			}
			else
			{
				if (we[2] == 'R')
					cout << "Error!";
				else
					cout << "The voltage between " << g << " & " << d << " = " << superposition(circ, l, we, g, s, d) << " volt";
			}
		}
		if (l == 'R')
		{
			element *G = circ.R;
			double  power = 0;
			cin >> we[0] >> we[1] >> we[2] >> we[3] >> we[4] >> we[5] >> we[6] >> we[7] >> g;
			cout << "Rmax for " << we[7] << we[8] << " = " << norton(circ, g, power) << "\n and Pmax = " << power << " watt\n";
		}
		if (l == 'P')
		{
			cin >> we[0] >> g;
			if (we[0] == 'E')
				cout << "\nThe power = " << (circ.E[g - 1].curr)*(circ.E[g - 1].volt) << " watt\n";
			if (we[0] == 'R')
				cout << "\nThe power = " << (circ.R[g - 1].curr)*(circ.R[g - 1].volt) << " watt\n";
			if (we[0] == 'J')
				cout << "\nThe power = " << (circ.J[g - 1].curr)*(circ.J[g - 1].volt) << " watt\n";
		}
		cout << "\nEnter the required: \n";
		cin >> l;
	}

	cout << "\tThe end of program. \n";
}



/*
double delta( node * L)
{
bool test = true;
int i=0;
element *q1;
element *q2;
element *q3;
q1->second=NULL;
q2->second=NULL;
q3->second=NULL;
node * p ;
if ( L!= L->the_element->first)
p =L->the_element->first;
else
p =L->the_element->second;
node * the_last = p ;
while ( test)
{
if ( p == the_last )
{
i++;
}

}
}*/

/*
double find_volt(circuit circ, int x, int z)
{
node* p = circ.head;
int i = 0;
while (i < x && p)
{
p = p->next;
i++;
}
if (p == NULL)
{
cout << "\n error , there are no element with this name \n ";
return 0;
}
link * t = p->the_elements;
while (t)
{
if (t->th == z)
return -(t->volt);
if (t->second->N == z)
return t->volt;
t = t->next;
}
t = p->the_element;
double the_last=0;
double sum = the_last;
while (t->first->N != z && t->second->N != z)
{
if (t->volt == the_last || t->volt == -the_last)
t = t->next;
if (t->first == p)
the_last = -(t->volt);
else
the_last = (t->volt);
sum = sum + the_last;
t = p->the_element;
if (p != t->first)
p = t->first;
else
p = t->second;
}
return sum;

}
*/

/*node * check_node_Analy( node* node0)
{
if( node0->the_elements->element->type == 'E' && node0->the_elements->next->element->type =='R' &&  node0->the_elements->next->next == NULL)
return node0;
if( node0->the_elements->element->type =='R' && node0->the_elements->next->element->type =='E' &&  node0->the_elements->next->next == NULL)
return node0;
return NULL;
}*/