#include "AnimalShelter.h"
#include <iostream>
#include <iomanip>

// Constructor
// Initialize AnimalShelter object and assigns value to member variables within object
AnimalShelter::AnimalShelter(unsigned int uID, const std::string& auo, const std::string& aID,
	const std::string& at, const std::string& b, const std::string& c, const std::string& dob,
	const std::string& dt, const std::string& my, const std::string& n, const std::string& os,
	const std::string& ot, const std::string& suo, double llat, double llong, double auoiw) :
	unique_id(uID), age_upon_outcome(auo), animal_id(aID), animal_type(at), breed(b), color(c),
	date_of_birth(dob), date_time(dt), month_year(my), name(n), outcome_subtype(os), outcome_type(ot),
	sex_upon_outcome(suo), location_lat(llat), location_long(llong), age_upon_outcome_in_weeks(auoiw)
{
}

// Getter
// Retrieves value from private member variable

unsigned int AnimalShelter::get_unique_id() const
{
	return unique_id;
}

std::string AnimalShelter::get_age_upon_outcome() const
{
	return age_upon_outcome;
}

std::string AnimalShelter::get_animal_id() const
{
	return animal_id;
}

std::string AnimalShelter::get_animal_type() const
{
	return animal_type;
}

std::string AnimalShelter::get_breed() const
{
	return breed;
}

std::string AnimalShelter::get_color() const
{
	return color;
}

std::string AnimalShelter::get_date_of_birth() const
{
	return date_of_birth;
}

std::string AnimalShelter::get_date_time() const
{
	return date_time;
}

std::string AnimalShelter::get_month_year() const
{
	return month_year;
}

std::string AnimalShelter::get_name() const
{
	return name;
}

std::string AnimalShelter::get_outcome_subtype() const
{
	return outcome_subtype;
}

std::string AnimalShelter::get_outcome_type() const
{
	return outcome_type;
}

std::string AnimalShelter::get_sex_upon_outcome() const
{
	return sex_upon_outcome;
}

double AnimalShelter::get_location_lat() const
{
	return location_lat;
}

double AnimalShelter::get_location_long() const
{
	return location_long;
}

double AnimalShelter::get_age_upon_outcome_in_weeks() const
{
	return age_upon_outcome_in_weeks;
}
