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
    uint8_t gender;
};

struct party
{
    uint32_t id_party;
};

class CPulitzer {

private:
    std::size_t N; // number of parties
    std::size_t P; // number of politicians
    struct politician* politicians; // array of politicians
    struct party* parties; // array of parties

    void register_party(uint32_t id_party) {
        for (size_t i = 0; i < this->N; i++) {
            if (this->parties[i].id_party == this->N) {
                this->parties[i].id_party = id_party;
                break;
            }
        }
    }
    
    bool party_exists(uint32_t id_party) {
        for (size_t i = 0; i < this->N; i++) {
            if (this->parties[i].id_party == id_party) {
                return true;
            }
        }
        
        return false;
    }

    // Return values
    //      false if politician is not found
    //      true otherwise, argument index containing index of politician in array 
    bool politician_exists(uint32_t id_politician, size_t& index) {
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
            this->politicians[i].gender = gender;
            this->politicians[i].id_party = id_party;
            if (!party_exists(id_party)) {
                register_party(id_party);
            }
            break;
        }
    }

    return true;
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

// Return value
//      false if party does not exist
//      politician_id
//      true
bool CPulitzer::party_leader(uint32_t id_party, uint32_t& id_leader) const {
    return false;
}


//  Return value:
//      false if no politician with given id exists
//      true otherwise
bool CPulitzer::change_popularity(uint32_t id_politician, uint32_t popularity) {
    return false;
}

// Return value:
//      false if there is no party with given id
//      true otherwise
bool CPulitzer::sack_leader(uint32_t id_party) {
    return false;
}

// Return value
//      true if both parties exist
//      false otherwise
bool CPulitzer::merge_parties(uint32_t dest_party, uint32_t src_party) {
    return false;
}

// Return value
//      false if party with given id doesn't exist
//      true if otherwise
bool CPulitzer::create_coalition(uint32_t id_party1, uint32_t id_party2) {
    return false;
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

// Return value
//      true if party is not part of any coalition
//      false otherwise
bool CPulitzer::coalition_leader(uint32_t id_party, uint32_t& id_leader) const {
    return false;
}

// Return value:
//      false if there is no registered politician with given id
//      true otherwise  
bool CPulitzer::politician_name(uint32_t id_politician, std::string& name) const {
    return false;
}

// Return value:
//      false if there is no registered politician with given id
//      true otherwise
bool CPulitzer::politician_popularity(uint32_t id_politician, uint32_t& popularity) const {
    return false;
}

// Return value:
//      false if there is no registered politician with given id
//      true otherwise
bool CPulitzer::politician_gender(uint32_t id_politician, uint8_t& gender) const {
    return false;
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
