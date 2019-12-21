#include "car.hpp"


///////////////////////////////////////////////////
// Palauttaa ID:n seuraavaan solmuun. 
long Car::Next_node()
{
    if(route_.size() == 1) { return 0; }
    else
    {
        long next = route_.front();
        return next;
    }
}


///////////////////////////////////////////////////
// Palauttaa ensimmäisen noden, joka päivittyy jatkuvasti. 
long Car::First_node()
{
    long first = route_.front();
    return first;
}
