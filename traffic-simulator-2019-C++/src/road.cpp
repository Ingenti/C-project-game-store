#include <map>
#include "road.hpp"
#include <QDebug>
using namespace std;


///////////////////////////////////////////////////
// Konstruktori, laskee tien pituuden alku- ja päätesolmujen avulla, mikä määrää kuinka kauan tienpätkä kestää ajaa.
Road::Road(Node* source, Node* destination, int side, unsigned int max_traffic)
{
    unique_cars_ = 0;
    // Määritellään tien lähtö-, ja määränpääsolmu sekä tien pituus
    source_ = source;
    destination_ = destination;
    length_ = 1000000 * std::sqrt(    std::pow( destination_->Location().first - source_->Location().first , 2 ) + 
                            std::pow( destination_->Location().second - source_->Location().second, 2) );
    
    // Max_traffic on suhteutettu kartan kokoon
    max_traffic_ = static_cast<unsigned int>(length_ / 80);
    if (max_traffic_ < 3) {max_traffic_ = 3;}

    // Lähtötiet eivät ole varsinaisia kartassa näkyviä teitä, joten niille pitää tehdä oma max_traffic_ määrittely
    if(length_ < 1) { max_traffic_ = 1000; }
    side_ = side;
    current_traffic_ = {};
}



///////////////////////////////////////////////////
// Osuus, kuinka täynnä tie on. (1 on tummanpunainen, 0 on vihreä)
float Road::Use_percent()
{
    float percent; float size = current_traffic_.size(); float max = max_traffic_;
    if(max_traffic_ != 0) { percent = size / max;  return percent;} else { return 0; }  }



///////////////////////////////////////////////////
// Lisää auton tielle, jos tiellä on tilaa.
int Road::Arrive_on_road(Car* car)
{
    unique_cars_ += 1;
    if(current_traffic_.size() < max_traffic_)
    {
        car->Pop();
        // Tien matka-aika
        current_traffic_[car] = length_;
        return 1; 
    }
    else
    {
        return 0;
    }
}



///////////////////////////////////////////////////
// Vähentää tiellä olevien autojen matka-aikaa, tai yrittää siirtää ne seuraavalle tielle. Parametrit siksi, koska nimiavaruuksia ei funtsittu ihan loppuun asti.
void Road::Drive(std::map<Node*, std::list<Road*> > adjacencylist, std::map<long, Node*> nodes, int time)
{
    histogram_data_.push_back(unique_cars_);
    // Tarkistetaan varmuuden vuoksi, onko tiellä yhtään autoja.
    if(current_traffic_.size())
    {
        // Iteroidaan tiellä olevien autojen läpi.
        for(auto cars = current_traffic_.begin(); cars != current_traffic_.end(); cars++)
        {
            // Matka-aikaa on vielä jäljellä, joten ajetaan eteenpäin.
            if(cars->second > 0)
            {
                // YHDEN LOOPIN AIKANA AJETTU MATKA, VOI MUOKATA
                cars->second -= 300;
            }
            // Auto on ajanut loppuun, ja haluaa siirtyä seuraavalle tielle.
            else
            {
                // Haetaan autolta seuraavan solmun ID
                long next = cars->first->Next_node();
                // Jos seuraavaa solmua ei ole, auto on päässyt maaliin ja se poistetaan ilman jatkotoimenpiteitä. 
                if( next == 0 ) { current_traffic_.erase(cars); continue; }
                // Haetaan osoitin solmuun ID:n avulla 
                Node* next_node = nodes[next];
                // Haetaan adjacencylististä lista teistä, jotka lähtevät tämän tien määränpäästä
                auto roads = adjacencylist[this->Destination()];
                Road* next_road;
                // Etsitään listasta se tie, jonka määränpää on auton seuraava solmu.
                for(auto i : roads) { if(i->Destination() == next_node) { next_road = i; } }
                // Yritetään lisätä auto seuraavalle tielle. Jos onnistuu, niin lyhennetään auton reittiä ja poistetaan auto tältä tieltä
                if(next_road->Arrive_on_road(cars->first) ) { current_traffic_.erase(cars); }
            }
        }
    }
}


