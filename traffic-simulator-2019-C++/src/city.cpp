#include "../lib/rapidxml-1.13/rapidxml_utils.hpp"
#include "../lib/rapidxml-1.13/rapidxml.hpp"
#include <algorithm>
#include <queue>
#include "city.hpp"
#include <math.h> 
#include <limits>
#include <random>
#include <chrono>
using namespace std;


///////////////////////////////////////////////////
// Yksi looppaus, joka on ajanhetkenä riittävän pieni
int City::Loop(int time) {

    int are_paths = home_office_.size() + office_home_.size() + gym_home_.size() + home_gym_.size() + office_gym_.size() + gym_office_.size();
    if(!are_paths) { return -1; }

    //Keskeiset funktiot Loopin sisällä
    are_paths = Create_cars(time);
        if(!are_paths) { return -1; } 
    Update_roads(time);   
    // Palauttaa kaikkien tiellä matkustavien määrän
    int total_drivers = 0;
    for(auto road : roads_){
	total_drivers += road->Total_drivers();
    }	

    return total_drivers;
}


///////////////////////////////////////////////////
// Luodaan satunnainen määrä ajajia karttaan	
int City::Create_cars(int time)
{   
    // Satunnaisuuden avulla luodaan ihmisten käyttäytyminen (esim. aamulla ihmiset menevät todennäköisemmin töihin kuin illalla)
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator (seed);
 
    // Kuvastaa monesko tunti menossa pyöristettynä alaspäin (esim 17, kello voisi olla 17:34) ottaen huomioon, että aikayksikkö on 10s
    int current_hour = 	floor (time / (6*60));

    // Lambda kuvastaa ihmisten aktiivisuutta (esim. aamulla ihmiset liikkuvat todennäköisemmin kuin yöllä)
    double current_lambda = lambdas[floor(current_hour)];
    
    // Ajajien luonti on Poisson-jakautunut
    std::poisson_distribution<int> distribution (current_lambda);
    
    // Luotavien ihmisten määrä: i
    int i = distribution(generator);

    // For-loop, jonka aikana luodaan ihmiset
    for(int amount = 0; amount < i; amount++)
    {
	// Valitse satunnaisesti, minkälainen polku on kyseessä. Tärkeää on ohjelman toimimisen kannalta tarkistaa, että polkuvektori ei ole tyhjä.
	int path_type = -1;
	// Normitus auttaa käsittelemään jakaumaa oikein.	
	double norm = 1;
	int counter = 0;
    // :D
    int purkka = 0;	
	// not_found_path_vector muuttuu todeksi kun on löydetty path-vektori, jossa on alkioita
	bool not_found_path_vector = true;
	while (not_found_path_vector) {
        purkka++;
        if(purkka > 300) { return 0; }
		// While-loop loppuu, kun kaikki paitsi viimeinen alkio on tarkastettu
		while ((path_type < 0 ) || (counter < 5)) {
			std::bernoulli_distribution distribution(propotions_routes[current_hour][counter]/norm);
			if (distribution(generator)) {path_type = counter; }
			norm = norm - propotions_routes[current_hour][counter];
			counter = counter + 1; 
		}
		
		if (path_type == -1) {path_type = 5;}
		// Jos ei ole löydetty sopivaa arvoa, while-loop jatkuu
		if (Check_vector_size(path_type)) {not_found_path_vector = false;}
		else {path_type = -1; counter = 0; norm = 1;}
		
	}	

	// Haetaan satunnaisuudella auton polku         
	auto path = Get_path(path_type);

	// Luodaan auto/matkustaja
        Car* car = new Car(path);

	// Haetaan solmu, josta auto aloittaa reitin
        Node* starting_node = nodes_[car->First_node()];

	// Autoja ei lisätä nodeille vaan roadeille. Etsitään siis seuraava tie, johon auto lisätään.
        auto road = find_if(roads_.begin(), roads_.end(), [&](Road* r) 
        {
            return ( r->Source() == starting_node );
        });
	
	// Syöttää auton löydetylle tielle
        (*(road))->Arrive_on_road(car);

        // Poistaa ensimmäisen solmun reitiltä, sillä auto on ajanut siitä ohi
	car->Pop();
    }
    return 1;
}


///////////////////////////////////////////////////
// Kutsuu jokaista tietä, ja jokainen tie pyrkii puskemaan autoja eteenpäin 
void City::Update_roads(int time)
{
    for(auto road : roads_)
    {
        road->Drive(adjacencylist_, nodes_, time);
    }
}



///////////////////////////////////////////////////
// Tarkistaa oikean tunnuksen avulla, että path-vektori ei ole tyhjä. True, jos path-vektori ei ole tyhjä.
bool City::Check_vector_size(int label){
	bool not_empty = false;

	switch(label) {
	      case '0' : 
		     if (home_office_.size()> 0) {not_empty = true;};
		 break;
	      case '1' :
   		     if (office_home_.size()> 0) {not_empty = true;};
		 break;
	      case '2' :
   		     if (home_gym_.size()> 0) {not_empty = true;};
		 break;
	      case '3' :
   		    if (gym_home_.size()> 0) {not_empty = true;};
		 break;
	      case '4' :
   		     if (office_gym_.size()> 0) {not_empty = true;};
		 break;
	      default :
   		    if (gym_office_.size()> 0) {not_empty = true;};
	
	}
    
	return not_empty;
}


///////////////////////////////////////////////////
// Arvotaan satunnainen polku tasaisesti oikean tunnuksen perusteella
list<long> City::Get_path(int path_type)
{	
	// Alustetaan arvot
	int i = 0;
	auto l = gym_office_[0];

	switch(path_type) {
	      case '0' :
		     i = rand() % home_office_.size();
   		     l = home_office_[i];
		 break;
	      case '1' :
		     i = rand() % office_home_.size();
   		     l = office_home_[i];
		 break;
	      case '2' :
		     i = rand() % home_gym_.size();
   		     l = home_gym_[i];
		 break;
	      case '3' :
		     i = rand() % gym_home_.size();
   		     l = gym_home_[i];
		 break;
	      case '4' :
		     i = rand() % office_gym_.size();
   		     l = office_gym_[i];
		 break;
	      default :
		     i = rand() % gym_office_.size();
   		     l = gym_office_[i];
	}

	return l;
}


///////////////////////////////////////////////////
// Lataa kartan ja luo tarvittavat rakenteet buildings jne.
void City::Load_map(const char* filename) 
{   
    // Avataan ja parsitaan tiedosto
    rapidxml::file<> xmlFile(filename);
    rapidxml::xml_document<> map;
    map.parse<0>(xmlFile.data());
    // Ensimmäinen alkio on karttatiedoston info, ei tehdä sillä mitään
    rapidxml::xml_node<> *node = map.first_node();
    // Toinen alkio (bounds) määrittää kartan sijainnin korkeus- ja leveysasteilla. Luetaan ne.
    node = node->first_node();
    // Arvot sijaitsevat bounds-solmun attribuuteissa. Kaikki xml-data on charactereina, joten pitää käyttää string to floattia.
    rapidxml::xml_attribute <> *attr = node->first_attribute();
    float minlat = stof(attr->value());
    dimensions_.push_back(minlat);
    attr = attr->next_attribute();
    float minlon = stof(attr->value());
    dimensions_.push_back(minlon); 
    attr = attr->next_attribute();
    float maxlat = stof(attr->value());
    dimensions_.push_back(maxlat); 
    attr = attr->next_attribute();
    float maxlon = stof(attr->value());
    dimensions_.push_back(maxlon);

    // Lähdetään iteroimaan tiedosta läpi. Luetaan ensin solmukohtia merkitsevät alkiot nodes_iin.
    if( node->next_sibling() != nullptr ) { node = node->next_sibling(); }
    while((node->next_sibling() != nullptr) && ("node" == string(node->name() ) ) )
    {
        long id = 1; float lat = 1; float lon = 1;
        for(rapidxml::xml_attribute<> *attr = node->first_attribute(); attr; attr = attr->next_attribute())
        {
            if( string(attr->name() ) == "id" ) { id = stoll( attr->value() ); }
            if( string(attr->name() ) == "lat" ) { lat = stof( attr->value() ); }
            if( string(attr->name() ) == "lon" ) { lon = stof( attr->value() ); }
        }
        Node* new_node = new Node(id, lat, lon);
        nodes_[id] = new_node;
        node = node->next_sibling();
    }
    
    // Sitten iteroidaan talot ja tiet.
    while((node != nullptr) && ("way" == string(node->name() ) ) )
    {
        rapidxml::xml_attribute<> *attr = node->first_attribute();
        long id = stoll( attr->value() );
        int buildingFlag = 0;
        int highwayFlag = 0;
        list<long> nd_list;
        for(rapidxml::xml_node<> *waychild = node->first_node(); waychild; waychild = waychild->next_sibling() )
        {
            string node_name = string( waychild->name() );
            if("nd" == node_name ) 
                {
                    rapidxml::xml_attribute<> *attr = waychild->first_attribute();
                    nd_list.push_back( stoll ( attr->value() ) );
                }
            if("tag" == node_name ) 
                {
                    rapidxml::xml_attribute<> *attr = waychild->first_attribute();
                    if( string( attr->value() ) == "building") { buildingFlag = 1; }
                    if( string( attr->value() ) == "highway") { highwayFlag = 1; }
                }
        }
        if(buildingFlag) 
        {
            // Arvotaan, mitä tyyppiä rakennukset ovat ( 0 == koti, 1 == toimisto, 2 = vapaa-ajan rakennus )
	    
	    // Valitse satunnaisesti, minkälainen polku on kyseessä building_proportionsienin mukaisesti
	     unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    	     std::default_random_engine generator (seed);
		
	     int path_type = -1;
	     // Normitus auttaa käsittelemään jakaumaa oikein.
	     double norm = 1;
	     int counter = 0;

		while ((path_type < 0 ) || (counter < 2)) {
			std::bernoulli_distribution distribution(building_proportions[counter] / norm);
			if (distribution(generator)) {path_type = counter; }
			norm = norm - building_proportions[counter];
			counter = counter + 1; 
		}
	
		if (path_type == -1) {path_type = 2;}

            Building* building = new Building(id, nd_list, path_type);
            buildings_[id] = building;
        }
        if(highwayFlag)
        {
            long nd_1 = nd_list.front();
            long nd_2;
            nd_list.pop_front();
            while(!nd_list.empty() )
            {
                nd_2 = nd_list.front();
                nd_list.pop_front();
                Road* new_road = new Road( nodes_[nd_1], nodes_[nd_2], 0 );
                roads_.insert(new_road);
                adjacencylist_[ nodes_[nd_1] ].push_back( new_road );
                Road* new_opposite_road = new Road( nodes_[nd_2], nodes_[nd_1], 1 );
                roads_.insert(new_opposite_road);
                adjacencylist_[ nodes_[nd_2] ].push_back( new_opposite_road );
                nd_1 = nd_2;
                visible_roads_.push_back(new_opposite_road);
                visible_roads_.push_back(new_road);
            }
        }
        node = node->next_sibling();
    }
    map.clear();
	
    // Liitetään talot osaksi verkkoa
    set<Node*> added_building_nodes;
    for(auto building : buildings_)
    {
        float min_dist = 420;
        Node* closest_road_node;
        Node* closest_building_node;
        for(auto corner : building.second->Corners() )
        {
            auto corner_location = nodes_[corner]->Location();
            for(auto road_node : adjacencylist_)
            {
                if( added_building_nodes.find(road_node.first) == added_building_nodes.end() ) 
                {
                    auto node_location = road_node.first->Location();
                    float dist = std::sqrt(std::pow( corner_location.first - node_location.first , 2 ) + std::pow( corner_location.second - node_location.second, 2) );
                    if( dist < min_dist ) 
                        {
                            min_dist = dist;
                            closest_road_node = road_node.first;
                            closest_building_node = nodes_[corner];
                        }
                }
            }
        }
        long randomid = rand() % 1000000;
        Node* invisible_node = new Node(randomid, closest_road_node->Location().first + 0.0000001, closest_road_node->Location().second + 0.0000001);
        nodes_[randomid] = invisible_node;
        Road* new_road = new Road( closest_road_node, invisible_node, 0 );
        roads_.insert(new_road);
        adjacencylist_[ closest_road_node ].push_back( new_road );
        Road* new_opposite_road = new Road( invisible_node, closest_road_node, 1 );
        roads_.insert(new_opposite_road);
        adjacencylist_[ invisible_node ].push_back( new_opposite_road );
        added_building_nodes.insert(closest_building_node);

        building.second->AddNode(randomid);
        building_nodes_[randomid] = invisible_node;
    }

    Dijkstra(); 
}



///////////////////////////////////////////////////
// Optimoi nopeimpia reittejä
void City::BFS()
{
    int nodecount = adjacencylist_.size();
    auto index_to_value = [&](int i) { int index = 0; for(auto iter : adjacencylist_) { if( index == i ) { return iter; } else { index++; } } };
    auto is_building_node = [&] (Node* c) { int index = 0; for(auto iter : building_nodes_) 
    {
        if(iter.second == c)   {return index;}
        else { index++;}
    } return 0; };
    auto get_building_type = [&] (Node* c)
    {
        for(auto iter : buildings_)
        {
            if(nodes_[iter.second->CornerNode()] == c) { return iter.second->Type(); }
        }
    };
    for(int i = 0; i < nodecount; i++)
    {
        auto p = index_to_value(i);
        int is_building = is_building_node(p.first);
        if(is_building)
        {
            int source_type = get_building_type(p.first);
            vector<int> visited(nodecount, 0);
            vector<int> prev(nodecount, -1);
            deque<int> queue;
            queue.push_front(i);
            visited[i] = 1;
            while(queue.size() > 0)
            {
                int v = queue.front(); queue.pop_front(); 
                if((v != i) && (is_building_node(index_to_value(v).first)))
                {
                    int dest_type = get_building_type(index_to_value(v).first);
                    list<long> path;
                    int path_iter = v;
                    while(path_iter != -1)
                    {
                        path.push_back( index_to_value(path_iter).first->ID() );
                        path_iter = prev[path_iter];
                    }
                     
                    if(path.size() > 3) {
                        if((source_type == 0) && (dest_type == 1)) { home_office_.push_back(path);}
                        if((source_type == 1) && (dest_type == 0)) { office_home_.push_back(path);}
                        if((source_type == 0) && (dest_type == 2)) { home_gym_.push_back(path);}
                        if((source_type == 2) && (dest_type == 0)) { gym_home_.push_back(path);}
                        if((source_type == 1) && (dest_type == 2)) { office_gym_.push_back(path);}
                        if((source_type == 2) && (dest_type == 1)) { gym_office_.push_back(path);}
                    }
                }
                auto roads = [&]() { auto iter = adjacencylist_.begin(); for(int v_temp = 0; v_temp < v; v_temp++) { iter++; } return iter->second; };
                list<Road*> edges = roads();
                vector<int> neighbors;
                auto getneighbor = [&](Road* edge) 
                {
                    int index = 0;
                    for(auto i : adjacencylist_) { if(edge->Destination() == i.first) { return index; } else { index++; } }
                };
                for(auto i : edges) { neighbors.push_back( getneighbor( i ) ); }
                for(unsigned int k = 0; k < neighbors.size(); k++)
                {
                    int w = neighbors[k];
                    if(!visited[w]) { visited[w] = 1; queue.push_front(w); prev[w] = v;} 
                }
            }
            
        }
    }
}


///////////////////////////////////////////////////
//Tuhotaan city
City::~City(){
    // Tuhotaan highlightattu tiepointteri
    delete road_;
}




void City::Dijkstra()
{
    int nodecount = adjacencylist_.size();
    auto index_to_value = [&](int i) { int index = 0; for(auto iter : adjacencylist_) { if( index == i ) { return iter; } else { index++; } } };
    auto is_building_node = [&] (Node* c) { int index = 0; for(auto iter : building_nodes_) 
    {
        if(iter.second == c)   {return index;}
        else { index++;}
    } return 0; };
    auto get_building_type = [&] (Node* c)
    {
        for(auto iter : buildings_)
        {
            if(nodes_[iter.second->CornerNode()] == c) { return iter.second->Type(); }
        }
    };

    for(int all = 0; all < nodecount; all++)
    {
        auto p = index_to_value(all);
        int is_building = is_building_node(p.first);
        if(is_building)
        {
            int source_type = get_building_type(p.first);
            // Vektori etäisyyksistä lähtösolmuun, alustetaan "infinityksi"
            vector<int> distances(adjacencylist_.size(), numeric_limits<int>::max());
            // Apuvektori, jolla katsotaan onko solmussa käyty ja mikä on edellinen solmu
            vector<int> visited(adjacencylist_.size(), 0);
            // Lista edellisistä solmuista
            vector<int> prev(adjacencylist_.size(), -1);
            distances[all] = 0;
            for(int n = 0; n < nodecount; n++)
            {
                auto minV = [&]()
                {
                    int v = 0;
                    for(int j = 0; j < nodecount; j++){ if( !visited[j] ) { v = j; break; } }
                    for(int j = 0; j < nodecount; j++){ if( !visited[j] && (distances[j] < distances[v]) ) { v = j; } }
                    return v;
                };    
                int v = minV();
                visited[v] = 1;
                if( distances[v] == numeric_limits<int>::max() ) { break; }
                auto roads = [&]() { auto iter = adjacencylist_.begin(); for(int v_temp = 0; v_temp < v; v_temp++) { iter++; } return iter->second; };
                list<Road*> edges = roads();
                vector<pair<int,Road*> > neighbors;
                auto getneighbor = [&](Road* edge) 
                {
                    int index = 0;
                    for(auto i : adjacencylist_) { if(edge->Destination() == i.first) { return make_pair(index,edge); } else { index++; } }
                };
                for(auto i : edges) { neighbors.push_back( getneighbor( i ) ); }
                for(unsigned int k = 0; k < neighbors.size(); k++)
                {
                    int w = neighbors[k].first;
                    if( distances[w] > (distances[v] + neighbors[k].second->ln() ) ) { distances[w] = distances[v] + neighbors[k].second->ln(); prev[w] = v;} 
                } 
                 
            }
            for(int v = 0; v < nodecount; v++)
            {
                if((v != all) && (is_building_node(index_to_value(v).first)))
                {
                    int dest_type = get_building_type(index_to_value(v).first);
                    list<long> path;
                    int path_iter = v;
                    while(path_iter != -1)
                    {
                        path.push_back( index_to_value(path_iter).first->ID() );
                        path_iter = prev[path_iter];
                    }
                     
                    if(path.size() > 7) { 
                        if((source_type == 0) && (dest_type == 1)) { home_office_.push_back(path);}
                        if((source_type == 1) && (dest_type == 0)) { office_home_.push_back(path);}
                        if((source_type == 0) && (dest_type == 2)) { home_gym_.push_back(path);}
                        if((source_type == 2) && (dest_type == 0)) { gym_home_.push_back(path);}
                        if((source_type == 1) && (dest_type == 2)) { office_gym_.push_back(path);}
                        if((source_type == 2) && (dest_type == 1)) { gym_office_.push_back(path);}
                    }
                }
            }
        }
    }
}
