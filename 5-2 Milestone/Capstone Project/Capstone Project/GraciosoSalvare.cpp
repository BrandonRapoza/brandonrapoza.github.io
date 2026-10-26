#include "AnimalShelterDatabase.h"
#include <iostream>
#include <fstream>
#include <conio.h>
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <stdexcept>
#include <cctype>

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
    std::cout << "~ 5. Search Animal by ID                                  ~" << std::endl;
    std::cout << "~ 6. Update list by ID                                    ~" << std::endl;
    std::cout << "~ 7. Delete Animal by ID                                  ~" << std::endl;
    std::cout << "~ 8. Display Interactive Map                              ~" << std::endl;
    std::cout << "~ 9. Exit Program                                         ~" << std::endl;
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    std::cout << "~ Select your option: ";
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
    std::cout << "Gracioso Salvare Program." << std::endl;
    std::cout << "-------------------------" << std::endl;

    // variables for username and password needed to access database along with key integration
    std::string username;
    std::string password;
    char ch;
    std::cout << "Enter username." << std::endl;
    std::cout << "Username: ";
    getline(std::cin, username);
    std::cout << "Enter password." << std::endl;
    std::cout << "Password: ";
    while ((ch = _getch()) != '\r') {
        if (ch == '\b') {
            if (!password.empty()) {
                password.pop_back();
                std::cout << "\b \b";
            }
        }
        else {
            password += ch;
            std::cout << '*';
        }
    }

    // Connects to database
    AnimalShelterDatabase db("tcp://127.0.0.1:3306", username, password, "animal_shelter");

    // Create and import all data
    db.create_table();
    db.import_CSV("aac_shelter_outcomes.csv", "animal_shelter");

    // Conditional to exit loop
    bool program_exit = false;
    // Option selection.
    int option_num = 0;
    // Variable for page number starting at 1
    int page_number = 1;
    int records_per_page = 10;
    // Get input from user to select what column to sort
    int sort_selection;
    // Get total pages from animal shelter based on all entries in database
    int total_pages = db.get_total_pages("animal_shelter", records_per_page);
    // Shows options within display list
    char nav_choice;
    // Gets input from user to move to a different page
    int page_input;
    // Sort bool check for ascending or descending order
    bool ascending = true;
    // Tells if sorting option is on/off
    std::string current_sort_column = "id";
    // Variable holding vector of strings to use in filtered list
    std::vector<std::string> breed_names;
    // ID input used to update or delete records
    int id_input;
    // Gets column and information for updating a record
    std::string column_input, entry;
    // Asks for user input to make ID search
    std::string search_animal_id = "";

    // Menu while loop
    while (program_exit == false) {
        main_menu();
        integer_verification(option_num);

        switch (option_num) {
            // Displays list
        case 1:
            if (total_pages == 0) {
                std::cout << "No records exist. Exiting the program." << std::endl;
                break;
            }
            // Show menu options for current page
            do {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                db.table_display();
                db.display_page("animal_shelter", page_number, records_per_page, current_sort_column, ascending);
                std::cout << std::endl << "You are on page " << page_number << " of " << total_pages << std::endl << std::endl;
                std::cout << "[N]ext" << std::endl;
                std::cout << "[P]revious" << std::endl;
                std::cout << "[G]o to page..." << std::endl;
                std::cout << "[S]ort by..." << std::endl;
                std::cout << "[R]everse order" << std::endl;
                std::cout << "[M]ain menu" << std::endl;
                std::cout << "Enter the letter corresponding to the option you' want to select're choosing: ";
                std::cin >> nav_choice;

                switch (tolower(nav_choice)) {
                // Next page
                case 'n':
                    if (page_number < total_pages) {
                        page_number++;
                    }
                    else {
                        std::cout << std::endl << "Last page has been reached." << std::endl;
                    }
                    break;
                // Previous page
                case 'p':
                    if (page_number > 1) {
                        page_number--;
                    }
                    else {
                        std::cout << std::endl << "First page has been reached." << std::endl;
                    }
                    break;
                // Go to specific page within range
                case 'g':
                    std::cout << "Enter page number between 1-" << total_pages << ": ";
                    integer_verification(page_input);
                    if (page_input >= 1 && page_input <= total_pages) {
                        page_number = page_input;
                    }
                    else {
                        std::cout << std::endl << "This page is invalid." << std::endl;
                    }
                    break;
                // Sort table by given options
                case 's':
                    std::cout << "1. Animal Type" << std::endl;
                    std::cout << "2. Breed" << std::endl;
                    std::cout << "3. Color" << std::endl;
                    std::cout << "4. Name" << std::endl;
                    std::cout << "5. Sex Upon Outcome" << std::endl;
                    std::cout << "#. Id" << std::endl;
                    std::cout << "Enter the number to sort: ";
                    integer_verification(sort_selection);
                    // Switch method to change value to corresponding column
                    switch (sort_selection) {
                    case 1:
                        current_sort_column = "animal_type";
                        break;
                    case 2:
                        current_sort_column = "breed";
                        break;
                    case 3:
                        current_sort_column = "color";
                        break;
                    case 4:
                        current_sort_column = "name";
                        break;
                    case 5:
                        current_sort_column = "sex_upon_outcome";
                        break;
                    default:
                        current_sort_column = "id";
                        break;
                    }
                    // Resets value
                    ascending = true;
                    page_number = 1;
                    std::cout << "Now sorting by " << current_sort_column << std::endl;
                    break;
                // Changes list to ascending or descending order
                case 'r':
                    ascending = !ascending;
                    page_number = 1;
                    std::cout << "Order: "
                        << (ascending ? "ascending" : "descending")
                        << std::endl;
                    break;
                // Returns to main menu
                case 'm':
                    std::cout << std::endl << "Returning to main menu." << std::endl;
                    break;
                default:
                    if (std::cin.failbit) {
                        std::cout << std::endl << "Input is invalid." << std::endl;
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    }
                    break;
                }
            } while (tolower(nav_choice) != 'm');
            break;

        // Filter by water rescue
        case 2:
            std::cout << std::endl << " Filtering by water rescue." << std::endl;
            breed_names = { "Labrador Retriever Mix", "Chesapeake Bay Retriever", "Newfoundland" };
            db.table_display();
            db.get_filtered_record("animal_shelter", breed_names, "Intact Female", 26, 156);
            break;

        // Filter by Mountain or Wilderness Rescue
        case 3:
            std::cout << std::endl << " Filtering by mountain or wilderness rescue." << std::endl;
            breed_names = { "German Shepherd", "Alaskan Malamute", "Old English Sheepdog" , "Siberian Husky", "Rottweiler" };
            db.table_display();
            db.get_filtered_record("animal_shelter", breed_names, "Intact Male", 26, 156);
            break;

        // Filters by disaster rescue or individual tracking
        case 4:
            std::cout << std::endl << " Filtering by disaster rescue or individual tracking." << std::endl;
            breed_names = { "Doberman Pinscher", "German Shepherd", "Golden Retriever", "Bloodhound", "Rottweiler" };
            db.table_display();
            db.get_filtered_record("animal_shelter", breed_names, "Intact Male", 20, 300);
            break;

        // Sort by individual ID
        case 5:
            // Clearing buffer
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Enter the Animal ID to search: ";
            getline(std::cin, search_animal_id);
            db.search_record("animal_shelter", search_animal_id);
            break;

        // Update by individual ID
        case 6:
            std::cout << "Enter ID to update entry: ";
            integer_verification(id_input);
            std::cout << "Enter what you want to update (use underscore instead of space): ";
            std::cin >> column_input;
            std::cout << "Enter new entry to overwrite: ";
            // Clearing buffer
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            getline(std::cin, entry);
            db.update_record("animal_shelter", id_input, column_input, entry);
            break;

        // Delete by individual ID
        case 7:
            std::cout << "Enter ID to delete entry: ";
            integer_verification(id_input);
            db.delete_record("animal_shelter", id_input);
            break;

        // Create and open interactive map
        case 8:
            db.createMap("animal_shelter");
            break;

        // Exit program
        case 9:
            program_exit = true;
            break;

        // If option is invalid
        default:
            std::cout << "Input is invalid" << std::endl;
            break;
        }
    }
    return 0;
}


