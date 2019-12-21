#include <list>
#include <map>
#include <string>
#include <vector>
#include "building.hpp"
#include "road.hpp"
#include <set>
#include <chrono>

class City {
    public:
	// Luo Cityn
        City() {}

	// Yksi looppaus, joka on ajanhetkenä riittävän pieni
    	int Loop(int time);

	// Luodaan satunnainen määrä ajajia karttaan	
	int Create_cars(int time);

	// Kutsuu jokaista tietä, ja jokainen tie pyrkii puskemaan autoja eteenpäin 
        void Update_roads(int time);
	
	// Arvotaan satunnainen polku, joka osaa etsiä oikean path-tyypin
	std::list<long> Get_path(int path_type);

	//Näiden avulla luodaan vektori paths_, joka kuvastaa mahdollisimman hyvää reittivalintaa 
        void Dijkstra();
        void BFS();

	// Lataa kartan ja luo tarvittavat rakenteet buildings jne.
        void Load_map(const char* filename);

   	// Kertoo kaikkien matkustajien määrän ulkopuolisille
        int GetTravellers(){return all_travellers;}

	// Tutki vektorin koko vastaavalla tunnukselle. True vektorissa on alkioita
	bool Check_vector_size(int label);

	// Tuhoaa kaupungin
        ~City();
        
	//Annetaan muille luokille mahdollisuus päästä cityn tietorakenteisiin
        std::map<long, Building*> Buildings() { return buildings_; }
        std::set<Road*> Roads() { return roads_; }
        std::map<long,Node*> Nodes() { return nodes_; }
        std::map<Node*, std::list<Road*> > AdjList() { return adjacencylist_; }
        std::vector<float> Dim() { return dimensions_; }
        std::vector<Road*> Visible() { return visible_roads_; }
        Road *road_ = nullptr;

    private:
	//Hakemalla node-avaimen avulla saadaan vastaavan noden roadit 
        std::map<Node*, std::list<Road*> > adjacencylist_;

	//Map kaikista rakennuksista
        std::map<long, Building*> buildings_;
        
	// Nodet, jossa avaimena on noden id
	std::map<long, Node*> nodes_;
	
	// Tiet, jota city alkaa iteroimaan
        std::set<Road*> roads_;

	// Neljä koordinaattia (min/max leveys ja korkeus)
        std::vector<float> dimensions_;

	// Pathit koostuvat taloista ja päätyteistä, listattuna kaikki mahdolliset tiet
        std::vector<std::list<long> > paths_;

	// Kaikilla teillä olevien matkustajien määrä
	int all_travellers = 0;

	//Seuraavat 6 vektoria kuvastaa suunnattuja polkuja eri rakennusten välillä!
        // Tunnus 0
        std::vector<std::list<long> > home_office_;
        
	// Tunnus 1
	std::vector<std::list<long> > office_home_;

	// Tunnus 2
        std::vector<std::list<long> > home_gym_;

	// Tunnus 3
        std::vector<std::list<long> > gym_home_;

	// Tunnus 4
        std::vector<std::list<long> > office_gym_;

	// Tunnus 5
        std::vector<std::list<long> > gym_office_;

	// Apulista, joka määrittelee, onko solmu kiinnitetty rakennuksiin
        std::map<long, Node*> building_nodes_;

	// Aktiivisuudet Poisson jakaumalle aikavälinä 10s minuutti, jonka arvo vaihtuu tunneittain 
	// Esimerkiksi: aamuviideltä keskimäärin 0.3 ihmistä generoidaan 10s aikana
	std::vector<double> lambdas = {0.3, 0.2, 0.15, 0.15, 0.2, 0.3,   // Tunti hetkestä 0,1,2,3,4,5,...
				       0.7, 1.2,  1.2,  0.7, 0.6, 0.5,   // Tunti hetkestä 6,7,8,9,10,11,...
				       0.6, 0.6,  0.7,  0.8, 1.0, 1.2,   // Tunti hetkestä 12,13,14,15,16,17,...
				       1.0, 0.8,  0.6,  0.5, 0.4, 0.4};  // Tunti hetkestä 18,19,20,21,22,23,...


	// Tässä osuudet, joilla generoidaan ihmisten käyttäytymistä
	// Keskeisellä tavalla tämän tavoite on kuvastaa ihmisten työmatkakäyttäytymistä (aamulla töihin, illalla töistä), mutta tätä voisi halutessaan optimoida vaikka kuinka tarkaksi
	std::vector<std::vector<double> > propotions_routes = {
				 //H.O,   O.H,   H.G,   G.H,   O.G,    G.O
			        { 0.166, 0.166, 0.166, 0.166, 0.166 , 0.166  }, //0
				{ 0.166, 0.166, 0.166, 0.166, 0.166 , 0.166  }, //1
				{ 0.166, 0.166, 0.166, 0.166, 0.166 , 0.166  }, //2
 			        { 0.166, 0.166, 0.166, 0.166, 0.166 , 0.166  }, //3
				{ 0.800, 0.030, 0.080, 0.030, 0.030 , 0.030  }, //4   Isoin osa lähtee kotoa ja menee töihin.
				{ 0.800, 0.025, 0.100, 0.025, 0.025 , 0.025  }, //5.  Isoin osa lähtee kotoa ja menee töihin. Jokin osuus kotoa vapaa-ajan kohteeseen (kauppaan/salille)
			        { 0.800, 0.025, 0.100, 0.025, 0.025 , 0.025  }, //6
				{ 0.800, 0.025, 0.100, 0.025, 0.025 , 0.025  }, //7
				{ 0.800, 0.025, 0.100, 0.025, 0.025 , 0.025  }, //8
			        { 0.600, 0.060, 0.100, 0.060, 0.060 , 0.060  }, //9   Aika suuri osa menee töihin, mutta muiden osuude alkaa kasvaa
				{ 0.600, 0.060, 0.100, 0.060, 0.060 , 0.060  }, //10
				{ 0.600, 0.060, 0.100, 0.060, 0.060 , 0.060  }, //11
			        { 0.300, 0.300, 0.100, 0.100, 0.100 , 0.100  }, //12  Alkaa olla yhtä paljon töistä lähtijöitä kuin tulijoita
				{ 0.300, 0.300, 0.100, 0.100, 0.100 , 0.100  }, //13
				{ 0.200, 0.400, 0.100, 0.100, 0.100 , 0.100  }, //14
			        { 0.100, 0.500, 0.100, 0.100, 0.100 , 0.100  }, //15
				{ 0.025, 0.800, 0.025, 0.100, 0.025 , 0.025  }, //16  Iltaruuhkat
				{ 0.025, 0.800, 0.025, 0.100, 0.025 , 0.025  }, //17
			        { 0.025, 0.800, 0.025, 0.100, 0.025 , 0.025  }, //18
				{ 0.025, 0.800, 0.025, 0.100, 0.025 , 0.025  }, //19
				{ 0.166, 0.166, 0.166, 0.166, 0.166 , 0.166  }, //20  Yöllä liikkeellä olevien ihmisten hajonta on suurta ihmisten
			        { 0.166, 0.166, 0.166, 0.166, 0.166 , 0.166  }, //21
				{ 0.166, 0.166, 0.166, 0.166, 0.166 , 0.166  }, //22
				{ 0.166, 0.166, 0.166, 0.166, 0.166 , 0.166  }  //23
			        }; 

	
	// Vektori, josta näkee suhteet, kuinka paljon erilaisia taloja luodaan (nyt taloista 65% koteja, 20% toimistoja ja 15% vapaa-ajan rakennuksia)
	std::vector<double> building_proportions = {0.65, 0.2, 0.15};

    // Näkyvät tiet analysointia varten
        std::vector<Road*> visible_roads_;
};




