#ifndef DATABASEANIMALSH_H
#define DATABASEANIMALSH_H
#include <string>
#include <memory>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <vector>

class AnimalShelterDatabase {
private:
	// Driver object for MySQL to handle connection towards the main server
	sql::mysql::MySQL_Driver* driver;
	// Smart pointer for database connection
	std::unique_ptr<sql::Connection> connection;

	// Takes filename as parameter and reads CSV data file to return it as a 2D vector
	static std::vector<std::vector<std::string>> read_CSV(const std::string& filename);

public:
	// Constructor
	// Connects to the MySQL server and creates a database if it doesn't already exist
	// Server address, username, password, and database are taken as arguments
	AnimalShelterDatabase(const std::string& host, const std::string& user, const std::string& password, const std::string& database);

	// Destructor
	// AnimalShelterDatabase is set as default and unique_ptr will handle cleanup
	// If object goes out of scope than the connection to the database is closed
	~AnimalShelterDatabase() = default;

	// Checks if the database in the table is empty
	bool table_empty(const std::string& table_name);

	// CRUD implementation

	// Data from CSV file is imported into database table
	void import_CSV(const std::string& csv_file, const std::string& table_name);

	// CREATE function to make the animal_shelter table
	void create_table();

	// UPDATE function to modify single field
	void update_record(const std::string& table_name, int id, const std::string& column, const std::string& new_value);

	// DELETE function to remove a record from table
	void delete_record(const std::string& table_name, int id);

	// READ function to read and show table displaying page with 10 records
	void display_page(const std::string& table_name, int page_number, int records_per_page = 10, const std::string& sort_column = "", bool ascending = true);

	// Displays column names of the table
	static void table_display();

	// Calculates total number of pages needed for pagination
	int get_total_pages(const std::string& table_name, int records_per_page = 10);

	// Displays animals that fit filter criteria for all types of rescue dogs
	void get_filtered_record(const std::string& table_name, const std::vector<std::string>& breed_names, const std::string& sex_upon_outcome, double min_age_in_weeks, double max_age_in_weeks);

	// Searches records through animal_id and displays specific animal to user
	void search_record(const std::string& table_name, const std::string& animal_id);

	// Creates an interactive map using the animal locations in MySQL
	void createMap(const std::string& table_name);
};

#endif //DATABASEANIMALSH_H
