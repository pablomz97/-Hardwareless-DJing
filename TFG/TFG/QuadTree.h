#ifndef QUADTREE_H
#define QUADTREE_H

#include <iostream>
#include <ostream>
#include <string>
#include "Point.h"
#include "Dimension.h"

#include "MidiFunctions.h"

using namespace std;

class QuadTree {
public:
	//const int QT_NODE_CAPACITY = 4;
	Dimension dim;
	void(*functionFader)(string, int);
	void(*functionButton)(string);
	string nameFader;
	string nameButton;

	QuadTree* one;
	QuadTree* two;
	QuadTree* three;
	QuadTree* four;

	/*------------------ CORE TREE FUNCTIONS -----------------*/
	QuadTree();
	QuadTree(Dimension dim);
	~QuadTree();
	void subdivide();
	bool subContains(Dimension d);
	bool isLastFader();
	bool isLastButton();
	bool isLastButtonRec();

	/*------------ FUNCTIONS FOR SEARCHING/INSERTING THE TREE --------------*/
	void searchFader(float x, float y);
	void searchRecFader(float x, float y, string deck);
	void searchButton(float x, float y);
	void searchRecButton(float x, float y, string deck);
	void placeFader(Dimension d, void(*func)(string, int), string name);
	void placeButton(Dimension d, void(*func)(string), string name);

	/*------------ FUNCTIONS FOR PRINTING THE TREE --------------*/
	void printBT(const string& prefix, const QuadTree* node, bool isLeft);
	void printBT();
};

QuadTree createTree();


#endif