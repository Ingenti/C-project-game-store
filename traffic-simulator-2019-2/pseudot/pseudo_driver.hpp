#pragma once

#include <map>
#include <vector>
#include "target.hpp"
//#include "road.hpp"

class Driver {
	public:
		// Toistaiseksi Driverin luonti tapahtuu ilman Roadin määrittelyä
		//Driver(Target starting_node, std::vector< std::pair<Target, int> > targets, std::vector<Road> path, Road previous_road);
		Driver(Target starting_node, std::vector< std::pair<Target, int> > targets);	


		// Päivittää Driver-olion kun se pääsee määränpäähän		
		void Update(); 
	private:
		// Aloitussolmu
		Target starting_node_; 
		
		// Lista määränpäistä, ja kuinka pitkään driver viettää kusskin
		std::vector< std::pair<Target, int> > targets_; 
		
		//Lyhin reitti seuraavaan määränpäähän		
		//std::vector<Road> path_; 

		// Edellinen Road-olio, vastaluodulla Driverilla Null
		//Road previous_road_; 
};
