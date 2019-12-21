//#pragma once
#include <deque>
#include <utility>
#include <iostream>
#include "driver.hpp"
#include "target.hpp"


class Road {

public:
	// Määritellään Road, johon City generoi vaadittavat parametrit. 
	Road(std::pair <int, int> coordinates, int max_drivers, int drive_time);
	
	// Road lähettää tien loppuun ajaneet autot seuraavalle tielle/targetille. Tämän jälkeen Road lisää mahdollisimman monta ajajaa queue_to_road:sta jonoon queue_travellers.
	void Drive();
	
	// Muut oliot kutsuvat tietä tällä funktiolla ja lisäävät näin ajajan jonoon queue_to_road
	void Add_to_queue(Driver&);
	
    // Ehkä funktio, joka palauttaa tien koordinaatit?
    
private: 
	// coordinates kertoo tien sijainnin
    std::pair <int, int> coordinates_;
    
    // max_drivers kertoo, kuinka monta ajajaa tielle enintään mahtuu
    int max_drivers_;
    
    // drive_time kertoo tien ajamiseen vaadittavan ajan. Myöhemmin tämä tulee muuttumaan tien ruuhkaisuuden funktiona.
    int drive_time_;
    
    // amount_travellers kuvaa tiellä matkustavia. HUOM: amount_travellers ei ole välttämättä yhtä suuri kuin queue_travellers.size().
    int amount_travellers_;
    
    //HUOM: Kuvastaako seuraavat jonorakenteet riittävästi teiden kaksisuuntaisuutta?
    // queue_travellers kuvastaa tiellä matkustavia ajajia, jotka eivät ole vielä matkustaneet tien loppuun.
    std::deque< Driver& > queue_travellers_;
    
    // queue_to_road kuvastaa tielle haluavia ajajia, jotka ovat yhä edellisellä tiellä. 
    // Jos (amount_travellers < max_drivers), tielle haluava ajaja voidaan ottaa jonoon queue_travellers.
    std::deque< Driver& > queue_to_road_;
    
};
