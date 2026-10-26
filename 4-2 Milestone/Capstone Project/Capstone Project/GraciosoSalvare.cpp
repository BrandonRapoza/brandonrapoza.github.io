#include "AnimalShelter.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>
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
    std::cout << "~ 5. Sort Through List                                    ~" << std::endl;
    std::cout << "~ 6. Search Animal by ID                                  ~" << std::endl;
    std::cout << "~ 7. Display Interactive Map                              ~" << std::endl;
    std::cout << "~ 8. Exit Program                                         ~" << std::endl;
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

// Function used to find animal specific ID and display that animal
void find_animal_id(const std::vector<AnimalShelter>& animal_list, const std::string& target) {
    // Object iterator is used with lower bound function to allow for binary search
    auto iterator = std::lower_bound(animal_list.begin(), animal_list.end(), target, [](const AnimalShelter& AnSh, const std::string& animal_id) {
        return AnSh.get_animal_id() < animal_id;
        });

    // If the target has been found before reaching the end of them list showcase animal else input error message
    if (iterator != animal_list.end() && iterator->get_animal_id() == target) {
        std::cout << std::endl << "Animal ID has been found, showcasing animal: " << iterator->get_animal_id();
        std::cout << std::endl << "Age: " << iterator->get_age_upon_outcome();
        std::cout << std::endl << "Type: " << iterator->get_animal_type();
        std::cout << std::endl << "Breed: " << iterator->get_breed();
        std::cout << std::endl << "Color: " << iterator->get_color();
        std::cout << std::endl << "Date of birth: " << iterator->get_date_of_birth();
        std::cout << std::endl << "Date time: " << iterator->get_date_time();
        //std::cout << std::endl << "Month year: " << iterator->get_month_year();
        std::cout << std::endl << "Name: " << iterator->get_name();
        std::cout << std::endl << "Outcome subtype: " << iterator->get_outcome_subtype();
        std::cout << std::endl << "Outcome type: " << iterator->get_outcome_type();
        std::cout << std::endl << "Sex upon outcome: " << iterator->get_sex_upon_outcome();
        std::cout << std::endl << "Location lat: " << iterator->get_location_lat();
        std::cout << std::endl << "Location long: " << iterator->get_location_long();
        std::cout << std::endl << "Age in weeks: " << iterator->get_age_upon_outcome_in_weeks();
        std::cout << std::endl;
    }
    else {
        std::cout << std::endl << "Animal ID: " << target << " is not in database." << std::endl;
    }
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
    // variable for sort selection set to -1 so no sorting selection has been made
    int sort_selection = -1;
    // Variable for the sorting type by choice set as 0 for a switch-case
    int sort_type_choice = 0;
    // Condition for sorting ascension set as true to display list by ascending order
    bool default_asc_sort = true;
    // Variable to get animal ID input
    std::string animal_id_input;


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

        // Sorts options and resets
        case 5:
            // Options displayed
            std::cout << std::endl;
            std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
            std::cout << "~ Choose one of these sort options:                       ~" << std::endl;
            std::cout << "~ 1. Type                                                 ~" << std::endl;
            std::cout << "~ 2. Breed                                                ~" << std::endl;
            std::cout << "~ 3. Color                                                ~" << std::endl;
            std::cout << "~ 4. Name                                                 ~" << std::endl;
            std::cout << "~ 5. Sex Upon Outcome                                     ~" << std::endl;
            std::cout << "~ 0. Resets list                                          ~" << std::endl;
            std::cout << "~ Enter any other input to go back to main menu           ~" << std::endl;
            std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
            std::cout << std::endl << "Select your option: ";
            integer_verification(sort_selection);

            // Input is between 1 and 5
            if (sort_selection >= 1 && sort_selection <= 5) {
                // When valid selection is made ask the user for order by ascension or descension
                std::cout << std::endl << "Choose sort order: 1 for ascending or 2 for descending:";
                std::cout << std::endl << "Select your option: ";
                integer_verification(sort_type_choice);

                // If 1 is selected then change order to ascending else if 2 is selected change to descending
                if (sort_type_choice == 1) {
                    default_asc_sort = true;
                    std::cout << std::endl << "Sorting will begin." << std::endl;
                }
                else if (sort_type_choice == 2) {
                    default_asc_sort = false;
                    std::cout << std::endl << "Sorting will begin." << std::endl;
                }
                else {
                    // variable reset
                    sort_selection = -1;
                }
            }

            // Lambda expression used to make sorting as quick and effective as possible
            // Sort selection 1 is chosen
            if (sort_selection == 1) {
                if (default_asc_sort == true) {
                    std::sort(animal_list.begin(), animal_list.end(), [](const AnimalShelter& a, const AnimalShelter& b) {
                        return a.get_animal_type() < b.get_animal_type();
                        });
                }
                else if (default_asc_sort == false) {
                    std::sort(animal_list.begin(), animal_list.end(), [](const AnimalShelter& a, const AnimalShelter& b) {
                        return a.get_animal_type() > b.get_animal_type();
                        });
                }
            }
            // Sort selection 2 is chosen
            else if (sort_selection == 2) {
                if (default_asc_sort == true) {
                    std::sort(animal_list.begin(), animal_list.end(), [](const AnimalShelter& a, const AnimalShelter& b) {
                        return a.get_breed() < b.get_breed();
                        });
                }
                else if (default_asc_sort == false) {
                    std::sort(animal_list.begin(), animal_list.end(), [](const AnimalShelter& a, const AnimalShelter& b) {
                        return a.get_breed() > b.get_breed();
                        });
                }
            }
            // Sort selection 3 is chosen
            else if (sort_selection == 3) {
                if (default_asc_sort == true) {
                    std::sort(animal_list.begin(), animal_list.end(), [](const AnimalShelter& a, const AnimalShelter& b) {
                        return a.get_color() < b.get_color();
                        });
                }
                else if (default_asc_sort == false) {
                    std::sort(animal_list.begin(), animal_list.end(), [](const AnimalShelter& a, const AnimalShelter& b) {
                        return a.get_color() > b.get_color();
                        });
                }
            }
            // Sort selection 4 is chosen
            else if (sort_selection == 4) {
                if (default_asc_sort == true) {
                    std::sort(animal_list.begin(), animal_list.end(), [](const AnimalShelter& a, const AnimalShelter& b) {
                        return a.get_name() < b.get_name();
                        });
                }
                else if (default_asc_sort == false) {
                    std::sort(animal_list.begin(), animal_list.end(), [](const AnimalShelter& a, const AnimalShelter& b) {
                        return a.get_name() > b.get_name();
                        });
                }
            }
            // Sort selection 5 is chosen
            else if (sort_selection == 5) {
                if (default_asc_sort == true) {
                    std::sort(animal_list.begin(), animal_list.end(), [](const AnimalShelter& a, const AnimalShelter& b) {
                        return a.get_sex_upon_outcome() < b.get_sex_upon_outcome();
                        });
                }
                else if (default_asc_sort == false) {
                    std::sort(animal_list.begin(), animal_list.end(), [](const AnimalShelter& a, const AnimalShelter& b) {
                        return a.get_sex_upon_outcome() > b.get_sex_upon_outcome();
                        });
                }
            }
            // Sort selection 0 is chosen
            else if (sort_selection == 0) {
                std::cout << std::endl << "Resetting will begin." << std::endl;
                std::sort(animal_list.begin(), animal_list.end(), [](const AnimalShelter& a, const AnimalShelter& b) {
                    return a.get_unique_id() < b.get_unique_id();
                    });
                std::cout << std::endl << "List has been reset to default." << std::endl;
            }
            // Sort selection outside of 0-5 is chosen
            else {
                std::cout << std::endl << "Sort has been canceled. Now returning to main menu." << std::endl;
            }
            // When sorting has been complete
            if (sort_selection >= 1 && sort_selection <= 5) {
                std::cout << std::endl << "Sorting has been finished." << std::endl;
            }

            break;

        // Searches for specific animal ID
        case 6:
            std::cout << std::endl << "Give specific ID to search for animal. ";
            std::cout << std::endl << "Animal ID: ";
            std::cin >> animal_id_input;

            // Sorts through list by ID in ascending order
            std::sort(animal_list.begin(), animal_list.end(), [](const AnimalShelter& a, const AnimalShelter& b) {
                return a.get_animal_id() < b.get_animal_id();
                });
            // Displays result to user
            find_animal_id(animal_list, animal_id_input);
            // Clears the input buffer when detecting invalid input
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            break;

        // Creates interactive map in browser
        case 7:
            createMap(animal_list);
            break;

        // Exit program
        case 8:
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


