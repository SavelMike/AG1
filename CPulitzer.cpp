// CPulitzer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

class CPulitzer {
public:
    CPulitzer(size_t N, size_t P);

    bool register_politician(uint32_t id_party, uint32_t id_politician, const string& name, uint32_t popularity, uint8_t gender);

    bool politician_name(uint32_t id_politician, string& name) const;

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


    // Bot implementation constructor
    // Input:
    //      number of allowed political parties
    //      1 >= N, P <= 10^6
    CPulitzer()
    {

    }

};

// Return value:
//      false if politician_id exists
//      true if otherwise
bool register_politician(politician_id) {

}

// Return value:
//      false if politician_id is not exist
//      true otherwise
bool deregister_politician() {

}

// Return value
//      false if party does not exist
//      politician_id
//      true
bool party_leader() {

}


//  Return value:
//      false if no politician with given id exists
//      true otherwise
bool change_popularity() {

}

// Return value:
//      false if there is no party with given id
//      true otherwise
bool sack_leader() {

}

// Return value
//      true if both parties exist
//      false otherwise
bool merge_leaders(unit32_t dest_party, unit32_t src_party) {

}

// Return value
//      false if party with given id doesn't exist
//      true if otherwise
bool create_coalition() {

}

// Return value
//      false if party is not part of coalition or does not exist
//      true if otherwise
bool leave_coalition() {

}

// Return value
//      false if party does not exist
//      true otherwise
bool scandal_occured() {

}

// Return value
//      true if party is not part of any coalition
//      false otherwise
bool coalition_leader() {

}

// Return value:
//      false if there is no registered politician with given id
//      true otherwise  
bool politician_name() {

}

// Return value:
//      false if there is no registered politician with given id
//      true otherwise
bool politician_popularity() {

}

// Return value:
//      false if there is no registered politician with given id
//      true otherwise
bool politician_gender() {

}

int main()
{
    std::cout << "Hello World!\n";
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
