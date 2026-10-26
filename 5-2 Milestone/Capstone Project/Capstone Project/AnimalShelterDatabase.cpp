#include "AnimalShelterDatabase.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <vector>

// Functions for interactive map

// Escapes text before inserting it into HTML
static std::string htmlEscape(const std::string& value)
{
	std::string result;

	for (char c : value) {
		switch (c) {
		case '&':  result += "&amp;";  break;
		case '<':  result += "&lt;";   break;
		case '>':  result += "&gt;";   break;
		case '"':  result += "&quot;"; break;
		case '\'': result += "&#39;";  break;
		default:   result += c;        break;
		}
	}
	return result;
}

// Quotes and escapes a string for use in JavaScript
static std::string jsString(const std::string& value) {
	std::ostringstream result;
	result << '"';

	for (unsigned char c : value) {
		switch (c) {
		case '\\': result << "\\\\"; break;
		case '"':  result << "\\\""; break;
		case '\n': result << "\\n";  break;
		case '\r': result << "\\r";  break;
		case '\t': result << "\\t";  break;
		default:
			if (c < 0x20) {
				result << "\\u"
					<< std::hex << std::setw(4)
					<< std::setfill('0')
					<< static_cast<int>(c)
					<< std::dec;
			}
			else {
				result << static_cast<char>(c);
			}
		}
	}
	result << '"';
	return result.str();
}

// Reads CSV file to convert it to a 2D vector of strings
std::vector<std::vector<std::string>> AnimalShelterDatabase::read_CSV(const std::string& filename)
{
	std::vector<std::vector<std::string>> data;
	std::ifstream file(filename);
	std::string line;

	while (std::getline(file, line)) {
		std::vector<std::string> row;
		std::stringstream ss(line);
		std::string cell;

		while (std::getline(ss, cell, ',')) {
			row.push_back(cell);
		}
		data.push_back(row);
	}
	return data;
}

// Constructor
// Sets up connection needed to utilize MySQL database
AnimalShelterDatabase::AnimalShelterDatabase(const std::string& host, const std::string& user, const std::string& password, const std::string& database)
{
	try {
		// Gets driver instance
		driver = sql::mysql::get_mysql_driver_instance();
		// Establishes connection with given credentials
		connection = std::unique_ptr<sql::Connection>(driver->connect(host, user, password));
		// Creates database if it doesn't exist
		std::unique_ptr<sql::Statement> stmt(connection->createStatement());
		stmt->execute("CREATE DATABASE IF NOT EXISTS animal_shelter");
		// Selects database to use
		connection->setSchema(database);
		std::cout << std::endl << "Access has been granted." << std::endl;
		std::cout << "Connected to MySQL." << std::endl;
		std::cout << "Checking table for import..." << std::endl;
	}
	// If error has occurred
	catch (sql::SQLException& e) {
		std::cerr << std::endl << "An error has occurred while trying to connect: " << e.what() << std::endl;
	}
}

// Checks if the table is empty of records or not
bool AnimalShelterDatabase::table_empty(const std::string& table_name)
{
	// Checks for table name
	try {
		std::unique_ptr<sql::Statement> stmt(connection->createStatement());
		std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT COUNT(*) as count FROM " + table_name));

		// If count equals to 0 then return true
		if (res->next()) {
			int count = res->getInt("count");
			return count == 0;
		}
	}
	// Gives error if table doesn't exist then return true to treat it as empty
	catch (sql::SQLException& e) {
		if (e.getErrorCode() == 1146) {
			return true;
		}
		std::cerr << "Error has been made trying to check table: " << e.what() << std::endl;
	}
	return false;
}

// Takes data from the CSV file to import it into database table
void AnimalShelterDatabase::import_CSV(const std::string& csv_file, const std::string& table_name)
{
	// Checks that table isn't empty before import
	if (!table_empty(table_name)) {
		std::cout << "Table " << table_name << " already contains data. Skipping import" << std::endl;
		return;
	}

	// Read data from CSV file
	auto data = read_CSV(csv_file);

	// Insert all values into table
	try {
		std::unique_ptr<sql::PreparedStatement> pstmt(connection->prepareStatement("INSERT INTO " + table_name +
			" (id, age_upon_outcome, animal_id, animal_type, breed, color, date_of_birth, "
			"datetime, monthyear, name, outcome_subtype, outcome_type, "
			"sex_upon_outcome, location_lat, location_long, age_upon_outcome_in_weeks) "
			"VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)"));

		// For each row of data
		for (size_t i = 1; i < data.size(); i++) {
			pstmt->setInt(1, std::stoi(data[i][0])); // id
			pstmt->setString(2, data[i][1]); // age_upon_outcome
			pstmt->setString(3, data[i][2]); // animal_id
			pstmt->setString(4, data[i][3]); // animal_type
			pstmt->setString(5, data[i][4]); // breed
			pstmt->setString(6, data[i][5]); // color
			pstmt->setString(7, data[i][6]); // date_of_birth
			pstmt->setString(8, data[i][7]); // datetime
			pstmt->setString(9, data[i][8]); // monthyear
			pstmt->setString(10, data[i][9]); // name
			pstmt->setString(11, data[i][10]); // outcome_subtype
			pstmt->setString(12, data[i][11]); // outcome_type
			pstmt->setString(13, data[i][12]); // sex_upon_outcome
			pstmt->setDouble(14, std::stod(data[i][13])); // location_lat
			pstmt->setDouble(15, std::stod(data[i][14])); // location_long
			pstmt->setDouble(16, std::stod(data[i][15])); // age_upon_outcome_in_weeks
			pstmt->execute();
		}
		std::cout << "Imported " << (data.size() - 1) << " rows of data. " << std::endl;
	}
	// If error has occurred
	catch (sql::SQLException& e) {
		std::cerr << "Import error has occurred: " << e.what() << std::endl;
	}
}

// Creates animal_shelter table if it doesn't exist
void AnimalShelterDatabase::create_table()
{
	// Creates table
	try {
		std::unique_ptr<sql::Statement> stmt(connection->createStatement());
		stmt->execute(
			"CREATE TABLE IF NOT EXISTS animal_shelter ("
			"id INT PRIMARY KEY, "
			"age_upon_outcome VARCHAR(100), "
			"animal_id VARCHAR(100), "
			"animal_type VARCHAR(100), "
			"breed VARCHAR(100), "
			"color VARCHAR(100), "
			"date_of_birth VARCHAR(100), "
			"datetime VARCHAR(100), "
			"monthyear VARCHAR(100), "
			"name VARCHAR(100), "
			"outcome_subtype VARCHAR(100), "
			"outcome_type VARCHAR(100), "
			"sex_upon_outcome VARCHAR(100), "
			"location_lat DOUBLE, "
			"location_long DOUBLE, "
			"age_upon_outcome_in_weeks DOUBLE"
			")");
	}
	// If error has occurred
	catch (sql::SQLException& e) {
		std::cerr << "Table creation has suffered an error: " << e.what() << std::endl;
	}
}

// Updates a specific column with a specific record
void AnimalShelterDatabase::update_record(const std::string& table_name, int id, const std::string& column, const std::string& new_value)
{
	// Colums user is allowed to update
	const std::vector<std::string> allowed_columns = {
		"age_upon_outcome",
		"animal_type",
		"breed",
		"color",
		"name",
		"outcome_subtype",
		"outcome_type",
		"sex_upon_outcome"
	};

	// Checks if column is allowed
	if (std::find(allowed_columns.begin(),
		allowed_columns.end(),
		column) == allowed_columns.end()) {
		std::cerr << "Invalid column name. Update has been cancelled."
			<< std::endl;
		return;
	}
	
	// Updates prior record with new value
	try {
		std::unique_ptr<sql::PreparedStatement> pstmt(connection->prepareStatement("UPDATE " + table_name + " SET " + column + " = ? WHERE id = ?"));
		pstmt->setString(1, new_value);
		pstmt->setInt(2, id);

		int rows = pstmt->executeUpdate();
		std::cout << "Updated " << rows << " record(s)." << std::endl;
	}
	// If error has occurred
	catch (sql::SQLException& e) {
		std::cerr << "A error has occurred while trying to update record: " << e.what() << std::endl;
	}
}

// Deletes a specific record from the table
void AnimalShelterDatabase::delete_record(const std::string& table_name, int id)
{
	// Deletes prior record from database
	try {
		std::unique_ptr<sql::PreparedStatement> pstmt(connection->prepareStatement("DELETE FROM " + table_name + " WHERE id = ?"));
		pstmt->setInt(1, id);

		int rows = pstmt->executeUpdate();
		std::cout << "Deleted " << rows << " record(s)." << std::endl;
	}
	// If error has occurred
	catch (sql::SQLException& e) {
		std::cerr << "A error has occurred while trying to delete record: " << e.what() << std::endl;
	}
}

// Reads and displays information within the table on a specific page
void AnimalShelterDatabase::display_page(const std::string& table_name, int page_number, int records_per_page, const std::string& sort_column, bool ascending)
{
	// Reads data from page in chosen order
	try {
		int offset = (page_number - 1) * records_per_page;
		std::string order_by_clause = "";
		if (!sort_column.empty()) {
			order_by_clause = " ORDER BY " + sort_column + " " + (ascending ? "ASC" : "DESC");
		}

		std::unique_ptr<sql::PreparedStatement> pstmt(connection->prepareStatement("SELECT * FROM " + table_name + order_by_clause + " LIMIT ? OFFSET ?"));
		pstmt->setInt(1, records_per_page);
		pstmt->setInt(2, offset);
		std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

		// Information shown to user
		while (res->next()) {
			std::cout << std::setw(9) << res->getInt("id") << " | "
				<< std::setw(9) << res->getString("age_upon_outcome") << " | "
				<< std::setw(9) << res->getString("animal_id") << " | "
				<< std::setw(6) << res->getString("animal_type") << " | "
				<< std::setw(51) << res->getString("breed") << " | "
				<< std::setw(19) << res->getString("color") << " | "
				<< std::setw(14) << res->getString("date_of_birth") << " | "
				<< std::setw(16) << res->getString("datetime") << " | "
				<< std::setw(19) << res->getString("monthyear") << " | "
				<< std::setw(15) << res->getString("name") << " | "
				<< std::setw(15) << res->getString("outcome_subtype") << " | "
				<< std::setw(15) << res->getString("outcome_type") << " | "
				<< std::setw(15) << res->getString("sex_upon_outcome") << " | "
				<< std::setw(13) << res->getDouble("location_lat") << " | "
				<< std::setw(14) << res->getDouble("location_long") << " | "
				<< std::setw(12) << res->getDouble("age_upon_outcome_in_weeks") << " | "
				<< std::endl << std::string(244, '-') << std::endl;
		}
	}
	// If error has occurred
	catch (sql::SQLException& e) {
		std::cerr << "A error has occurred trying to read page: " << e.what() << std::endl;
	}
}

void AnimalShelterDatabase::table_display()
{
	std::cout << std::endl
		<< std::string(244, '-') << std::endl
		<< std::left << std::setw(9) << "Unique ID" << " | "
		<< std::setw(9) << "Age" << " | "
		<< std::setw(9) << "Animal ID" << " | "
		<< std::setw(6) << "Type" << " | "
		<< std::setw(51) << "Breed" << " | "
		<< std::setw(19) << "Color" << " | "
		<< std::setw(14) << "Date Of Birth" << " | "
		<< std::setw(16) << "Date Time" << " | "
		<< std::setw(19) << "Month Year" << " | "
		<< std::setw(15) << "Name" << " | "
		<< std::setw(15) << "Outcome Subtype" << " | "
		<< std::setw(15) << "Outcome Type" << " | "
		<< std::setw(15) << "Sex Upon Outcome" << " | "
		<< std::setw(13) << "Location Lat" << " | "
		<< std::setw(14) << "Location Long" << " | "
		<< std::setw(12) << "Age In Weeks" << " | "
		<< std::endl << std::string(244, '-') << std::endl;
}

// Calculates total number of pages for pagination
int AnimalShelterDatabase::get_total_pages(const std::string& table_name, int records_per_page)
{
	// Gets total number of pages to user
	try {
		std::unique_ptr<sql::Statement> stmt(connection->createStatement());
		std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT COUNT(*) as total FROM " + table_name));

		// Math made to get page count
		if (res->next()) {
			int total_records = res->getInt("total");
			return (total_records + records_per_page - 1) / records_per_page;
		}
	}
	// If error has occurred
	catch (sql::SQLException& e) {
		std::cerr << "A error has occurred getting total amount of pages: " << e.what() << std::endl;
	}
	return 0;
}

// Filters list of animals to show qualifies dogs for specific rescue training type
void AnimalShelterDatabase::get_filtered_record(const std::string& table_name, const std::vector<std::string>& breed_names, const std::string& sex_upon_outcome, double min_age_in_weeks, double max_age_in_weeks)
{
	// Showcases filtered animals
	try {
		std::unique_ptr<sql::Statement> stmt(connection->createStatement());
		// Creates variable value condition if vector is holding more than one breed of animal
		std::string value_condition = "";
		for (size_t i = 0; i < breed_names.size(); i++) {
			if (i > 0) {
				value_condition += " OR ";
			}
			value_condition += "breed = '" + breed_names[i] + "'";
		}
		std::string query = "SELECT * FROM " + table_name + " WHERE (" + value_condition + ")" + " AND sex_upon_outcome = '" + sex_upon_outcome + "' AND age_upon_outcome_in_weeks >= " + std::to_string(min_age_in_weeks) + " AND age_upon_outcome_in_weeks <= " + std::to_string(max_age_in_weeks);
		std::unique_ptr<sql::ResultSet> res(stmt->executeQuery(query));
		// Variable count to see how many records are found
		int count = 0;

		// Information regarding animal
		while (res->next()) {
			std::cout << std::setw(9) << res->getInt("id") << " | "
				<< std::setw(9) << res->getString("age_upon_outcome") << " | "
				<< std::setw(9) << res->getString("animal_id") << " | "
				<< std::setw(6) << res->getString("animal_type") << " | "
				<< std::setw(51) << res->getString("breed") << " | "
				<< std::setw(19) << res->getString("color") << " | "
				<< std::setw(14) << res->getString("date_of_birth") << " | "
				<< std::setw(16) << res->getString("datetime") << " | "
				<< std::setw(19) << res->getString("monthyear") << " | "
				<< std::setw(15) << res->getString("name") << " | "
				<< std::setw(15) << res->getString("outcome_subtype") << " | "
				<< std::setw(15) << res->getString("outcome_type") << " | "
				<< std::setw(15) << res->getString("sex_upon_outcome") << " | "
				<< std::setw(13) << res->getDouble("location_lat") << " | "
				<< std::setw(14) << res->getDouble("location_long") << " | "
				<< std::setw(12) << res->getDouble("age_upon_outcome_in_weeks") << " | "
				<< std::endl << std::string(244, '-') << std::endl;
			count++;
		}

		// Shows no matching records
		if (count == 0) {
			std::cout << "No matching records could be found." << std::endl;
		}
		else {
			std::cout << std::endl << "Found " << count << " requested records" << std::endl;
		}
	}
	// If error has occurred
	catch (sql::SQLException& e) {
		std::cerr << "A error has occurred while trying to filter animals: " << e.what() << std::endl;
	}
}

// Searches through table using an animal's unique ID to showcase all information related to that animal
void AnimalShelterDatabase::search_record(const std::string& table_name, const std::string& animal_id)
{
	// Lets user search for animal through ID
	try {
		std::unique_ptr<sql::PreparedStatement> pstmt(connection->prepareStatement("SELECT * FROM " + table_name + " WHERE animal_id = ?"));
		pstmt->setString(1, animal_id);
		std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

		// Showcases information about animal
		if (res->next()) {
			std::cout << std::endl << "Found Animal ID: " << res->getString("animal_id");
			std::cout << std::endl << "Age upon outcome: " << res->getString("age_upon_outcome");
			std::cout << std::endl << "Age upon outcome in weeks: " << res->getDouble("age_upon_outcome_in_weeks");
			std::cout << std::endl << "Animal type: " << res->getString("animal_type");
			std::cout << std::endl << "Animal breed: " << res->getString("breed");
			std::cout << std::endl << "Color: " << res->getString("color");
			std::cout << std::endl << "Date of birth: " << res->getString("date_of_birth");
			std::cout << std::endl << "Date time: " << res->getString("datetime");
			std::cout << std::endl << "Location lat: " << res->getDouble("location_lat");
			std::cout << std::endl << "Location long: " << res->getDouble("location_long");
			std::cout << std::endl << "Month year: " << res->getString("monthyear");
			std::cout << std::endl << "Name: " << res->getString("name");
			std::cout << std::endl << "Outcome subtype: " << res->getString("outcome_subtype");
			std::cout << std::endl << "Outcome type: " << res->getString("outcome_type");
			std::cout << std::endl << "Sex upon outcome: " << res->getString("sex_upon_outcome");
			std::cout << std::endl;
		}
		else {
			std::cout << "No records were found matching that ID: " << animal_id << std::endl;
		}
	}
	// If error has occurred
	catch (sql::SQLException& e) {
		std::cerr << "A error has occurred while trying to search: " << e.what() << std::endl;
	}
}

// Creates an interactive map through animal locations
void AnimalShelterDatabase::createMap(const std::string& table_name)
{
	std::ofstream mapFile("animal_map.html");

	// If map cannot be open
	if (!mapFile.is_open()) {
		std::cerr << "Could not create map file." << std::endl;
		return;
	}

	// Creates map through retrieve animal coordinates
	try {
		std::unique_ptr<sql::PreparedStatement> pstmt(connection->prepareStatement("SELECT animal_id, name, breed, animal_type, "
			"sex_upon_outcome, age_upon_outcome, "
			"location_lat, location_long "
			"FROM " + table_name +
			" WHERE location_lat BETWEEN -90 AND 90 "
			"AND location_long BETWEEN -180 AND 180 "
			"AND location_lat IS NOT NULL "
			"AND location_long IS NOT NULL"));
		std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

		// Write HTML header and initialize Leaflet
		mapFile << R"HTML(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport"
          content="width=device-width, initial-scale=1.0">

    <title>Gracioso Salvare Animal Map</title>

    <link rel="stylesheet"
          href="https://unpkg.com/leaflet@1.9.4/dist/leaflet.css"/>

    <script src="https://unpkg.com/leaflet@1.9.4/dist/leaflet.js"></script>

    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 20px;
        }

        #map {
            height: 700px;
            width: 100%;
        }
    </style>
</head>

<body>

<h2>Gracioso Salvare Animal Map</h2>

<div id="map"></div>

<script>
	// Map center
	var map = L.map('map').setView([30.2672, -97.7431], 10);

	// Load tiles
	L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
        attribution: '&copy; OpenStreetMap contributors'
    }).addTo(map);

	// Track marker locations for map to zoom in
	var animalMarkers = [];
)HTML";

		int markerCount = 0;

		// Generate one map marker for each record
		while (res->next()) {
			double latitude = res->getDouble("location_lat");
			double longitude = res->getDouble("location_long");

			std::string animalId =
				htmlEscape(res->getString("animal_id"));

			std::string name =
				htmlEscape(res->getString("name"));

			std::string breed =
				htmlEscape(res->getString("breed"));

			std::string animalType =
				htmlEscape(res->getString("animal_type"));

			std::string sex =
				htmlEscape(res->getString("sex_upon_outcome"));

			std::string age =
				htmlEscape(res->getString("age_upon_outcome"));

			std::ostringstream popup;
			popup << "<b>Animal ID:</b> " << animalId
				<< "<br><b>Name:</b> " << name
				<< "<br><b>Breed:</b> " << breed
				<< "<br><b>Type:</b> " << animalType
				<< "<br><b>Sex:</b> " << sex
				<< "<br><b>Age:</b> " << age;

			// Writes marker and popup to HTML file
			mapFile << std::setprecision(12)
				<< "var marker = L.marker(["
				<< latitude << ", " << longitude
				<< "]).addTo(map)"
				<< ".bindPopup("
				<< jsString(popup.str())
				<< ");\n"
				<< "animalMarkers.push(marker);\n";

			++markerCount;
		}
		mapFile << R"HTML(
    if (animalMarkers.length > 0) {
        var markerGroup = L.featureGroup(animalMarkers);
        map.fitBounds(markerGroup.getBounds().pad(0.1));
    }
</script>
</body>
</html>
)HTML";
		mapFile.close();

		// If error has occurred while trying to write HTML
		if (!mapFile) {
			std::cerr << "Error occurred trying to write animal_map.html: " << std::endl;
			return;
		}
		// If map has been made
		std::cout << "Map created successfully: animal_map.html"
			<< std::endl;
		std::cout << "Animal markers added: "
			<< markerCount << std::endl;

		// Opens map in default browser
#ifdef _WIN32
		std::system("start \"\" \"animal_map.html\"");
#elif __APPLE__
		std::system("open \"animal_map.html\"");
#else
		std::system("xdg-open \"animal_map.html\"");
#endif
	}
	// If error has occurred
	catch (sql::SQLException& e) {
		mapFile.close();
		std::cerr << "A error has occurred trying to build map: " << e.what() << std::endl;
	}
}