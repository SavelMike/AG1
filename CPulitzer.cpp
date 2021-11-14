// CPulitzer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#ifndef __PROGTEST__
#include <iostream>
#include <iomanip>
#include <cassert>
#include <limits>
#endif
#include <cstdint>
#include <string>

using namespace std;


struct bst_node {
	uint32_t id_politician;
	struct bst_node* left;
	struct bst_node* right;
};

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
	struct bst_node *pop_order;
};

#define MAXUINT32 0xffffffff
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
		return parties[id_party].coalition != MAXUINT32;
	}

	void cancel_party(uint32_t id_party) {
		if (!party_exists(id_party)) {
			return;
		}
		assert(parties[id_party].pop_order == NULL);
		parties[id_party].coalition = MAXUINT32;
	}

	// Input args:
	//      id_politician - index in array of politicians
	// Return values:
	//      false if politician is not registered
	//      true otherwise 
	bool politician_exists(uint32_t id_politician) const {
		return (this->politicians[id_politician].generation != MAXUINT32);
	}

	// this is used by bst_insert() and bst_delete() to compare
	// popularities of politicians
	// Return value:
	//      0 if popularities are equal
	//      -1 if politician1 popularity is smaller
	//      1 if ppolitician1 populatity is higher
	int compare_popularities(uint32_t id_politician1, uint32_t id_politician2) {
		if (politicians[id_politician1].popularity < politicians[id_politician2].popularity) {
			return -1;
		}
		if (politicians[id_politician1].popularity > politicians[id_politician2].popularity) {
			return 1;
		}
		if (politicians[id_politician1].generation < politicians[id_politician2].generation) {
			return -1;
		}
		if (politicians[id_politician1].generation > politicians[id_politician2].generation) {
			return 1;
		}
		return 0;
	}

	// Insert new politician into popularity' order
	// Input args:
	//      root - root of binary search  tree
	//      id_politician - id of politician being inserted
	// Return value
	//      true if new node is inserted
	//      false otherwise
	// This is BSTInsert from AG1, lecture 6, page 10
	bool bst_insert(struct bst_node **root, uint32_t id_politician) {
		if (*root == NULL) {
			*root = new struct bst_node;

			(*root)->id_politician = id_politician;
			(*root)->left = NULL;
			(*root)->right = NULL;
			return true;
		}

		int cmp = compare_popularities((*root)->id_politician, id_politician);
		if (cmp == 0)
			// this is already politician with exactly
			// same popularity/generation. Should never
			// happen as there are no identical
			// popularities
			return false;
		
		if (cmp == 1) {
			return bst_insert(&((*root)->left), id_politician);
		} else {
			return bst_insert(&((*root)->right), id_politician);
		}
	}

	// Return value
	//      politician with highest popularity
	// This is BSTMin from AG1, lecture 6, page 9
	uint32_t bst_max(const struct bst_node *root) const {
		if (root == NULL)
			return P;
		if (root->right == NULL) {
			return root->id_politician;
		}
		return bst_max(root->right);
	}

	// Return value
	//      politician with smallest popularity
	// This is BSTMin from AG1, lecture 6, page 9
	uint32_t bst_min(const struct bst_node *root) const {
		if (root == NULL)
			return P;
		if (root->left == NULL) {
			return root->id_politician;
		}
		return bst_min(root->left);
	}

	// Insert new politician into popularity' order
	// Input args:
	//      root - root of binary search  tree
	//      id_politician - id of politician being inserted
	// Return value
	//      true - the politician successfully deleted from order
	//      false - politician is not found in order, should not happen
	// This is BSTDelete from AG1, lecture 6, page 14
	bool bst_delete(struct bst_node **root, uint32_t id_politician) {
		uint32_t s;

		if (*root == NULL) {
			/* empty root */
			return false;
		}
		
		int cmp = compare_popularities((*root)->id_politician, id_politician);

		if (cmp == 1) {
			return bst_delete(&((*root)->left), id_politician);
		}
		if (cmp == -1) {
			return bst_delete(&((*root)->right), id_politician);
		}
		if ((*root)->left == NULL && (*root)->right == NULL) {
			/* no children */
			delete *root;
			*root = NULL;
			return true;
		}
		if ((*root)->left == NULL) {
			/* only right child */
			struct bst_node *tmp;

			tmp = *root;
			*root = (*root)->right;
			delete tmp;
			return true;
		}
		if ((*root)->right == NULL) {
			/* only left child */
			struct bst_node *tmp;

			tmp = *root;
			*root = (*root)->left;
			delete tmp;
			return true;
		}
		/* two children */
		s = bst_min((*root)->right);
		(*root)->id_politician = s;
		bst_delete(&((*root)->right), s);
		return true;
	}

	void bst_destroy(struct bst_node *root)	{
		if (root == NULL)
			return;
		bst_destroy(root->left);
		bst_destroy(root->right);
		delete root;
	}
	
public:

	CPulitzer(size_t N, size_t P);

	~CPulitzer();

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

	void print();
};

// Bot implementation constructor
// Input:
//      number of allowed political parties and politicians
//      1 <= N,P <= 10^6
CPulitzer::CPulitzer(size_t N, size_t P)
{
	this->P = P;
	politicians = new struct politician[P];
	// mark all lines in array as free
	for (size_t i = 0; i < P; i++) {
		politicians[i].generation = MAXUINT32;
		politicians[i].gender = 0;
		politicians[i].id_party = 0;
		politicians[i].popularity = 0;
		politicians[i].name = string("");
	}
	this->N = N;
	parties = new struct party[N];
	// mark all lines in array as free
	for (size_t i = 0; i < N; i++) {
		parties[i].coalition = MAXUINT32;
		parties[i].pop_order = NULL;
	}
}

CPulitzer::~CPulitzer() {
	delete[] this->politicians;
	for (int i = 0; i < N; i++)
		bst_destroy(parties[i].pop_order);
	delete[] this->parties;
}

// This increases everytime popularity changes and coalition is created
uint32_t generation = 0;

// Return value:
//      false if politician_id exists
//      true if otherwise
bool CPulitzer::register_politician(uint32_t id_party, uint32_t id_politician, 
                                    const string& name, uint32_t popularity, uint8_t gender) {
	// Check if politician with given id exists
	if (politician_exists(id_politician)) {
		return false;
	}
    
	politicians[id_politician].name = name;
	politicians[id_politician].popularity = popularity;
	politicians[id_politician].generation = ++generation;
	politicians[id_politician].gender = gender;
	politicians[id_politician].id_party = id_party;
	if (!party_exists(id_party)) {
		register_party(id_party);       
	}
	bst_insert(&parties[id_party].pop_order, id_politician);

	return true;
}

// Return value:
//      false if there is no registered politician with given id
//      true otherwise  
bool CPulitzer::politician_name(uint32_t id_politician, string& name) const {
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
	// remove from popularity index
	bst_delete(&parties[politicians[id_politician].id_party].pop_order, id_politician);
	politicians[id_politician].generation = MAXUINT32;
	
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

	id_leader = bst_max(parties[id_party].pop_order);
	if (id_leader == P)
		return false;
	return true;
}

//  Return value:
//      false if no politician with given id exists
//      true otherwise
//         the politian is removed from popularity order and inserted with new popularity
bool CPulitzer::change_popularity(uint32_t id_politician, uint32_t popularity) {
	
	if (!politician_exists(id_politician)) {
		return false;
	}

	uint32_t id_party = politicians[id_politician].id_party;
	
	bst_delete(&parties[id_party].pop_order, id_politician);	
	politicians[id_politician].popularity = popularity;
	politicians[id_politician].generation = ++generation;
	bst_insert(&parties[id_party].pop_order, id_politician);
	return true;
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
		// remove from popularity index of src_party
		bst_delete(&parties[src_party].pop_order, i);
		bst_insert(&parties[dest_party].pop_order, i);
	}
    
	cancel_party(src_party);

	return true;
}

// Two parties decided to create a coalition. If any of the parties is
// already part of the coalition, then all coalitions are merged.
// If the party with the given id does not exist, the method returns
// false.  Otherwise, it returns true.
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
	uint32_t coalition = parties[id_party].coalition;
	parties[id_party].coalition = 0;

	// Check whether only one party is still in coalition
	uint32_t cnt_left_parties = 0;
	uint32_t id_left_party = 0;
	for (uint32_t i = 0; i < N; i++) {
		if (parties[i].coalition == coalition) {
			cnt_left_parties++;
			id_left_party = i;
		}
	}
	if (cnt_left_parties == 1) {
		// Last party in coalition leaves coalition
		parties[id_left_party].coalition = 0;
	}
    
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
// If the specified party is not part of any coalition, then the method outputs the party's leader and returns true.
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
	uint32_t id_cur_coalition_leader = MAXUINT32;
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

#ifndef __PROGTEST__
void  CPulitzer::print() {
	cout << std::setw(10);
	cout << std::setw(10) << "Name"; 
	cout << std::setw(10) << "id"; 
	cout << std::setw(10) << "party_id"; 
	cout << std::setw(10) << "popularity";
	cout << std::setw(10) << "gender\n";
	for (size_t i = 0; i < this->P; i++) {
		if (this->politicians[i].generation == MAXUINT32) {
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

int example1()
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

	return 0;
}

int example2() {
	uint32_t id_leader;

	CPulitzer bot(5, 5);
	assert(bot.register_politician(0, 0, "RS", 150, 77) == true); // true
	assert(bot.register_politician(1, 1, "RS", 50, 77) == true); // true
	assert(bot.register_politician(2, 2, "RS", 60, 77) == true); // true
	assert(bot.register_politician(3, 3, "VKml", 100, 77) == true); // true
	assert(bot.register_politician(3, 4, "ZMZ", 50, 70) == true); // true
	assert(bot.deregister_politician(3) == true); // true
	assert(bot.merge_parties(3, 2) == true); // true
	assert(bot.merge_parties(3, 1) == true); // true
	assert(bot.party_leader(0, id_leader) == true && id_leader == 0); // true, 0
	assert(bot.party_leader(1, id_leader) == false); // false
	assert(bot.party_leader(2, id_leader) == false); // false
	assert(bot.party_leader(3, id_leader) == true && id_leader == 2); // true, 2

	return 0;
}

int example3() {;
	uint32_t id_leader;

	CPulitzer bot(10, 10);
	assert(bot.register_politician(9, 1, "MK", 100, 77) == true); // true
	assert(bot.register_politician(0, 0, "IB", 150, 77) == true); // true
	assert(bot.register_politician(1, 2, "VR", 50, 77) == true); // true
	assert(bot.create_coalition(9, 1) == true); // true
	assert(bot.leave_coalition(1) == true); // true
	assert(bot.create_coalition(0, 1) == true); // true
	assert(bot.coalition_leader(0, id_leader) == true && id_leader == 0); // true, 0
	assert(bot.coalition_leader(1, id_leader) == true && id_leader == 0); // true, 0
	assert(bot.coalition_leader(9, id_leader) == true && id_leader == 1); // true, 1
	assert(bot.change_popularity(2, 200) == true); // true
	assert(bot.coalition_leader(0, id_leader) == true && id_leader == 2); // true, 2
	assert(bot.leave_coalition(9) == false); // false

	return 0;
}

int main() {
	example1();
	example2();
	example3();

	return 0;
}
#endif
