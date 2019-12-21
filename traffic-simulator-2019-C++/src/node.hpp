#include <utility>

// Yleinen solmu kartalla. Sisältää sijainnin koordinaattiesityksenä ja ID:n

class Node {
    public:
        // Luodaan node, jolla id ja sijainti maailmankartalla
        Node(long id, float lat, float lon) : id_(id), lat_(lat), lon_(lon) {}
        // Palauttaa noden koordinaatit std::pairina
        const std::pair<float,float> Location() const { return std::make_pair(lat_, lon_); }
        long ID() { return id_; }
    private:
        long id_;
        const float lat_;
        const float lon_;
};
