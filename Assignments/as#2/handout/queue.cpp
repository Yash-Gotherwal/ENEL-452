/**
   Project: Implementation of a Queue in C++.
   Programmer: Karim Naqvi
   Course: enel452

   //edit: Yash Gotherwal
   200420786
   Added a queue insert function to modify the code.
*/

#include "queue.h"
#include <iostream>
#include <cstdlib>              // for exit

using namespace std;

Queue::Queue()
{
    head = 0;
    tail = 0;
    nelements = 0;
    verbose = false;
}

Queue::~Queue()
{
    for (QElement* qe = head; qe != 0;)
    {
	QElement* temp = qe;
	qe = qe->next;
	delete(temp);
    }
}

void Queue::remove(Data* d)
{
    if (size() > 0)
    {
        QElement* qe = head;
        head = head->next;
        nelements--;
        *d = qe->data;
	delete qe;
    }
    else
    {
        cerr << "Error: Queue is empty.\n";
        exit(1);
    }
}

void Queue::insert(Data d)
{
    if (verbose) std::cout << "insert(d)\n";
    QElement* el = new QElement(d);
    if (size() > 0)
    {
        tail->next = el;
    }
    else
    {
        head = el;
    }
    tail = el;
    nelements++;
}
//*********************************************************************************
void Queue::insert(Data d, unsigned position)
{
    if (position > size())          //conditional check to provide error if you try to input data in a position greater than queue size
    {
        cerr << "insert: range error." << endl;
        exit(3);
    }

    QElement* el = new QElement(d);     //if error check passes create new element
    if (position == 0)                  //insert element at head if pos 0
    {
        el->next = head;
        head = el;
    }
    else                                //if not zero create new element to insert at specific position
    {
        QElement* prev = head;
        for (unsigned i = 1; i < position; i++)
        {
            prev = prev->next;
        }
        el->next = prev->next;
        prev->next = el;
    }
//************************************************************************************************

    if (position == size())
    {
        // If the element is inserted at the tail, update the tail pointer.
        tail = el;
    }

    nelements++;
}


bool Queue::search(Data otherData) const
{
    QElement* insideEl = head;
    for (int i = 0; i < nelements; i++)
    {
        if (insideEl->data.equals(otherData))
            return true;
        insideEl = insideEl->next;
    }
    return false;
}

void Queue::print() const
{
    QElement* qe = head;
    if (size() > 0)
    {
        for (unsigned i = 0; i < size(); i++)
        {
            cout << i << ":(" << qe->data.x << "," << qe->data.y << ") ";
            qe = qe->next;
        }
    }
    cout << "\n";
}

unsigned Queue::size() const
{
    return nelements;
}
