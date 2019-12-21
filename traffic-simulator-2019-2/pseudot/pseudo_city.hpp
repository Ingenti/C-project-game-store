//#pragma once
#include <deque>
#include <utility>
#include <vector>
#include <iostream>
#include <map>  
// cityllä oikeudet kaikkiin luokkiin
#include "road.hpp"
#include "driver.hpp"
#include "target.hpp"

class City {

public:
	// Määritellään City. 
	City(int time, int time_step, int activity);
	
	// Lisää uudet ajajat teille kutsumalla funktiota Add_to_queue kullekin tielle
	void Add_drivers(int amount_new_people);
    
    // Update_targets päivittää ajajien asiointiajat targeteissa ja siirtää ajajat tarvittaessa seuraavaan paikkaan
    void Update_drivers_in_targets();
    
    // City komentaa kunkin tien Drive() - funktiota
    void Command_to_drive();
    
    // Lisää ajaja vektoriin drivers_in_targets. Road tulee kutsumaan tätä funktiota.
    void add_drivers_in_targets(Driver& driver, int time);
    
private:
	// Tämän hetkinen kellonaika
	int time_;
	
	// For-loopin yksi aikahyppäys
	int time_step_;
	
	// Aktiivisuus, jolla ihmisiä lisätään
	int activity_;
	
	// Lukumäärä, kuinka monta ihmistä tulee luoda seuraavalla aika tikkauksella. Generoidaan siis joka aikavälille.
	int amount_new_people_;
	
	// Vektori kaikista kaupungin teistä
	std::vector<Road&> roads_;
	
	// Vektori kaikista kaupungin targeteista 
	std::vector<Target&> targets_;
	
	// Vektori drivers_in_targets asioivista drivereistä ja ajoista, joka heillä kuuluu vielä asiointiin. Drivereitä ei ole järjestetty ajan perusteella. 
	std::vector<std::pair <Driver&, int>> drivers_in_targets;
	
	// Key on koordinaatti ja value on vektori teistä, jotka ovat oikeassa järjestyksessä
	std::map< std::pair <int, int> coordinates, std::vector<Road&> > optimal_routes;
	    
};
