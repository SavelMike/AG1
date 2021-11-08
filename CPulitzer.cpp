// CPulitzer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <iomanip>

using namespace std;

struct politician 
{
    uint32_t id_party;
    uint32_t id_politician;
    string name;
    uint32_t popularity;
    uint32_t popularity_change_cnt;
    uint8_t gender;
    
};

struct party
{
    uint32_t id_party;
    uint32_t coalition;  // 0 if party is not in coalition
};

class CPulitzer {

private:
    std::size_t N; // number of parties
    std::size_t P; // number of politicians
    struct politician* politicians; // array of politicians
    struct party* parties; // array of parties

    void register_party(uint32_t id_party) {
        for (size_t i = 0; i < N; i++) {
            if (parties[i].id_party == N) {
                parties[i].id_party = id_party;
                parties[i].coalition = 0;
                break;
            }
        }
    }
    
    bool party_exists(uint32_t id_party, size_t& index) const {
        for (size_t i = 0; i < this->N; i++) {
            if (this->parties[i].id_party == id_party) {
                index = i;
                return true;
            }
        }
        
        return false;
    }

    void cancel_party(uint32_t id_party) {
        size_t index;
        if (!party_exists(id_party, index)) {
            return;
        }
        parties[index].id_party = N;
    }

    // Return values
    //      false if politician is not found
    //      true otherwise, argument index containing index of politician in array 
    bool politician_exists(uint32_t id_politician, size_t& index) const {
        for (size_t i = 0; i < this->P; i++) {
            if (this->politicians[i].id_politician == id_politician) {
                index = i;
                return true;
            }
        }

        return false;
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
        this->politicians[i].id_politician = P;
    }
    this->N = N;
    this->parties = new struct party[N];
    // mark all lines in array as free
    for (size_t i = 0; i < N; i++) {
        this->parties[i].id_party = N;
    }
}

CPulitzer::~CPulitzer() {
    delete[] this->politicians;
    delete[] this->parties;
}

// This increases everytime popularity changes
uint32_t change_popularity_cnt = 0;

// Return value:
//      false if politician_id exists
//      true if otherwise
bool CPulitzer::register_politician(uint32_t id_party, uint32_t id_politician, 
                                    const std::string& name, uint32_t popularity, uint8_t gender) {
    size_t index;
    // Check if politician with given id exists
    if (politician_exists(id_politician, index)) {
        return false;
    }
    // Search for free space, save politician
    for (size_t i = 0; i < this->P; i++) {
        if (this->politicians[i].id_politician == this->P) {
            // Empty field is found
            this->politicians[i].id_politician = id_politician;
            this->politicians[i].name = name;
            this->politicians[i].popularity = popularity;
            this->politicians[i].popularity_change_cnt = ++change_popularity_cnt;
            this->politicians[i].gender = gender;
            this->politicians[i].id_party = id_party;
            if (!party_exists(id_party, index)) {
                register_party(id_party);
            }
            break;
        }
    }

    return true;
}

//  Return value:
//      false if no politician with given id exists
//      true otherwise
bool CPulitzer::change_popularity(uint32_t id_politician, uint32_t popularity) {
    size_t index;
    if (politician_exists(id_politician, index) == true) {
          politicians[index].popularity = popularity;
          politicians[index].popularity_change_cnt = ++change_popularity_cnt;
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
    size_t index;
    if (!party_exists(dest_party, index) || !party_exists(src_party, index)) {
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
    size_t index1;
    size_t index2;
    if (!party_exists(id_party1, index1) || !party_exists(id_party2, index2)) {
        return false;
    }
    if (parties[index1].coalition == 0 && parties[index2].coalition == 0) {
        change_popularity_cnt++;
        parties[index1].coalition = parties[index2].coalition = change_popularity_cnt;
        return true;
    }
    if (parties[index1].coalition != 0 && parties[index2].coalition == 0) {
        parties[index2].coalition = parties[index1].coalition;
        return true;
    }
    if (parties[index1].coalition == 0 && parties[index2].coalition != 0) {
        parties[index1].coalition = parties[index2].coalition;
        return true;
    }
    for (size_t i = 0; i < N; i++) {
        if (parties[index1].coalition == parties[i].coalition) {
            parties[i].coalition = parties[index2].coalition;
        }
    }

    return true;
}

// Return value
//      false if party is not part of coalition or does not exist
//      true if otherwise
bool CPulitzer::leave_coalition(uint32_t id_party) {
    return false;
}

// Return value
//      false if party does not exist
//      true otherwise
bool CPulitzer::scandal_occured(uint32_t id_party) {
    return false;
}

// The method obtains the current coalition leader, which is a politician 
// with the highest popularity over all parties in the coalition.
// If the specified party is not part of any coalition, then the method outputs the party’s leader and returns true.
// If the party with a specified id does not exist, the method returns false.
bool CPulitzer::coalition_leader(uint32_t id_party, uint32_t& id_leader) const {
    return false;
}

// Return value:
//      false if there is no registered politician with given id
//      true otherwise  
bool CPulitzer::politician_name(uint32_t id_politician, std::string& name) const {
    size_t index;
    if (politician_exists(id_politician, index) == true) {
        name = politicians[index].name;
        return true;
    }
    return false;
}

// Return value:
//      false if there is no registered politician with given id
//      true otherwise
bool CPulitzer::politician_gender(uint32_t id_politician, uint8_t& gender) const {
    size_t index;
    if (politician_exists(id_politician, index) == true) {
        gender = politicians[index].gender;
        return true;
    }
    return false;
}

// Return value:
//      false if there is no registered politician with given id
//      true otherwise
bool CPulitzer::politician_popularity(uint32_t id_politician, uint32_t& popularity) const {
    size_t index;
    if (politician_exists(id_politician, index) == true) {
        popularity = politicians[index].popularity;
        return true;
    }
    return false;
}

// Return value:
//      false if politician_id is not exist
//      true otherwise
bool CPulitzer::deregister_politician(uint32_t id_politician) {
    size_t index;
    if (!politician_exists(id_politician, index)) {
        return false;
    }
    this->politicians[index].id_politician = P;

    return true;
}

// The method determines the leader of the given party.
// If the party does not exist, the method returns false.
// Otherwise, the output parameter id_leader contains the id of the politician with 
// the highest popularity through all party members.
// If two politicians have the same popularity, then the leader is the politician who 
// received this popularity at a later time.
bool CPulitzer::party_leader(uint32_t id_party, uint32_t& id_leader) const {
    size_t index;
    if (!party_exists(id_party, index)) {
        return false;
    }
    uint32_t cur_max_popularity = 0;
    uint32_t cur_cnt = 0;
    id_leader = this->P;
    for (size_t i = 0; i < P; i++) {
        if (politicians[i].id_politician == P || politicians[i].id_party != id_party || 
            politicians[i].popularity < cur_max_popularity) {
            continue;
        }
        if (politicians[i].popularity > cur_max_popularity) {
            cur_max_popularity = politicians[i].popularity;
            cur_cnt = politicians[i].popularity_change_cnt;
            id_leader = politicians[i].id_politician;
            continue;
        }
        
        if (politicians[i].popularity_change_cnt > cur_cnt) {
            cur_max_popularity = politicians[i].popularity;
            cur_cnt = politicians[i].popularity_change_cnt;
            id_leader = politicians[i].id_politician;
        }
    }

    if (id_leader == this->P) {
        // No politicians of this party
        return false;
    }
    return true;
}


void  CPulitzer::print() {
    cout << std::setw(10);
    cout << std::setw(10) << "Name"; 
    cout << std::setw(10) << "id"; 
    cout << std::setw(10) << "party_id"; 
    cout << std::setw(10) << "popularity";
    cout << std::setw(10) << "gender\n";
    for (size_t i = 0; i < this->P; i++) {
        if (this->politicians[i].id_politician == this->P) {
            cout << "free\n";
            continue;
        }
        cout << std::setw(10) << this->politicians[i].name;
        cout << std::setw(10) << this->politicians[i].id_politician;
        cout << std::setw(10) << this->politicians[i].id_party;
        cout << std::setw(10) << this->politicians[i].popularity;
        cout << std::setw(10) << this->politicians[i].gender << "\n";
    }
}

int main()
{
//    int8_t gender;
//    uint32_t popularity, id_leader;
//    std::string name;

    CPulitzer bot(3, 10);
//    bot.party_leader(1, id_leader); // false
    bot.register_politician(1, 5, "VK", 1000, 77); // true
    bot.register_politician(2, 4, "MZ", 1000, 77); // true
    bot.register_politician(2, 7, "VS", 500, 77); // true
    bot.deregister_politician(4);
    bot.print();
    string name;

    bot.politician_name(5, name);
    cout << "Name " << name << "\n";
 
    cin.get();

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
