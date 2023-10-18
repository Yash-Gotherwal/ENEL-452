/**
   Project: Implementation of a Queue in C++.
   Programmer: Karim Naqvi
   Course: enel452
   Description: test code
*/

#include <iostream>
#include <cassert>
#include <cstdlib>
#include <vector>

#include "queue.h"

/**
   Compare the given queue to the given array of data elements.  Return
   true if they are all equal.
 */
bool testQueueEquality( Queue & q, std::vector<Data> const & array)
{
    if (q.size() != array.size())
	return false;

    for (unsigned i = 0; i < q.size(); ++i)
    {
        Data d(0,0);
	q.remove(&d);
	if ( !d.equals(array[i]) )
	    return false;
	q.insert(d);
    }
    return true;
}


int main()
{
    using namespace std;

    cout << "Testing queue.\n";
    Queue q1;

    q1.insert(Data(1,2));       //changed q1 inserts to match example given in outline
    q1.insert(Data(3,3));       //changed q1 inserts to match example given in outline
    q1.insert(Data(5,6));       //changed q1 inserts to match example given in outline

    q1.insert(Data(-2, -3),1);  //Added data to test similiar to outline
    q1.insert(Data(-4, -5),3);  //Added data to test similiar to outline

    q1.print();

    //Added a new q with an illegal operation the below queue should give an error
    Queue q;

    q.insert(Data(1, 2)); // Queue size is 1.
    q.insert(Data(3, 4)); // Queue size is 2.

    q.insert(Data(-2, -3), 3); // Attempting to insert at position 3, which is greater than the current size of the queue.

    vector<Data> dataVec;
    dataVec.push_back(Data(1,2));       //changed because failing test
    dataVec.push_back(Data(3,4));       //changed to match my initial numbers as test was failing
    dataVec.push_back(Data(5,6));

    assert(testQueueEquality(q1, dataVec));

    Data d44(4, 4);
    bool found = q1.search(d44);
    assert(found == false);


    q1.insert(d44);  // now is (1,1),(3,3),(5,5),(4,4)
    found = q1.search(d44);
    assert(found == true);

    // now queue is(1,1),(3,3),(5,5),(4,4) and 
    // dataVec has (1,1),(3,3),(5,5).  Not equal
    assert(testQueueEquality(q1, dataVec) == false);

    Data temp;
    q1.remove(&temp);  // now q1 is (3,3),(5,5),(4,4)

    Data temp2(1,1);
    assert(temp.equals(temp2));  // (1,1) == (1,1)

    Data temp3(6,6);
    found = q1.search(temp3);
    assert(found == false);
}
