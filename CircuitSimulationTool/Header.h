#include <iostream>
#include <cmath>
#include <Eigen/Dense>
using namespace Eigen;
using namespace std;

struct node;
struct element;
struct circuit;
struct link;

struct node
{
	int N; //number of node
	int m; //number of elements connected to the node
	link* the_elements;
	node* next;
	double  volt;
};

struct link
{
	element* element;
	link* next;
};

struct element
{
	char type;
	int d;  // the number of the element - the place of element at array = d-1
	double  value;
	double value2; //when we moved the current source to volt and R series
	double  volt;
	double  curr;
	node* first;
	node* second;
};

struct circuit
{
	node* head;
	int num_nodes;
	element* R;
	element* E;
	element* J;
};


double norton(circuit, int, double &);
bool check_volt(circuit);
bool check(circuit);
void init(circuit &);
double superposition(circuit, char, char[], int, int, int);
void node_analysis2(circuit &);
void node_analysis(circuit &);
void add_to_node(node *& N, element E);
bool check_and_add(circuit cir, char A, int d, node* node0, double value0);
node* addthenode(node *& head);
double R_parr(double  R1, double  R2);