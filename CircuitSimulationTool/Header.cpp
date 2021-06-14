#include "header.h"


void init(circuit & circ, int n){
	circ.head = NULL;
	circ.num_nodes = n;

	circ.J = new element[MAX_ELEMENTS];
	circ.R = new element[MAX_ELEMENTS];
	circ.E = new element[MAX_ELEMENTS];

	for (int i = 0; i < MAX_ELEMENTS; i++) {
		circ.J[i].value = 0;
		circ.E[i].value = 0;
		circ.R[i].value = 0;
		circ.R[i].second = NULL;
		circ.R[i].first = NULL;
		circ.J[i].second = NULL;
		circ.J[i].first = NULL;
		circ.E[i].second = NULL;
		circ.E[i].first = NULL;
	}
}

node* addthenode(node *& head)
{
	if (head == NULL){
		head = new node;
		head->N = 1;
		head->m = 0;
		head->the_elements = NULL;
		head->next = NULL;
		return head;
	}
	node* p = head;
	int i = 2;
	while (p->next) 
		p = p->next, i++;
	node* t = new node;
	t->N = i;
	t->m = 0;
	t->next = NULL;
	t->the_elements = NULL;
	p->next = t;
	return t;
}

bool check_and_add(circuit cir, char A, int d, node* node0, double value0) //false when no nodes or the element already connected with 2 nodes
{
	if (cir.head == NULL)
		return false;
	double value2;
	element* H = NULL;
	if (A == 'R'){
		H = cir.R;
		value2 = value0;
	}
	if (A == 'E'){
		H = cir.E;
		value2 = -value0;
	}
	if (A == 'J'){
		H = cir.J;
		value2 = -value0;
	}
	int y = d - 1;
	if (y >= 1000 || y<0 || (H[y].first != NULL&& H[y].second != NULL))
		return false;
	if (H[y].second == NULL && H[y].value == value2)
	{
		H[y].second = node0;
		node0->m++;
		add_to_node(node0, H[y]);
		return true;
	}
	if (H[y].first == NULL)
	{

		H[y].type = A;
		H[y].d = d;
		H[y].first = node0;
		H[y].value = value0;
		H[y].value2 = value0;
		node0->m++;
		add_to_node(node0, H[y]);
		if (A == 'E')
			H[y].volt = value0;
		if (A == 'J')
			H[y].curr = value0;
		return true;
	}
	return false;
}

void add_to_node(node *& N, element E)
{
	link* temp = new link;
	temp->element = &E;
	temp->next = NULL;
	if (N->the_elements == NULL)
	{
		N->the_elements = temp;
	}
	else
	{
		link* p = N->the_elements;
		while (p->next != NULL)
			p = p->next;
		p->next = temp;
	}
}

bool check_volt(circuit circ)
{
	bool test1 = false; // test1: there are must R series with volt
	bool test2 = true; //
	if (circ.E[0].first == NULL)
		return true;
	int i = 0;
	int j;
	while (circ.E[i].first != NULL && !(test1) && test2)
	{
		test1 = false;
		node * p = circ.E[i].first;
		node * T = circ.E[i].second;
		if (p->the_elements->next->next == NULL || T->the_elements->next->next == NULL)
			test1 = true;
		j = i;
		while (circ.E[j].value != 0)
		{
			if (circ.E[j].first == p && circ.E[j].second == T && circ.E[i].volt != circ.E[j].volt)
				test2 = false;
			j++;
		}
		i++;
	}
	return (test1 && test2);
}

double superposition(circuit circ, char l, char we[], int g, int s, int d) //l : the thing he want , m[0] :the elemnt we want to calculate and y is the number , m[2] :the element the reson for that and w is the number
{
	int i = 0;
	while (circ.J[i].value)
	{
		if (!(i == s - 1 && we[1] == 'J'))
			circ.J[i].value2 = 0;
		i++;
	}
	i = 0;
	while (circ.E[i].value)
	{
		if (!(i == s - 1 && we[1] == 'E'))
			circ.E[i].value2 = 0;
		i++;
	}

	node_analysis2(circ);

	i = 0;
	while (circ.J[i].first)
	{
		circ.J[i].value2 = circ.J[i].value;
		i++;
	}
	i = 0;
	while (circ.E[i].first)
	{
		circ.E[i].value2 = circ.E[i].value;
		i++;
	}

	i = 0;
	if (l == 'I')
	{
		if (we[0] == 'R') {
			while (circ.R[i].d != g)
			{
				i++;
			}
			return circ.R[i].curr;
		}
		if (we[0] == 'E')
		{
			while (circ.E[i].d != g)
			{
				i++;
			}
			return circ.E[i].curr;
		}
		if (we[0] == 'J')
		{
			while (circ.J[i].d != g)
			{
				i++;
			}
			return circ.J[i].value;
		}
	}
	else if (l == 'V')
	{
		double V1, V2;
		node* t = circ.head;
		while (t != NULL)
		{
			if (t->N = g) V1 = t->volt;
			if (t->N = d) V2 = t->volt;
			t = t->next;
		}

		return (V1 - V2);
	}
	//////////////////////Power if we have time ///////////////////
}

double norton(circuit circ, int g, double & p)
{
	int i = 0;
	while (circ.J[i].first)
	{
		circ.J[i].value2 = 0;
		i++;
	}
	i = 0;
	while (circ.E[i].first)
	{
		circ.E[i].value2 = 0;
		i++;
	}
	i++;
	node * H1 = circ.R[g - 1].first;
	node* H2 = circ.R[g - 1].second;
	circ.E[i].first = H1;
	circ.E[i].second = H2;
	circ.E[i].value2 = circ.R[g - 1].value;
	add_to_node(H1, circ.E[i]);
	add_to_node(H2, circ.E[i]);
	int j = 0;
	while (circ.R[j].first != H1 && circ.R[j].second != H2)
		j++;
	double Ro = circ.R[j].value;
	circ.R[j].value = 9999999999;

	double I, RN;

	node_analysis2(circ);
	I = circ.E[i].curr;
	RN = Ro / I;
	circ.E[i].first = NULL;
	circ.E[i].second = NULL;
	circ.E[i].value2 = 0;
	circ.R[j].value = 0.0000000001;

	node_analysis2(circ);

	I = circ.R[j].curr;
	p = (I * I * RN) / 4;

	i = 0;
	while (circ.J[i].first)
	{
		circ.J[i].value2 = circ.J[i].value;
		i++;
	}
	i = 0;
	while (circ.E[i].first)
	{
		circ.E[i].value2 = circ.E[i].value;
		i++;
	}

	return RN;

}

bool check(circuit circ) // for check all element connected with 2 node 
{
	bool test = true;
	int i = 0;
	while (circ.R[i].value != 0)
	{
		if (circ.R[i].first == NULL || circ.R[i].second == NULL)
		{
			cout << "the element R" << i + 1 << "isn't connected with 2 node";
			test = false;
		}
		i++;
	}
	i = 0;
	while (circ.E[i].value != 0)
	{
		if (circ.E[i].first == NULL || circ.E[i].second == NULL)
		{
			cout << "the element E" << i + 1 << "isn't connected with 2 node";
			test = false;
		}
		i++;
	}
	i = 0;
	while (circ.J[i].value != 0)
	{
		if (circ.J[i].first == NULL || circ.J[i].second == NULL)
		{
			cout << "the element J" << i + 1 << "isn't connected with 2 node";
			test = false;
		}
		i++;
	}
	return test;
}

void node_analysis(circuit &circ)
{
	circ.head->volt = 0; //Node 1 is the Ground node.
	if (!circ.head && !circ.head->next) return;
	node* temp = circ.head->next;
	bool test = false;
	int nodes = circ.num_nodes - 1, Vsources = 0;
	double conductances, Csources = 0;
	for (int i = 0; i < 1000 && circ.E[i].value != 0; i++)
		Vsources++;

	MatrixXd A((nodes + Vsources), (nodes + Vsources));
	VectorXd Z((nodes + Vsources));

	MatrixXd G(nodes, nodes);
	for (int i = 0; i < nodes; i++) //Make G.
	{
		for (int j = 0; j < nodes; j++)
		{
			conductances = 0;
			if (i == j)
			{
				for (int k = 0; k < 1000 && circ.R[k].value != 0; k++)
					if (circ.R[k].first->N == i + 2 || circ.R[k].second->N == i + 2)
						conductances += (1 / circ.R[k].value);
				G(i, j) = conductances;
			}
			else
			{
				test = false;
				for (int k = 0; k < 1000 && circ.R[k].value != 0; k++)
				{
					if ((circ.R[k].first->N == i + 2 && circ.R[k].second->N == j + 2) || (circ.R[k].first->N == j + 2 && circ.R[k].second->N == i + 2))
					{
						G(i, j) = -1 / circ.R[k].value;
						G(j, i) = -1 / circ.R[k].value;
						test = true;
						break;
					}
				}
				if (!test)
				{
					G(i, j) = 0;
					G(j, i) = 0;
				}
			}
		}
	}

	//cout << "G\n" << G << endl << endl;

	MatrixXd B(nodes, Vsources);
	for (int i = 0; (i < 1000 && (circ.E[i].value != 0)); i++) //make B.
	{
		if (circ.E[i].value > 0)
		{
			for (int j = 0; j < nodes; j++)
			{
				if (circ.E[i].first->N == j + 2) B(j, i) = 1;
				else if (circ.E[i].second->N == j + 2) B(j, i) = -1;
				else B(j, i) = 0;
			}
		}
		else if (circ.E[i].value < 0)
		{
			for (int j = 0; j < nodes; j++)
			{
				if (circ.E[i].first->N == j + 2) B(j, i) = -1;
				else if (circ.E[i].second->N == j + 2) B(j, i) = 1;
				else B(j, i) = 0;
			}
		}
	}

	//cout << "B\n" << B << endl << endl;

	MatrixXd C = B.transpose(); // Make C.

	//cout << "C\n" << C << endl << endl;

	MatrixXd D(Vsources, Vsources);
	D = MatrixXd::Zero(Vsources, Vsources); //Make D.

	//cout << "D\n" << D << endl << endl;

	//Make A.
	A.topLeftCorner(nodes, nodes) = G;
	A.topRightCorner(nodes, Vsources) = B;
	A.bottomLeftCorner(Vsources, nodes) = C;
	A.bottomRightCorner(Vsources, Vsources) = D;

	//cout << "A\n" <<  A << endl << endl;

	VectorXd u(nodes);
	for (int i = 0; i < nodes; i++) //Make u.
	{
		Csources = 0;
		for (int j = 0; j < 1000 && circ.J[j].value != 0; j++)
		{
			if (circ.J[j].first->N == (i + 2))
			{
				if (circ.J[i].value > 0) Csources += circ.J[j].value;
				else Csources -= circ.J[j].value;
			}
			else if (circ.J[j].second->N == (i + 2))
			{
				if (circ.J[i].value > 0) Csources += circ.J[j].value;
				else Csources -= circ.J[j].value;
			}
		}
		u(i) = Csources;
	}

	//cout << "u\n" << u << endl << endl;

	VectorXd e(Vsources);
	for (int i = 0; i < Vsources; i++) //Make e.
		e(i) = abs(circ.E[i].value);

	//cout << "e\n" << e << endl << endl;

	//Make Z.
	Z.topRows(nodes) = u;
	Z.bottomRows(Vsources) = e;

	//cout << "Z\n" << Z << endl << endl;

	//Solving A * X = Z.
	VectorXd X = A.colPivHouseholderQr().solve(Z);

	//cout << "X\n" << X << endl << endl;

	//Derivation of solution's elements.
	temp = circ.head->next;
	int k = 0;
	while (temp)
	{
		temp->volt = X(k);
		k++;
		temp = temp->next;
	}
	for (int i = 0; i < 1000 && circ.E[i].value != 0; i++)
	{
		circ.E[i].curr = X(k);
		k++;
	}
	for (int i = 0; i < 1000 && circ.R[i].value != 0; i++)
	{
		if (abs(circ.R[i].first->volt - circ.R[i].second->volt) < 1e5) circ.R[i].volt = circ.R[i].first->volt - circ.R[i].second->volt;
		else circ.R[i].volt = 0;
		if (circ.R[i].volt != 0) circ.R[i].curr = circ.R[i].volt / circ.R[i].value;
		else circ.R[i].curr = 0;
	}
	for (int i = 0; i < 1000 && circ.J[i].value != 0; i++)
		circ.J[i].volt = circ.J[i].first->volt - circ.J[i].second->volt;
}

void node_analysis2(circuit &circ)
{
	circ.head->volt = 0; //Node 1 is the Ground node.
	if (!circ.head && !circ.head->next) return;
	node* temp = circ.head->next;
	bool test = false;
	int nodes = circ.num_nodes - 1, Vsources = 0;
	double conductances, Csources = 0;
	for (int i = 0; i < 1000 && circ.E[i].value != 0; i++)
		Vsources++;

	MatrixXd A((nodes + Vsources), (nodes + Vsources));
	VectorXd Z((nodes + Vsources));

	MatrixXd G(nodes, nodes);
	for (int i = 0; i < nodes; i++) //Make G.
	{
		for (int j = 0; j < nodes; j++)
		{
			conductances = 0;
			if (i == j)
			{
				for (int k = 0; k < 1000 && circ.R[k].value != 0; k++)
					if (circ.R[k].first->N == i + 2 || circ.R[k].second->N == i + 2)
						conductances += (1 / circ.R[k].value2);
				G(i, j) = conductances;
			}
			else
			{
				test = false;
				for (int k = 0; k < 1000 && circ.R[k].value != 0; k++)
				{
					if ((circ.R[k].first->N == i + 2 && circ.R[k].second->N == j + 2) || (circ.R[k].first->N == j + 2 && circ.R[k].second->N == i + 2))
					{
						G(i, j) = -1 / circ.R[k].value2;
						G(j, i) = -1 / circ.R[k].value2;
						test = true;
						break;
					}
				}
				if (!test)
				{
					G(i, j) = 0;
					G(j, i) = 0;
				}
			}
		}
	}

	//cout << G << endl << endl;

	MatrixXd B(nodes, Vsources);
	for (int i = 0; (i < 1000 && (circ.E[i].value != 0)); i++) //make B.
	{
		if (circ.E[i].value2 > 0)
		{
			for (int j = 0; j < nodes; j++)
			{
				if (circ.E[i].first->N == j + 2) B(j, i) = 1;
				else if (circ.E[i].second->N == j + 2) B(j, i) = -1;
				else B(j, i) = 0;
			}
		}
		else if (circ.E[i].value2 < 0)
		{
			for (int j = 0; j < nodes; j++)
			{
				if (circ.E[i].first->N == j + 2) B(j, i) = -1;
				else if (circ.E[i].second->N == j + 2) B(j, i) = 1;
				else B(j, i) = 0;
			}
		}
	}

	//cout << B << endl << endl;

	MatrixXd C = B.transpose(); // Make C.

								//cout << C << endl << endl;

	MatrixXd D(Vsources, Vsources);
	D = MatrixXd::Zero(Vsources, Vsources); //Make D.

											//cout << D << endl << endl;

											//Make A.
	A.topLeftCorner(nodes, nodes) = G;
	A.topRightCorner(nodes, Vsources) = B;
	A.bottomLeftCorner(Vsources, nodes) = C;
	A.bottomRightCorner(Vsources, Vsources) = D;

	//cout << A << endl << endl;

	VectorXd u(nodes);
	for (int i = 0; i < nodes; i++) //Make u.
	{
		Csources = 0;
		for (int j = 0; j < 1000 && circ.J[j].value != 0; j++)
		{
			if (circ.J[j].first->N == (i + 2))
			{
				if (circ.J[i].value2 > 0) Csources += circ.J[j].value2;
				else Csources -= circ.J[j].value2;
			}
			else if (circ.J[j].second->N == (i + 2))
			{
				if (circ.J[i].value2 > 0) Csources += circ.J[j].value2;
				else Csources -= circ.J[j].value2;
			}
		}
		u(i) = Csources;
	}

	//cout << u << endl << endl;

	VectorXd e(Vsources);
	for (int i = 0; i < Vsources; i++) //Make e.
		e(i) = abs(circ.E[i].value2);

	//cout << e << endl << endl;

	//Make Z.
	Z.topRows(nodes) = u;
	Z.bottomRows(Vsources) = e;

	//cout << Z << endl << endl;

	//Solving A * X = Z.
	VectorXd X = A.colPivHouseholderQr().solve(Z);

	//cout << X << endl << endl;

	//Derivation of solution's elements.
	temp = circ.head->next;
	int k = 0;
	while (temp)
	{
		temp->volt = X(k);
		k++;
		temp = temp->next;
	}
	for (int i = 0; i < 1000 && circ.E[i].value != 0; i++)
	{
		circ.E[i].curr = X(k);
		k++;
	}
	for (int i = 0; i < 1000 && circ.R[i].value != 0; i++)
	{
		if (abs(circ.R[i].first->volt - circ.R[i].second->volt) < 1e5) circ.R[i].volt = circ.R[i].first->volt - circ.R[i].second->volt;
		else circ.R[i].volt = 0;
		if (circ.R[i].volt != 0) circ.R[i].curr = circ.R[i].volt / circ.R[i].value2;
		else circ.R[i].curr = 0;
	}
	for (int i = 0; i < 1000 && circ.J[i].value != 0; i++)
		circ.J[i].volt = circ.J[i].first->volt - circ.J[i].second->volt;
}

double R_parr(double R1, double R2)
{
	return (R1*R2 / (R1 + R2));
}