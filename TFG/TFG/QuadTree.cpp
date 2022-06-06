#include "QuadTree.h"

QuadTree::QuadTree() {
	dim = Dimension();
	functionFader = nullptr;
	functionButton = nullptr;
	nameFader = "";
	nameButton = "";

	one = nullptr;
	two = nullptr;
	three = nullptr;
	four = nullptr;
}

QuadTree::QuadTree(Dimension dim) {
	this->dim = dim;
	functionFader = nullptr;
	functionButton = nullptr;
	nameFader = "";
	nameButton = "";

	one = nullptr;
	two = nullptr;
	three = nullptr;
	four = nullptr;
}

QuadTree::~QuadTree() {
	delete one;
	delete two;
	delete three;
	delete four;
}

/* Post: Creates children for this QuadTree */
void QuadTree::subdivide() {
	//CHECK THE DIVISIONS SINCE THE LINES ARE SHARED AT THE MOMENT (ADD 1 OR REMOVE 1 FROM SOME BOUNDARIES)

	one = new QuadTree(Dimension(dim.minX, dim.minY, dim.centX, dim.centY));
	two = new QuadTree(Dimension(dim.centX, dim.minY, dim.maxX, dim.centY));
	three = new QuadTree(Dimension(dim.centX, dim.centY, dim.maxX, dim.maxY));
	four = new QuadTree(Dimension(dim.minX, dim.centY, dim.centX, dim.maxY));
}

/* Post: Checks whether the Dimension d is inside this QuadTree */
bool QuadTree::subContains(Dimension d) {
	//quadtree is leaf -> return false
	if (one == nullptr) { return false; }
	//check if children contain d
	if (d.isContained(one->dim) || d.isContained(two->dim) || d.isContained(three->dim) || d.isContained(four->dim)) {
		return true;
	}
	return false;
}

/* Post: Returns true if current Quadtree is the last one with a Fader function*/
bool QuadTree::isLastFader() {
	//checks if fader function is nullptr
	if (this->functionFader != nullptr) {
		return false;
	}//checks if QuadTree has children
	else if (this->one == nullptr) {
		return true;
	}

	//if fader function is nullptr and node has children, check the children for functions
	return (this->one->isLastFader() && this->two->isLastFader() && this->three->isLastFader() && this->four->isLastFader());
}

/* Post: Returns true if current Quadtree is the last one with a Button function*/
bool QuadTree::isLastButton() {
	//checks if QuadTree has children
	if (this->one == nullptr) {
		return true;
	}

	//if button function is nullptr and node has children, check the children for functions
	return (this->one->isLastButtonRec() && this->two->isLastButtonRec() && this->three->isLastButtonRec() && this->four->isLastButtonRec());
}

/* Post: Returns true if current Quadtree is the last one with a Button function*/
bool QuadTree::isLastButtonRec() {
	//checks if button function is nullptr
	if (this->functionButton != nullptr) {
		return false;
	}
	//checks if QuadTree has children
	if (this->one == nullptr) {
		return true;
	}

	//if button function is nullptr and node has children, check the children for functions
	return (this->one->isLastButton() && this->two->isLastButton() && this->three->isLastButton() && this->four->isLastButton());
}


/* Post: Calls searchRecFader with the deck assigned or executes Fader func if on children*/
void QuadTree::searchFader(float x, float y) {
	string deck = "";
	//Exit condition
	if (isLastFader()) {
		//calculate appropiate value with scale
		int value = 0;
		//midi function call
		if (functionFader != nullptr) {
			functionFader(deck, value);
		}
		return;
	}
	//Recursive case
	if (x < dim.centX) {
		deck = "A";
		if (y < dim.centY) {
			one->searchRecFader(x, y, deck);
		}
		else {
			four->searchRecFader(x, y, deck);
		}
	}
	else {
		deck = "B";
		if (y < dim.centY) {
			two->searchRecFader(x, y, deck);
		}
		else {
			three->searchRecFader(x, y, deck);
		}
	}
}

/* Post: Executes the functionFader assigned to the Position */
void QuadTree::searchRecFader(float x, float y, string deck) {
	//Exit condition
	if (isLastFader()) {
		//calculate appropiate value with scale
		int value=0;
		//midi function call if it exists
		if (functionFader != nullptr) {
			functionFader(deck, value);
		}
		return;
	}
	//Recursive case
	if (x < dim.centX) {	
		if (deck == "") { deck = "A"; }
		if (y < dim.centY) {
			one->searchRecFader(x, y, deck);
		}
		else {
			four->searchRecFader(x, y, deck);
		}
	}else{
		if (deck == "") { deck = "B"; }
		if (y < dim.centY) {
			two->searchRecFader(x, y, deck);
		}
		else {
			three->searchRecFader(x, y, deck);
		}
	}
}

/* Post: Calls searchRecButton with the deck assigned or executes Fader func if on children*/
void QuadTree::searchButton(float x, float y) {
	string deck = "";
	//Exit condition
	if (isLastButton()) {
		//calculate appropiate value with scale
		int value = 0;
		//midi function call
		if (functionButton != nullptr) {
			functionButton(deck);
		}

		cout << nameButton << endl;

		return;
	}
	//Recursive case
	if (x < dim.centX) {
		deck = "A";
		if (y < dim.centY) {
			one->searchRecButton(x, y, deck);
		}
		else {
			four->searchRecButton(x, y, deck);
		}
	}
	else {
		deck = "B";
		if (y < dim.centY) {
			two->searchRecButton(x, y, deck);
		}
		else {
			three->searchRecButton(x, y, deck);
		}
	}
}

/* Post: Executes the functionButton assigned to the Position */
void QuadTree::searchRecButton(float x, float y, string deck) {
	//Exit condition
	if (isLastButton()) {
		//calculate appropiate value with scale
		int value = 0;
		//midi function call if it exists
		if (functionButton != nullptr) {
			functionButton(deck);
		}
		return;
	}
	//Recursive case
	if (x < dim.centX) {
		if (deck == "") { deck = "A"; }
		if (y < dim.centY) {
			one->searchRecButton(x, y, deck);
		}
		else {
			four->searchRecButton(x, y, deck);
		}
	}
	else {
		if (deck == "") { deck = "B"; }
		if (y < dim.centY) {
			two->searchRecButton(x, y, deck);
		}
		else {
			three->searchRecButton(x, y, deck);
		}
	}
}

/*
 * Pre: d is contained in QuadTree
 * Post: Inserts func into all the quadtrees which sum is Dimension in the fader handler
 */
void QuadTree::placeFader(Dimension d, void(*func)(string, int), string name) {

	if (d == dim) {
		functionFader = func;
		nameFader = name;
		return;
	}

	if (one == nullptr) {
		//d is contained in QuadTree since it is a precondition to enter
		subdivide();
		//if a function already exists fill the children with such function(new added function will override the needed spots)
		if (functionFader != nullptr) {
			placeFader(dim, functionFader, name);
		}
	}

	//see if d is fully contained in any of the node children
	if (d.isContained(one->dim)) {
		one->placeFader(d, func, name);
	}else if (d.isContained(two->dim)) {
		two->placeFader(d, func, name);
	}else if (d.isContained(three->dim)) {
		three->placeFader(d, func, name);
	}else if (d.isContained(four->dim)) {
		four->placeFader(d, func, name);
	}
	//it is not contained fully in any children, so d must be part of 2 minimum
	else {
		//one is part of d
		if (one->dim.isContained(d) || one->dim.intersects(d)) {
			Dimension y = d.cut(one->dim);
			one->placeFader(y, func, name);
		}
		//two is part of d
		if (two->dim.isContained(d) || two->dim.intersects(d)) {
			Dimension y = d.cut(two->dim);
			two->placeFader(y, func, name);
		}
		//three is part of d
		if (three->dim.isContained(d) || three->dim.intersects(d)) {
			Dimension y = d.cut(three->dim);
			three->placeFader(y, func, name);
		}
		//four is part of d
		if (four->dim.isContained(d) || four->dim.intersects(d)) {
			Dimension y = d.cut(four->dim);
			four->placeFader(y, func, name);
		}
	}
}

/*
 * Pre: d is contained in QuadTree
 * Post: Inserts func into all the quadtrees which sum is Dimension in the button handler
 */
void QuadTree::placeButton(Dimension d, void(*func)(string), string name) {

	if (d == dim) {
		functionButton = func;
		nameButton = name;
		return;
	}

	if (one == nullptr) {
		//d is contained in QuadTree since it is a precondition to enter
		subdivide();
		//if a function already exists fill the children with such function(new added function will override the needed spots)
		if (functionButton != nullptr) {
			placeButton(dim, functionButton, name);
		}
	}

	//see if d is fully contained in any of the node children
	if (d.isContained(one->dim)) {
		one->placeButton(d, func, name);
	}
	else if (d.isContained(two->dim)) {
		two->placeButton(d, func, name);
	}
	else if (d.isContained(three->dim)) {
		three->placeButton(d, func, name);
	}
	else if (d.isContained(four->dim)) {
		four->placeButton(d, func, name);
	}
	//it is not contained fully in any children, so d must be part of 2 minimum
	else {
		//one is part of d
		if (one->dim.isContained(d) || one->dim.intersects(d)) {
			Dimension y = d.cut(one->dim);
			one->placeButton(y, func, name);
		}
		//two is part of d
		if (two->dim.isContained(d) || two->dim.intersects(d)) {
			Dimension y = d.cut(two->dim);
			two->placeButton(y, func, name);
		}
		//three is part of d
		if (three->dim.isContained(d) || three->dim.intersects(d)) {
			Dimension y = d.cut(three->dim);
			three->placeButton(y, func, name);
		}
		//four is part of d
		if (four->dim.isContained(d) || four->dim.intersects(d)) {
			Dimension y = d.cut(four->dim);
			four->placeButton(y, func, name);
		}
	}
}

void QuadTree::printBT(const string& prefix, const QuadTree* node, bool isLeft)
{
	if (node != nullptr)
	{
		cout << prefix;

		cout << (isLeft ? u8"\u251C\u2500\u2500" : u8"\u2514\u2500\u2500");

		// print the value of the node
		cout << node->nameFader << "/" << node->nameButton << endl;

		// enter the next tree level - left and right branch
		printBT(prefix + (isLeft ? u8"\u2502   " : "    "), node->one, true);
		printBT(prefix + (isLeft ? u8"\u2502   " : "    "), node->two, true);
		printBT(prefix + (isLeft ? u8"\u2502   " : "    "), node->three, true);
		printBT(prefix + (isLeft ? u8"\u2502   " : "    "), node->four, false);
	}
}

void QuadTree::printBT()
{
	printBT("", this, false);
}

/*-------------------------------------------------------------------------------------------
  ------------------------- EXTERNAL FUNCTIONS TO CLASS -------------------------------------
  -------------------------------------------------------------------------------------------*/

QuadTree createTree() {
	QuadTree tree = QuadTree(Dimension(0, 0, 640, 480));

	tree.placeFader(Dimension(200, 0, 320, 60), volume, "gainA");
	tree.placeFader(Dimension(320, 0, 440, 60), volume, "gainB");
	tree.placeFader(Dimension(200, 60, 320, 120), volume, "highsA");
	tree.placeFader(Dimension(320, 60, 440, 120), volume, "highsB");
	tree.placeFader(Dimension(200, 120, 320, 180), volume, "midsA");
	tree.placeFader(Dimension(320, 120, 440, 180), volume, "midsB");
	tree.placeFader(Dimension(200, 180, 320, 240), volume, "lowsA");
	tree.placeFader(Dimension(320, 180, 440, 240), volume, "lowsB");

	tree.placeFader(Dimension(250, 240, 320, 420), volume, "volumeA");
	tree.placeFader(Dimension(320, 240, 390, 420), volume, "volumeA");

	return tree;
}