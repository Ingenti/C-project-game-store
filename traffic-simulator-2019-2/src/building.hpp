// Talot kartalla. Corners sisältää listan solmujen ID:stä, jotka ovat talon kulmia.



class Building {
    public:
	// Rakennuksen luontiin vaadittavia parametreja
        Building(long id, std::list<long> corners, int type) : id_(id), corners_(corners), type_(type) {}
        std::list<long> Corners() { return corners_; }
        int Type() { return type_; }
        void AddNode(long id) { corner_node_ = id; }
        long CornerNode() { return corner_node_; }

    private:
        const long id_;
        std::list<long> corners_;

        // Rakennuksen tyyppi: 0 koti, 1 toimisto, 2 sali
        int type_;
        long corner_node_;
};  
