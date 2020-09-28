#ifndef USEFUL_STUFF_H
#define USEFUL_STUFF_H

#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <sal.h>
#include <windows.h>
#include <math.h>
#include <limits>
#include <vector>

/**
    Typo:
        All function and class should start uppercase and be camelcase
        All members of class should start lowercase and be camelcase
        All constants and macro should be all uppercase
        All macros should be defined here. Try not to pollute the namespace.
    Rules:
        All class should have Initialize function in place of constructor
        All class should have Release function in place of destructor
        Unless it CANNOT fail or it has a return value, functions should return RESULT for error-checking.
        If a function succeed, return 0. Else, return whatever else (should be 1 if unspecified).
        Constants should be defined in relevant HEADERS.
*/

/** Function pointer quick-define*/
#define FUNCTION(returntype,name,arguement) returntype (*name) (arguement)

/** Call Release of target object. No error checking.*/
#define DESTROY(p_COM_object) if (p_COM_object) {p_COM_object->Release();p_COM_object = NULL;}

/** Result of command*/
#define RESULT LONG

/** Allocate the variable. If out of memory, return 1 */
#define BLOCKALLOC(type, pname) if (!(pname = new type)) { OutputDebugString(L"out of memory\n");return 1;}

/** Call the COM command. If it fails, return 1*/
#define COMCALL(COM_command) if (FAILED(COM_command)) return 1

/** Call the command. If it fails, return 1*/
#define BLOCKCALL(command, errorString) if (command) {OutputDebugString(L##errorString); return 1;}

/** Mark as error checking. Will be defined as nothing in final code*/
#define DEBUG(stuff) stuff

struct DXGI_RATIONAL;
typedef DXGI_RATIONAL RefreshRate;

using std::cerr;

class NonCopyable
{
    public:
        NonCopyable();
        virtual ~NonCopyable();

    protected:

    private:
        NonCopyable(NonCopyable&);
        NonCopyable& operator = (const NonCopyable&);
};

class Invokable
{
public:
	Invokable() {};
	virtual ~Invokable() {};
	virtual void Invoke(void* param) {};
};

template <typename T>
struct Coordinate
{
    T x;
    T y;
	T length() {
		return sqrt(x*x + y * y);
	}
	T operator*(const Coordinate& p) const {
		return x * p.x + y * p.y;
	}
	Coordinate operator*(T k) const {
		return Coordinate(x*k, y*k);
	}
	Coordinate operator-(const Coordinate& p) const {
		return Coordinate(x - p.x, y - p.y);
	}
	Coordinate operator+(const Coordinate& p) const {
		return Coordinate(x + p.x, y + p.y);
	}
	Coordinate& operator+=(const Coordinate& p) {
		this->x += p.x;
		this->y += p.y;
		return *this;
	}
	Coordinate& operator*=(const Coordinate& p) {
		this->x *= p.x;
		this->y *= p.y;
		return *this;
	}
	bool operator==(const Coordinate& right) {
		return this->x == right.x && this->y == right.y;
	}
    Coordinate(T a=T(), T b=T()): x(a), y(b){}
	Coordinate(const Coordinate& p):x(p.x), y(p.y) {}
};

typedef Coordinate<float> Point;
typedef Coordinate<int> Coord;

// Given three colinear points p, q, r, the function checks if 
// point q lies on line segment 'pr' 
// OR it should, but it returns whether p is in the rectangle made by q & r instead.
//bool onSegment(Point p, Point q, Point r);

// To find orientation of ordered triplet (p, q, r). 
// The function returns following values 
// 0 --> p, q and r are colinear 
// 1 --> Clockwise 
// 2 --> Counterclockwise 
//int orientation(Point p, Point q, Point r);

// The function that returns true if line segment 'p1q1' 
// and 'p2q2' intersect. 
//bool doIntersect(Point p1, Point q1, Point p2, Point q2);

// Returns true if the point p lies inside the polygon[] with n vertices 
//bool isInside(std::vector<Point>& polygon, Point p);

// TODO: move this. Thanh moved already so only need copy-paste.
/*/
class DragAndDrop
{
public:
    void UpdateDragAndDrop(Point& pos,std::vector<Point> hitBox, class Input& input, Point camPos);

private:

private:
    bool beingHold = false;
    bool clickable = false;
    Point prevPos;

};
//*/
#define CODE_KILL 2
#define CODE_ERROR 1

#define CODE_PRIORITIZE 4

#endif // USEFUL_STUFF_H
