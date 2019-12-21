#include <iostream>
#include "car.hpp"
#include "node.hpp"
#include <cmath>
#include <vector>
// Luokka tielle. Rakentuu lähtösolmusta ja määränpäästä. Sisältää myös tiedon kaikista autoista tiellä, ja kauan niillä menee matkustaa.

class Road {
    public:
	// Luo tien laskemalla tien pituuden alku- ja päätesolmujen avulla, mikä määrää kuinka kauan tienpätkä kestää ajaa.
        Road(Node* source, Node* destination, int side, unsigned int max_traffic_=10);
	
	// Lisää auton tielle, jos tiellä on tilaa.
        int Arrive_on_road(Car* car);

	// Vähentää tiellä olevien autojen matka-aikaa, tai yrittää siirtää ne seuraavalle tielle. 
        void Drive(std::map<Node*, std::list<Road*> > adjacencylist, std::map<long,Node*> nodes, int time);

	// Palauttaa tien määränpääsolmun
	Node* Destination() { return destination_; }

	// Palauttaa tien lähtösolmun
        Node* Source() { return source_; }

	// Palauttaa tien pituuden ja siden
        int ln() { return length_; } 
        int Side() { return side_; }

	// Osuus, kuinka täynnä tie on. (1 on tummanpunainen, 0 on vihreä)
        float Use_percent();

	// Palauttaa kirjaston, jossa autot ja matka-ajat 
        std::map<Car*, int> Traffic() { return current_traffic_; }

	// Palauttaa tiellä matkustavien määrän
    	int Total_drivers() {return current_traffic_.size();} 
    
        std::vector<int> Data() {return histogram_data_;}
    private:
        // Kirjasto tiellä olevista autoista, avain on osoitin autoon ja arvo jäljellä oleva matka-aika
        std::map<Car*,int> current_traffic_;

	// Tien lähtösolmu
        Node* source_;

	// Tien määränpääsolmu
        Node* destination_;
	
	// Tien pituus
        int length_;

	// Tien maksimikapasiteetti
        unsigned int max_traffic_;

    // Piirtämistä varten parametri. Kuvaa tien puolta
        int side_;

    // Autojen määrä päivän aikana histogrammia varten
        std::vector<int> histogram_data_;
    // Apumuuttuja histogrammia varten
        int unique_cars_;
};




