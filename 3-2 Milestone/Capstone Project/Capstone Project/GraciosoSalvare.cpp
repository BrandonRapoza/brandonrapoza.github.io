#include "AnimalShelter.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <limits>

// Menu display for main Gracioso Salvare screen
void main_menu() {
    std::cout << std::endl;
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    std::cout << "~                Welcome to Gracioso Salvare              ~" << std::endl;
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    std::cout << "~ 1. Show List                                            ~" << std::endl;
    std::cout << "~ 2. Filter by Water Rescue                               ~" << std::endl;
    std::cout << "~ 3. Filter by Mountain/Wilderness Rescue                 ~" << std::endl;
    std::cout << "~ 4. Filter by Disaster Rescue/Individual Tracking        ~" << std::endl;
    std::cout << "~ 5. Display Interactive Map                              ~" << std::endl;
    std::cout << "~ 6. Exit Program                                         ~" << std::endl;
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    std::cout << "~ Select your option: ";
}

// Function to show elements in the table
void table_labels() {
    std::cout << std::endl
        << std::string(243, '-') << std::endl
        << std::left << std::setw(9) << "Unique ID" << " | "
        << std::setw(9) << "Age" << " | "
        << std::setw(9) << "Animal ID" << " | "
        << std::setw(6) << "Type" << " | "
        << std::setw(51) << "Breed" << " | "
        << std::setw(18) << "Color" << " | "
        << std::setw(14) << "Date Of Birth" << " | "
        << std::setw(16) << "Date Time" << " | "
        //<< std::setw(19) << "Month Year" << " | "
        << std::setw(15) << "Name" << " | "
        << std::setw(15) << "Outcome Subtype" << " | "
        << std::setw(15) << "Outcome Type" << " | "
        << std::setw(15) << "Sex Upon Outcome" << " | "
        << std::setw(13) << "Location Lat" << " | "
        << std::setw(14) << "Location Long" << " | "
        << std::setw(12) << "Age In Weeks" << " | "
        << std::endl << std::string(243, '-') << std::endl;
}

// Function to display each element's value
void table_list(const std::vector<AnimalShelter>& animal_list, int i) {
    std::cout << std::left << std::setw(9) << animal_list[i].get_unique_id() << " | "
        << std::setw(9) << animal_list[i].get_age_upon_outcome() << " | "
        << std::setw(9) << animal_list[i].get_animal_id() << " | "
        << std::setw(6) << animal_list[i].get_animal_type() << " | "
        << std::setw(51) << animal_list[i].get_breed() << " | "
        << std::setw(18) << animal_list[i].get_color() << " | "
        << std::setw(14) << animal_list[i].get_date_of_birth() << " | "
        << std::setw(16) << animal_list[i].get_date_time() << " | "
        //<< std::setw(19) << animal_list[i].get_month_year() << " | "
        << std::setw(15) << animal_list[i].get_name() << " | "
        << std::setw(15) << animal_list[i].get_outcome_subtype() << " | "
        << std::setw(15) << animal_list[i].get_outcome_type() << " | "
        << std::setw(16) << animal_list[i].get_sex_upon_outcome() << " | "
        << std::setw(13) << animal_list[i].get_location_lat() << " | "
        << std::setw(14) << animal_list[i].get_location_long() << " | "
        << std::setw(12) << animal_list[i].get_age_upon_outcome_in_weeks() << " | "
        << std::endl << std::string(243, '-') << std::endl;
}

// Function to create interactive map
void createMap(const std::vector<AnimalShelter>& animal_list) {
    std::ofstream mapFile("animal_map.html");

    if (!mapFile.is_open()) {
        std::cout << "Could not create map file." << std::endl;
        return;
    }

    // HTML header and Leaflet setup
    mapFile << R"(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>Gracioso Salvare Animal Map</title>

    <link rel="stylesheet"
          href="https://unpkg.com/leaflet@1.9.4/dist/leaflet.css"/>

    <script src="https://unpkg.com/leaflet@1.9.4/dist/leaflet.js"></script>

    <style>
        #map{
            height: 700px;
            width: 100%;
       }
    </style>
</head>

<body>

<h2>Gracioso Salvare Animal Map</h2>

<div id="map"></div>

<script>
   
    // Centers map around shelter's general area
    var map = L.map('map').setView([30.2672, -97.7431], 10);

    // Add OpenStreetMap tiles
    L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
        attribution: '&copy; OpenStreetMap contributors'
    }).addTo(map);
)";

    // Add marker for every animal
    for (const AnimalShelter& animal : animal_list) {
        mapFile << "L.marker(["
            << animal.get_location_lat()
            << ", "
            << animal.get_location_long()
            << "])"
            << ".addTo(map)"
            << ".bindPopup(\""
            << "<b>Animal ID:</b> "
            << animal.get_animal_id()
            << "<br><b>Name:</b> "
            << animal.get_name()
            << "<br><b>Breed:</b> "
            << animal.get_breed()
            << "<br><b>Type:</b> "
            << animal.get_animal_type()
            << "<br><b>Sex:</b> "
            << animal.get_sex_upon_outcome()
            << "<br><b>Age:</b> "
            << animal.get_age_upon_outcome()
            << "\");"
            << std::endl;
    }

    // Finishes HTML
    mapFile << R"(
</script>

</body>
</html>
)";

    mapFile.close();

    std::cout << "Map created successfully: animal_map.html" << std::endl;

    // Opens map in default browser
#ifdef _WIN32
    system("start animal_map.html");
#elif __APPLE__
    system("open animal_map.html");
#else
    system("xdg-open animal_map.html");
#endif
}

// Function used to input validate an integer and will ignore input if a non-integer input occurs
int integer_verification(int& number) {
    try {
        std::cin >> number;
        if (std::cin.fail()) {
            // Throw exception for invalid non-integer input
            throw std::runtime_error("Invalid Input. Please input numbers only:");
        }
    }
    catch (std::runtime_error&) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    return number;
}

int main()
{
    // Input stream for reading
    std::ifstream file; 

    // Opens file
    file.open("aac_shelter_outcomes.csv");

    // If file doesn't open
    if (!file.is_open()) {
        std::cout << "Can't open file." << std::endl;
    }

    // Creates the vector animal_list
    std::vector<AnimalShelter> animal_list;

    // String for reading lines
    std::string line;

    // Skips first line of file
    std::getline(file, line);

    // Reads each line and stores the information
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string unique_id_str, age_upon_outcome, animal_id, animal_type, breed,
            color, date_of_birth, date_time, month_year, name, outcome_subtype,
            outcome_type, sex_upon_outcome, location_lat_str, location_long_str,
            age_upon_outcome_in_weeks_str;

        std::getline(ss, unique_id_str, ',');
        std::getline(ss, age_upon_outcome, ',');
        std::getline(ss, animal_id, ',');
        std::getline(ss, animal_type, ',');
        std::getline(ss, breed, ',');
        std::getline(ss, color, ',');
        std::getline(ss, date_of_birth, ',');
        std::getline(ss, date_time, ',');
        std::getline(ss, month_year, ',');
        std::getline(ss, name, ',');
        std::getline(ss, outcome_subtype, ',');
        std::getline(ss, outcome_type, ',');
        std::getline(ss, sex_upon_outcome, ',');
        std::getline(ss, location_lat_str, ',');
        std::getline(ss, location_long_str, ',');
        std::getline(ss, age_upon_outcome_in_weeks_str, ',');

        // Creates new variables while performing string conversions for appropriate variable type
        unsigned int unique_id = std::stoul(unique_id_str);
        double location_lat = std::stod(location_lat_str);
        double location_long = std::stod(location_long_str);
        double age_upon_outcome_in_weeks = std::stod(age_upon_outcome_in_weeks_str);

        // Utilizes the function emplace_back to insert each object through the constructor
        try {
            animal_list.emplace_back(
                unique_id,
                age_upon_outcome,
                animal_id,
                animal_type,
                breed,
                color,
                date_of_birth,
                date_time,
                month_year,
                name,
                outcome_subtype,
                outcome_type,
                sex_upon_outcome,
                location_lat,
                location_long,
                age_upon_outcome_in_weeks
            );
        }
        // Catches bad line read
        catch (const std::exception&) {
            std::cerr << "Bad row was skipped: " << line << std::endl;
        }
    }

    // Closes the file after it's finished reading
    file.close();

    // Controls when the program is done running
    bool program_exit = false;
    // Option selection
    int option_num = 0;
    // Gets first 10 elements in table
    int first_ten = std::min(10, (int)animal_list.size());
    // Variable to limit maximum amount of pages to 10
    int max_pages = (int)animal_list.size() / 10;
    // Variable to make initial page number 1
    int page_number = 1;
    // Temporal number ensuring page number displayed is still correct even if an incorrect input is made
    int page_temp = page_number;


    // While loop for menu
    while (program_exit == false) {
        main_menu();
        integer_verification(option_num);

        // Control flow statement to choose which code block to follow based on user input
        switch (option_num) {
        // Displays list
        case 1:
            table_labels();
            // Displays the table list
            for (int i = 0; i < first_ten; i++) {
                table_list(animal_list, i);
            }

            // Logic to make page jumps
            while (page_number != 0) {
                std::cout << "You're on page " << page_temp << " out of " << max_pages << std::endl << std::endl;
                std::cout << "Enter any number between 1 and " << max_pages <<
                    " to see other records available. Enter 0 to go back to the main menu" << std::endl;
                std::cout << "Jump to page: ";

                // Validates verification of page number
                integer_verification(page_number);

                // If else in case of invalid input
                if (page_number > max_pages || page_number < 0) {
                    std::cout << "Invalid. Page does not exist." << std::endl;
                }
                else if (page_number == 0) {
                    std::cout << "Exiting to main menu." << std::endl;
                }
                else {
                    table_labels();
                    for (int i = (first_ten * page_number) - 10; i < first_ten * page_number; i++) {
                        table_list(animal_list, i);
                    }
                    page_temp = page_number;
                }
            }
            // Makes sure variables are back to default values for when the user inputs option 1 again
            page_number = 1;
            page_temp = page_number;
            break;

        // Water rescue filter
        case 2:
            // Creates table only listing rescue dogs filtered towards water rescue
            table_labels();
            for (int i = 0; i < animal_list.size(); i++) {
                if ((animal_list[i].get_breed() == "Labrador Retriever Mix" ||
                    animal_list[i].get_breed() == "Chesapeake Bay Retriever" ||
                    animal_list[i].get_breed() == "Newfoundland") &&
                    animal_list[i].get_sex_upon_outcome() == "Intact Female" &&
                    (animal_list[i].get_age_upon_outcome_in_weeks() >= 26 &&
                    animal_list[i].get_age_upon_outcome_in_weeks() <= 156))
                {
                    table_list(animal_list, i);
                }
            }
            break;

        // Moutain or wilderness rescue filter
        case 3:
            table_labels();
            for (int i = 0; i < animal_list.size(); i++) {
                if ((animal_list[i].get_breed() == "German Shepherd" ||
                    animal_list[i].get_breed() == "Alaskan Malamute" ||
                    animal_list[i].get_breed() == "Old English Sheepdog" ||
                    animal_list[i].get_breed() == "Siberian Husky" ||
                    animal_list[i].get_breed() == "Rottweiler") &&
                    animal_list[i].get_sex_upon_outcome() == "Intact Male" &&
                    (animal_list[i].get_age_upon_outcome_in_weeks() >= 26 &&
                    animal_list[i].get_age_upon_outcome_in_weeks() <= 156))
                {
                    table_list(animal_list, i);
                }
            }
            break;

        // Disaster rescue or individual tracking filter
        case 4:
            table_labels();
            for (int i = 0; i < animal_list.size(); i++) {
                if ((animal_list[i].get_breed() == "Doberman Pinscher" ||
                    animal_list[i].get_breed() == "German Shepherd" ||
                    animal_list[i].get_breed() == "Golden Retriever" ||
                    animal_list[i].get_breed() == "Bloodhound" ||
                    animal_list[i].get_breed() == "Rottweiler") &&
                    animal_list[i].get_sex_upon_outcome() == "Intact Male" &&
                    (animal_list[i].get_age_upon_outcome_in_weeks() >= 20 &&
                    animal_list[i].get_age_upon_outcome_in_weeks() <= 300))
                {
                    table_list(animal_list, i);
                }
            }
            break;

        // Creates interactive map in browser
        case 5:
            createMap(animal_list);
            break;

        // Exit program
        case 6:
            program_exit = true;
            break;
        // If given invalid input
        default:
            std::cout << "Invalid option was chosen, please try again." << std::endl;
            break;
        }
    }

    return 0;
}


