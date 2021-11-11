// CPulitzer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <iomanip>
#include <cassert>

using namespace std;

struct politician 
{
    uint32_t id_party;
    string name;
    uint32_t popularity;
    uint32_t generation; // is 0 if a politician is not registered  
    uint8_t gender;
};

struct party
{
    uint32_t coalition;  // 0 if party is not in coalition
};

class CPulitzer {

private:
    std::size_t N; // number of parties
    std::size_t P; // number of politicians
    struct politician* politicians; // array of politicians
    struct party* parties; // array of parties

    void register_party(uint32_t id_party) {
        parties[id_party].coalition = 0;
    }
    
    bool party_exists(uint32_t id_party) const {
        return parties[id_party].coalition = numeric_limits<uint32_t>::max();
    }

    void cancel_party(uint32_t id_party) {
        if (!party_exists(id_party)) {
            return;
        }
        parties[id_party].coalition = numeric_limits<uint32_t>::max();
    }

    // Input args:
    //      id_politician - index in array of politicians
    // Return values:
    //      false if politician is not registered
    //      true otherwise 
    bool politician_exists(uint32_t id_politician) const {
        return (this->politicians[id_politician].generation != numeric_limits<uint32_t>::max());
    }

public:

    CPulitzer(std::size_t N, std::size_t P);

    ~CPulitzer();

    bool register_politician(uint32_t id_party, uint32_t id_politician, const std::string& name, uint32_t popularity, uint8_t gender);

    bool politician_name(uint32_t id_politician, std::string& name) const;

    bool politician_gender(uint32_t id_politician, uint8_t& gender) const;

    bool politician_popularity(uint32_t id_politician, uint32_t& popularity) const;

    bool deregister_politician(uint32_t id_politician);

    bool party_leader(uint32_t id_party, uint32_t& id_leader) const;

    bool change_popularity(uint32_t id_politician, uint32_t popularity);

    bool sack_leader(uint32_t id_party);

    bool merge_parties(uint32_t dest_party, uint32_t src_party);

    bool create_coalition(uint32_t id_party1, uint32_t id_party2);

    bool leave_coalition(uint32_t id_party);

    bool coalition_leader(uint32_t id_party, uint32_t& id_leader) const;

    bool scandal_occured(uint32_t id_party);

    void print();
};

// Bot implementation constructor
// Input:
//      number of allowed political parties and politicians
//      1 <= N,P <= 10^6
CPulitzer::CPulitzer(size_t N, size_t P)
{
    this->P = P;
    this->politicians = new struct politician[P];
    // mark all lines in array as free
    for (size_t i = 0; i < P; i++) {
        this->politicians[i].generation = numeric_limits<uint32_t>::max();
    }
    this->N = N;
    this->parties = new struct party[N];
    // mark all lines in array as free
    for (size_t i = 0; i < N; i++) {
        this->parties[i].coalition = numeric_limits<uint32_t>::max();
    }
}

CPulitzer::~CPulitzer() {
    delete[] this->politicians;
    delete[] this->parties;
}

// This increases everytime popularity changes and coalition is created
uint32_t generation = 0;

// Return value:
//      false if politician_id exists
//      true if otherwise
bool CPulitzer::register_politician(uint32_t id_party, uint32_t id_politician, 
                                    const std::string& name, uint32_t popularity, uint8_t gender) {
    // Check if politician with given id exists
    if (politician_exists(id_politician)) {
        return false;
    }
    
    this->politicians[id_politician].name = name;
    this->politicians[id_politician].popularity = popularity;
    this->politicians[id_politician].generation = ++generation;
    this->politicians[id_politician].gender = gender;
    this->politicians[id_politician].id_party = id_party;
    if (!party_exists(id_party)) {
        register_party(id_party);       
    }

    return true;
}

// Return value:
//      false if there is no registered politician with given id
//      true otherwise  
bool CPulitzer::politician_name(uint32_t id_politician, std::string& name) const {
    if (politician_exists(id_politician)) {
        name = politicians[id_politician].name;
        return true;
    }
    return false;
}

// Return value:
//      false if there is no registered politician with given id
//      true otherwise
bool CPulitzer::politician_gender(uint32_t id_politician, uint8_t& gender) const {
    if (politician_exists(id_politician)) {
        gender = politicians[id_politician].gender;
        return true;
    }
    return false;
}

// Return value:
//      false if there is no registered politician with given id
//      true otherwise
bool CPulitzer::politician_popularity(uint32_t id_politician, uint32_t& popularity) const {
    if (politician_exists(id_politician)) {
        popularity = politicians[id_politician].popularity;
        return true;
    }
    return false;
}

// Return value:
//      false if politician_id is not exist
//      true otherwise
bool CPulitzer::deregister_politician(uint32_t id_politician) {
    if (!politician_exists(id_politician)) {
        return false;
    }
    this->politicians[id_politician].generation = numeric_limits<uint32_t>::max();

    return true;
}

// The method determines the leader of the given party.
// If the party does not exist, the method returns false.
// Otherwise, the output parameter id_leader contains the id of the politician with 
// the highest popularity through all party members.
// If two politicians have the same popularity, then the leader is the politician who 
// received this popularity at a later time.
bool CPulitzer::party_leader(uint32_t id_party, uint32_t& id_leader) const {
    if (!party_exists(id_party)) {
        return false;
    }
    uint32_t cur_max_popularity = 0;
    uint32_t cur_generation = 0;
    id_leader = P;
    for (size_t i = 0; i < P; i++) {
        if (politicians[i].generation == numeric_limits<uint32_t>::max() ||
            politicians[i].id_party != id_party || 
            politicians[i].popularity < cur_max_popularity) {
            continue;
        }

        if (politicians[i].popularity > cur_max_popularity) {
            cur_max_popularity = politicians[i].popularity;
            cur_generation = politicians[i].generation;
            id_leader = i;
            continue;
        }

        if (politicians[i].generation > cur_generation) {
            cur_max_popularity = politicians[i].popularity;
            cur_generation = politicians[i].generation;
            id_leader = i;
        }
    }

    if (id_leader == P) {
        // No politicians of this party
        return false;
    }
    return true;
}

//  Return value:
//      false if no politician with given id exists
//      true otherwise
bool CPulitzer::change_popularity(uint32_t id_politician, uint32_t popularity) {
    if (politician_exists(id_politician)) {
        politicians[id_politician].popularity = popularity;
        politicians[id_politician].generation = ++generation;
        return true;
    }
    return false;
}

// Due to the results of the last elections, the leader of the given party leaves politics.
// The method returns false if there is no party with the given id, otherwise, 
// it returns true.
bool CPulitzer::sack_leader(uint32_t id_party) {
    uint32_t id_leader;
    if (!party_leader(id_party, id_leader)) {
        return false;
    }
    
    return deregister_politician(id_leader);
}

// The method merges dest_party with src_party.
// All members of src_party are trasfered into dest_party and the src_party is canceled.
// If both parties exists, the method returns true, otherwise, it returns false.
bool CPulitzer::merge_parties(uint32_t dest_party, uint32_t src_party) {
    if (!party_exists(dest_party) || !party_exists(src_party) || (dest_party == src_party)) {
        return false;
    }
    for (size_t i = 0; i < P; i++) {
        if (politicians[i].id_party != src_party) {
            continue;
        }
        politicians[i].id_party = dest_party;
    }
    
    cancel_party(src_party);

    return true;
}

// Two parties decided to create a coalition.If any of the parties is already part of the coalition, 
// then all coalitions are merged.
// If the party with the given id does not exist, the method returns false.
// Otherwise, it returns true.
bool CPulitzer::create_coalition(uint32_t id_party1, uint32_t id_party2) {
    if (!party_exists(id_party1) || !party_exists(id_party2) || (id_party1 == id_party2)) {
        return false;
    }
    if (parties[id_party1].coalition == 0 && parties[id_party2].coalition == 0) {
        parties[id_party1].coalition = parties[id_party2].coalition = ++generation;
        return true;
    }
    if (parties[id_party1].coalition != 0 && parties[id_party2].coalition == 0) {
        parties[id_party2].coalition = parties[id_party1].coalition;
        return true;
    }
    if (parties[id_party1].coalition == 0 && parties[id_party2].coalition != 0) {
        parties[id_party1].coalition = parties[id_party2].coalition;
        return true;
    }
    for (size_t i = 0; i < N; i++) {
        if (parties[id_party1].coalition == parties[i].coalition) {
            parties[i].coalition = parties[id_party2].coalition;
        }
    }

    return true;
}

// The given party is leaving the coalition. If the party is not a member of a coalition, 
// or the party does not exist at all, the method returns false.
// Otherwise, the method returns true.
bool CPulitzer::leave_coalition(uint32_t id_party) {

    if (!party_exists(id_party) || parties[id_party].coalition == 0) {
        return false;
    }
    parties[id_party].coalition = 0;
    return true;
}

// A scandal broke out.The leader of the given political party should leave politics.
// If the party is a member of a coalition, the same holds for the coalition leader.
// If the party does not exist, the method returns false.Otherwise, the method returns true.
bool CPulitzer::scandal_occured(uint32_t id_party) {
    uint32_t id_leader;

    if (!party_exists(id_party)) {
        return false;
    }
    sack_leader(id_party);
    if (parties[id_party].coalition > 0) {
        if (coalition_leader(id_party, id_leader)) {
            deregister_politician(id_leader);
        }
    }

    return true;
}

// The method obtains the current coalition leader, which is a politician 
// with the highest popularity over all parties in the coalition.
// If the specified party is not part of any coalition, then the method outputs the party’s leader and returns true.
// If the party with a specified id does not exist, the method returns false.
bool CPulitzer::coalition_leader(uint32_t id_party, uint32_t& id_leader) const {

    if (!party_exists(id_party)) {
        return false;
    }
    if (parties[id_party].coalition == 0) {
        // Party is not in coalition
        party_leader(id_party, id_leader);
        return true;
    }
    uint32_t id_cur_coalition_leader = numeric_limits<uint32_t>::max();
    uint32_t cur_popularity = 0;
    uint32_t cur_generation = 0;
    for (size_t i = 0; i < N; i++) {
        if (parties[id_party].coalition == parties[i].coalition) {
            if (!party_leader(i, id_leader)) {
                continue;
            }
            if (cur_generation == 0 || 
                politicians[id_leader].popularity > cur_popularity ||
                (politicians[id_leader].popularity == cur_popularity &&
                 politicians[id_leader].generation > cur_generation)) {
                id_cur_coalition_leader = id_leader;
                cur_popularity = politicians[id_leader].popularity;
                cur_generation = politicians[id_leader].generation;
            }
           
        }
    }

    if (cur_generation == 0) {
        return false;
    }
    id_leader = id_cur_coalition_leader;

    return true;
}

#if 0
void  CPulitzer::print() {
    cout << std::setw(10);
    cout << std::setw(10) << "Name"; 
    cout << std::setw(10) << "id"; 
    cout << std::setw(10) << "party_id"; 
    cout << std::setw(10) << "popularity";
    cout << std::setw(10) << "gender\n";
    for (size_t i = 0; i < this->P; i++) {
        if (this->politicians[i].generation == numeric_limits<uint32_t>::max()) {
            cout << "free\n";
            continue;
        }
        cout << std::setw(10) << this->politicians[i].name;
        cout << std::setw(10) << i;
        cout << std::setw(10) << this->politicians[i].id_party;
        cout << std::setw(10) << this->politicians[i].popularity;
        cout << std::setw(10) << this->politicians[i].gender << "\n";
    }
}


int main()
{
    uint32_t id_leader;
    CPulitzer bot(3, 10);
    
    assert(bot.party_leader(1, id_leader) == false); // false
    assert(bot.register_politician(1, 5, "VK", 1000, 77) == true); // true
    assert(bot.register_politician(2, 4, "MZ", 1000, 77) == true); // true
    assert(bot.register_politician(2, 7, "VS", 500, 77) == true); // true
    assert(bot.party_leader(1, id_leader) == true && id_leader == 5); // true, 5
    assert(bot.party_leader(2, id_leader) == true && id_leader == 4); // true, 4
    assert(bot.change_popularity(7, 2000) == true); // true
    assert(bot.party_leader(2, id_leader) == true && id_leader == 7); // true, 7
    assert(bot.register_politician(1, 2, "MT", 500, 77) == true); // true
    assert(bot.register_politician(2, 2, "JP", 500, 77) == false); // false
    assert(bot.register_politician(2, 9, "JP", 500, 77) == true); // true
    assert(bot.deregister_politician(5) == true); // true
    assert(bot.party_leader(1, id_leader) == true && id_leader == 2); // true, 2
    assert(bot.sack_leader(2) == true); // true
    assert(bot.change_popularity(9, 200) == true); // true
    assert(bot.sack_leader(1) == true); // true

    bot.print();
    string name;
 
    cin.get();

    return 0;
}
#endif

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
