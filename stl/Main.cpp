#include "Algorithms.h"
#include "Container.h"
#include <iostream>
#include <iomanip>


// TASK 1
struct Goods
{
	uint32_t m_id;                    // Код товара
	std::string m_name;               // Название
	std::string m_manufacturer;       // Производитель
	std::string m_warehouse_address;  // Адрес склада
	double m_weight;                  // Вес
};


int main() {
	using PairType = std::pair<const uint32_t, Goods>;
	using namespace std;

	// TASK 2
	UnorderedMap<uint32_t, Goods> cont_1{
		{320, {320, "cupboard",   "IKEA",      "Moscow, Pushkin street, 7",     10.25}},
		{117, {117, "shelf",      "MZ5 group", "Saratov, Prospekt mira, 23",    5.62}},
		{54,  {54,  "nightstand", "IKEA",      "Moscow, Tverskaya street, 12",  7.11}},
		{61,  {61,  "chair",      "RIVAL",     "Rostov, Sokolov Avenue, 1",     6.32}},
		{546, {546, "armchair",  "Sanflor",   "Moscow, Tsvetnoy Boulevard, 3", 9.93}},
		{4,   {4,   "table",      "IKEA",      "Moscow, Pushkin street, 7",     4.95}},
		{28,  {28,  "chair",      "MZ5 group", "Saratov, Prospekt mira, 23",    7.0}},
		{9,   {9,   "cupboard",   "Aquanet",   "Omsk, Lenin street, 31",        13.2}},
		{1123,{1123,"cupboard",   "IKEA",      "Moscow, Pushkin street, 7",     20.3}},
		{3,   {3,   "chair",      "RIVAL",     "Rostov, Sokolov Avenue, 1",     5.20}}
	};
	cout << "Contents of cont_1: " << endl;
	forEach(cont_1.begin(), cont_1.end(),
		[](const PairType& pair) {
			cout << "|  Id: " << setw(5) << pair.first << "  |  Name: " << setw(10) << pair.second.m_name << "  |  Manufacturer: " << setw(10) << pair.second.m_manufacturer <<
				"  |  Warehouse address: " << setw(30) << pair.second.m_warehouse_address << "  |  Weight: " << setw(5) << pair.second.m_weight << "  |" << endl;
		});
	cout << endl;


	// TASK 3
	UnorderedMap<uint32_t, Goods> cont_2;
	PairType obj_1 = { 108, {108, "cup", "FixPrice", "Tomsk, Andropov avenue, 8",   0.35} };
	PairType obj_2 = { 1,   {1,   "pen", "FixPrice", "Tomsk, Lomonosov street, 53", 0.13} };
	cont_2.Insert(obj_1);
	cont_2[obj_2.first] = obj_2.second;
	cout << "Contents of cont_2: " << endl;
	forEach(cont_2.begin(), cont_2.end(),
		[](const PairType& pair) {
			cout << "|  Id: " << setw(5) << pair.first << "  |  Name: " << setw(10) << pair.second.m_name << "  |  Manufacturer: " << setw(10) << pair.second.m_manufacturer <<
				"  |  Warehouse address: " << setw(30) << pair.second.m_warehouse_address << "  |  Weight: " << setw(5) << pair.second.m_weight << "  |" << endl;
		});
	cout << endl;


	// TASK 3.1 AND 4 - not feasible for unordered_map


	// TASK 5	
	auto iter = findIf(cont_1.begin(), cont_1.end(),
		[](const PairType& pair) {
			return (7 < pair.second.m_weight && pair.second.m_weight < 9);   // We will look for the first element with a weight from 7 to 9:
		});
	cout << "The first element found with a mass from 7 to 9  from the container cont_1:" << endl;
	cout << "|  Id: " << setw(5) << iter->first << "  |  Name: " << setw(10) << iter->second.m_name << "  |  Manufacturer: " << setw(10) << iter->second.m_manufacturer <<
		"  |  Warehouse address: " << setw(30) << iter->second.m_warehouse_address << "  |  Weight: " << setw(5) << iter->second.m_weight << "  |" << endl;
	cout << endl;

	
	// TASK 6
	iter->second.m_warehouse_address = "Moscow, Pushkin street, 8";
	cout << "The contents of the container cont_1, after replacing the address of the found element with 'Moscow, Pushkin street, 8':" << endl << endl;
	forEach(cont_1.begin(), cont_1.end(),
		[](const PairType& pair) {
			cout << "|  Id: " << setw(5) << pair.first << "  |  Name: " << setw(10) << pair.second.m_name << "  |  Manufacturer: " << setw(10) << pair.second.m_manufacturer <<
				"  |  Warehouse address: " << setw(30) << pair.second.m_warehouse_address << "  |  Weight: " << setw(5) << pair.second.m_weight << "  |" << endl;
		});


	// TASK 7 - not feasible for unordered_map
}
