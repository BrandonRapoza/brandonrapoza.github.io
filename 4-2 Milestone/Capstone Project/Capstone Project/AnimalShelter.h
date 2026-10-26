#ifndef ANIMALSHELTER_H
#define ANIMALSHELTER_H
#include <string>

class AnimalShelter {
// Private member variables within the AnimalShelter class
private: 
	unsigned int unique_id;
	std::string age_upon_outcome;
	std::string animal_id;
	std::string animal_type;
	std::string breed;
	std::string color;
	std::string date_of_birth;
	std::string date_time;
	std::string month_year;
	std::string name;
	std::string outcome_subtype;
	std::string outcome_type;
	std::string sex_upon_outcome;
	double location_lat;
	double location_long;
	double age_upon_outcome_in_weeks;

// Public member variables outside the AnimalShelter class
public:
	AnimalShelter(unsigned int uID,
		const std::string& auo,
		const std::string& aID,
		const std::string& at,
		const std::string& b,
		const std::string& c,
		const std::string& dob,
		const std::string& dt,
		const std::string& my,
		const std::string& n,
		const std::string& os,
		const std::string& ot,
		const std::string& suo,
		double llat,
		double llong,
		double auoiw);

// Getters
// Code outside the AnimalShelter class will be able to retrieve values of private member variables
	unsigned int get_unique_id() const;
	std::string get_age_upon_outcome() const;
	std::string get_animal_id() const;
	std::string get_animal_type() const;
	std::string get_breed() const;
	std::string get_color() const;
	std::string get_date_of_birth() const;
	std::string get_date_time() const;
	std::string get_month_year() const;
	std::string get_name() const;
	std::string get_outcome_subtype() const;
	std::string get_outcome_type() const;
	std::string get_sex_upon_outcome() const;
	double get_location_lat() const;
	double get_location_long() const;
	double get_age_upon_outcome_in_weeks() const;
};

#endif // ANIMALSHELTER_H