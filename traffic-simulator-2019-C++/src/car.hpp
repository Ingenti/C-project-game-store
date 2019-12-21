// Kuvaa autoa, joka liikkuu kaupungissa. Sisältää reitin, joka on lista solmujen ID:tä

#include <list>
class Car {
    public:
	// Luo auton, sen reitin avulla 
        Car(std::list<long> route) : route_(route) {}

        // Palauttaa seuraavan solmun reitillä
        long Next_node();
	
	// Palauttaa ensimmäisen noden, joka päivittyy jatkuvasti. 
        long First_node();

        // Poistaa solmun reitiltä, kun auto on ajanut sen ohi.
        void Pop() { route_.pop_front(); }
    
    private:
	// Auton koko reitti alkutilasta määränpäähän
        std::list<long> route_;
};
