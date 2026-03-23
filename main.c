//Main File / driver code

#include "header.h"
#include <stdio.h>
#include <stdlib.h>
// These are the ACTUAL variables. header.h just says they "extern"ally exist.
employee employee_list[MAX_EMPLOYEES];
team team_list[MAX_TEAMS];
int employee_count = 0;
int team_count = 0;

/**
 * @brief Clears the input buffer to prevent infinite loops on bad input.
 */
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// menu function
void print_menu() {
    printf("\n--- Work Schedule Management System ---\n");
    printf("1. Add Employee\n");
    printf("2. Create Team\n");
    printf("3. Show Top 5 Employees\n");
    printf("4. Check Team Achievement\n");
    printf("5. Generate Team Ranking\n");
    printf("6. Check Employee Reward\n");
    printf("7. Update or Delete Employee\n");
    printf("8. Delete Team\n");
    printf("9. Merge Teams\n");
    printf("10. Display All Team Info\n");
    printf("0. Exit\n");
    printf("Enter your choice: ");
}

int main(void){
    int choice = -1;
    int flag = 1; //while loop flag
    
    //Load files (employees.txt and teams.txt)
    load_data();

    
    while (flag == 1) {
        print_menu();
        
        // input menu choice & validate it
        scanf("%d", &choice);
        if (choice <0|| choice > 10) {
            printf("Invalid input. Please enter a number.\n");
            clear_input_buffer(); // clear wrong input
            choice = -1; // Set to a non-zero, non-menu value
            continue; // Skip the switch and re-prompt
        }
        
        // Valid integer was entered, so clear the trailing newline
        clear_input_buffer();

        
        switch (choice) {
            case 1: 
                add_employee(); 
                save_data();
                break;
            case 2: 
                create_team();
                save_data();
                break;
            case 3: 
                top_5_employees();
                save_data();
                break;
            case 4: 
                check_team_achievement(); 
                save_data();
                break;
            case 5: 
                generate_team_ranking(); 
                save_data();
                break;
            case 6: 
                check_employee_reward(); 
                save_data();
                break;
            case 7: 
                update_or_delete_employee(); 
                save_data();
                break;
            case 8: 
                delete_team(); 
                save_data();
                break;
            case 9: 
                merge_teams(); 
                save_data();
                break;
            case 10: 
                display_team_info();
                save_data();
                break;
            case 0:
                printf("exiting...\n");
                flag = 0; // Set flag to 0 to exit loop
                break;
            default: 
                printf("invalid choice, please try again .\n");
        }
    }
}