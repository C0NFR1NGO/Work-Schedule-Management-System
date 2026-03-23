//Function File 

#include "header.h" // Include all definitions and globals

//====================================================================
    // HELPER FUNCTIONS
//====================================================================


// Validate employee id (Uses binary search)
int find_employee_by_id(int emp_id) {
    int low = 0, high = employee_count - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (employee_list[mid].employee_id == emp_id) {
            return mid;
        }
        if (employee_list[mid].employee_id < emp_id) {
            low = mid + 1;
        } 
        else {
            high = mid - 1; 
        }
    }
    return -1; // Employee ID not found 
}

// Find team index by ID using binary search
int find_team_by_id(int team_id) {
    int low = 0, high = team_count - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (team_list[mid].team_id == team_id) {
            return mid;
        }
        if (team_list[mid].team_id < team_id) {
            low = mid + 1;
        } 
        else {
            high = mid - 1;
        }
    }
    return -1; // Team ID not found
}

// Check if employee is already in any team
int is_employee_in_team(int emp_id) {
    for (int i = 0; i < team_count; i++) {
        for (int j = 0; j < team_list[i].member_count; j++) {
            if (team_list[i].member_ids[j] == emp_id) {
                return 1; // Employee found to be in some team
            }
        }
    }
    return 0; // Employee not in team
}

// Sum of hours worked by employee
int calculate_employee_total_hours(int emp_index) {
    if (emp_index < 0 || emp_index >= employee_count) {
        return 0;
    }
    int total = 0;
    for (int d = 0; d < DAYS_IN_WEEK; d++) {
        total += employee_list[emp_index].weekly_working_hours[d];
    }
    return total;
}

// Sum of hours worked by team
int calculate_team_total_hours(int team_index) {
    if (team_index < 0 || team_index >= team_count) {
        return 0;
    }
    int team_total = 0;
    team current_team = team_list[team_index];
    
    for (int i = 0; i < current_team.member_count; i++) {
        int emp_id = current_team.member_ids[i];
        int emp_index = find_employee_by_id(emp_id);
        if (emp_index != -1) {
            team_total += calculate_employee_total_hours(emp_index);
        }
    }
    return team_total;
}

// Comparison Fn for qsort (emp_performance)
int compare_emp_performance(const void *a, const void *b) {
    emp_performance *pa = (emp_performance *)a;
    emp_performance *pb = (emp_performance *)b;
    // Sort in descending order
    return pb->total_hours - pa->total_hours;
}

// Comparison Fn for qsort (integers)
int compare_integers(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

// It must be at the top level to be used by qsort.
int compare_team_ranks(const void *a, const void *b) {
    team_performance *ra = (team_performance *)a;
    team_performance *rb = (team_performance *)b;
    // Sort descending by hours
    return rb->total_hours - ra->total_hours;
}

// Helper function for merge_teams
void internal_delete_team_by_id(int team_id) {
    int team_index = find_team_by_id(team_id);
    
    if (team_index == -1) {
        printf("Internal Error: Could not find team %d to delete.\n", team_id);
        return;
    }

    for (int i = team_index; i < team_count - 1; i++) {
        team_list[i] = team_list[i + 1];
    }
    team_count--;
}


/* ====================================================================
   CORE FUNCTIONS
====================================================================
*/

// Function to add a new employee as per question
void add_employee() {
    if (employee_count >= MAX_EMPLOYEES) {
        printf("Error: Employee list is full. Cannot add more employees.\n");
        return;
    }

    employee new_emp;
    printf("Enter new Employee ID: ");
    scanf("%d", &new_emp.employee_id);

    // Find insertion position to maintain sorted order
    int i = 0;
    while (i < employee_count && employee_list[i].employee_id < new_emp.employee_id) {
        i++;
    }

    // Check for existing ID
    if (i < employee_count && employee_list[i].employee_id == new_emp.employee_id) {
        printf("Error: Employee ID %d already exists.\n", new_emp.employee_id);
        return;
    }

    // Right align elements
    for (int j = employee_count; j > i; j--) {
        employee_list[j] = employee_list[j - 1];
    }

    // Insert new employee
    employee_list[i] = new_emp; // Set ID
    printf("Enter Name: ");
    scanf(" %[^\n]", employee_list[i].name);
    printf("Enter Age: ");
    scanf("%d", &employee_list[i].age);
    printf("Enter Department: ");
    scanf(" %[^\n]", employee_list[i].department);
    
    printf("Enter Weekly Target Hours (max 50): ");
    scanf("%d", &employee_list[i].weekly_target_hours);
    if(employee_list[i].weekly_target_hours > 50) {
        printf("Warning: Target hours exceed 50, setting to 50.\n");
        employee_list[i].weekly_target_hours = 50;
    }

    printf("Enter hours worked for 7 days (Mon-Sun): ");
    for (int d = 0; d < DAYS_IN_WEEK; d++) {
        scanf("%d", &employee_list[i].weekly_working_hours[d]);
    }

    employee_count++;
    printf("Employee added successfully.\n");
}


// Function to create a new team while maintaining sorted order by team ID
void create_team() {
    if (team_count >= MAX_TEAMS) {
        printf("Error: Team list is full. Cannot create more teams.\n");
        return;
    }

    team new_team;
    new_team.member_count = 0;
    
    printf("Enter new Team ID: ");
    scanf("%d", &new_team.team_id);

    // Find insertion position to maintain sorted order
    int i = 0;
    while (i < team_count && team_list[i].team_id < new_team.team_id) {
        i++;
    }

    // Check for duplicate ID
    if (i < team_count && team_list[i].team_id == new_team.team_id) {
        printf("Error: Team ID %d already exists.\n", new_team.team_id);
        return;
    }

    printf("Enter Team Name: ");
    scanf(" %[^\n]", new_team.team_name);
    printf("Enter Weekly Team Target Hours: ");
    scanf("%d", &new_team.weekly_team_target);

    printf("How many members to add (max %d)? ", MAX_TEAM_MEMBERS);
    int num_to_add;
    scanf("%d", &num_to_add);
    
    if (num_to_add <= 0 || num_to_add > MAX_TEAM_MEMBERS) {
        printf("Error: Invalid number of members. Must be between 1 and %d.\n", MAX_TEAM_MEMBERS);
        printf("Team creation cancelled.\n");
        return; 
    }

    for (int m = 0; m < num_to_add; m++) {
        int emp_id;
        printf("Enter Employee ID for member %d: ", m + 1);
        scanf("%d", &emp_id);

        // Validate employee
        if (find_employee_by_id(emp_id) == -1) {
            printf("Error: Employee ID %d does not exist.\n", emp_id);
            m--; // Retry this member
            continue;
        }
        
        // Check if already in team
        if (is_employee_in_team(emp_id)) {
            printf("Error: Employee ID %d already belongs to another team.\n");
            m--; // Retry member
            continue;
        }

        // Add member
        new_team.member_ids[new_team.member_count] = emp_id;
        new_team.member_count++;
    }

    // Shift elements to right
    for (int j = team_count; j > i; j--) {
        team_list[j] = team_list[j - 1];
    }

    // Insert new team
    team_list[i] = new_team;
    team_count++;
    printf("Team created successfully.\n");
}

// Case 3: Display Top 5 Employees
void top_5_employees() {
    emp_performance qualified_employees[MAX_EMPLOYEES];
    int qualified_count = 0;

    printf("\n--- Top 5 Employees ---\n");

    // 1. Find all employees who met target
    for (int i = 0; i < employee_count; i++) {
        int total_hours = calculate_employee_total_hours(i);
        int target_hours = employee_list[i].weekly_target_hours;

        // Suggest time-off if exceeded by > 50%
        if (total_hours > (target_hours * 1.5)) {
            printf("Suggestion: Employee %s (ID: %d) exceeded target by over 50%%. Recommend time-off.\n",
                   employee_list[i].name, employee_list[i].employee_id);
        }

        // Add to qualified list only if they met target
        if (total_hours >= target_hours) {
            qualified_employees[qualified_count].emp_id = employee_list[i].employee_id;
            qualified_employees[qualified_count].total_hours = total_hours;
            qualified_count++;
        }
    }

    if (qualified_count == 0) {
        printf("No employees met their weekly target hours.\n");
        return;
    }

    // 2. Sort the qualified employees by total hours (descending)
    qsort(qualified_employees, qualified_count, sizeof(emp_performance), compare_emp_performance);

    // 3. Display the top 5
    printf("Top performers who met their goals:\n");
    int limit = (qualified_count < 5) ? qualified_count : 5;

    for (int i = 0; i < limit; i++) {
        int emp_id = qualified_employees[i].emp_id;
        int emp_index = find_employee_by_id(emp_id);
        if (emp_index != -1) {
            printf("Rank %d: %s (ID: %d) - Total Hours: %d (Target: %d)\n",
                   i + 1,
                   employee_list[emp_index].name,
                   emp_id,
                   qualified_employees[i].total_hours,
                   employee_list[emp_index].weekly_target_hours);
        }
    }
}

// Case 4: Check Team Achievement
void check_team_achievement() {
    int team_id;
    printf("Enter Team ID to check: ");
    scanf("%d", &team_id);

    int team_index = find_team_by_id(team_id);
    if (team_index == -1) {
        printf("Error: Team ID %d not found.\n", team_id);
        return;
    }

    team t = team_list[team_index];
    int total_hours = calculate_team_total_hours(team_index);

    printf("\n--- Team Achievement for %s (ID: %d) ---\n", t.team_name, t.team_id);
    printf("Team Target Hours: %d\n", t.weekly_team_target);
    printf("Total Hours Worked: %d\n", total_hours);

    if (total_hours >= t.weekly_team_target) {
        printf("Result: ACHIEVED\n");
    } 
    else {
        printf("Result: NOT ACHIEVED (Short by %d hours)\n", t.weekly_team_target - total_hours);
    }
}

// Case 5: Generate Team Ranking
void generate_team_ranking() {
    if (team_count == 0) {
        printf("No teams available to rank.\n");
        return;
    }

    team_performance ranks[MAX_TEAMS];

    // 1. Calculate total hours for all teams
    for (int i = 0; i < team_count; i++) {
        ranks[i].team_index = i; // Use index from header struct
        ranks[i].total_hours = calculate_team_total_hours(i);
    }

    // 2. Sort teams in descending order of total hours
    qsort(ranks, team_count, sizeof(team_performance), compare_team_ranks);

    // 3. Display ranking
    printf("\n--- Team Ranking by Total Hours Worked ---\n");
    for (int i = 0; i < team_count; i++) {
        int team_index = ranks[i].team_index;
        team t = team_list[team_index];
        printf("Rank %d: %s (ID: %d) - Total Hours: %d\n",
               i + 1,
               t.team_name,
               t.team_id,
               ranks[i].total_hours);
    }
}

// Case 6: Check Employee Reward
// Helper function to build ranked list for rewards
int get_employee_rank(int emp_id, emp_performance* list, int count) {
    for (int i = 0; i < count; i++) {
        if (list[i].emp_id == emp_id) {
            return i + 1; // Rank is 1-based
        }
    }
    return -1; // Not in qualified list
}

void check_employee_reward() {
    int emp_id;
    printf("Enter Employee ID to check rewards: ");
    scanf("%d", &emp_id);

    int emp_index = find_employee_by_id(emp_id);
    if (emp_index == -1) {
        printf("Error: Employee ID %d not found.\n", emp_id);
        return;
    }

    employee emp = employee_list[emp_index];
    int total_hours = calculate_employee_total_hours(emp_index);
    int target_hours = emp.weekly_target_hours;
    int points = 0;

    // 1. Check if target was met
    if (total_hours < target_hours) {
        points = 0;
        printf("Employee %s (ID: %d) did not meet target. Reward: %d points.\n", emp.name, emp.employee_id, points);
        return;
    }
    
    // 2. Target was met, now find rank among qualified employees
    emp_performance qualified_employees[MAX_EMPLOYEES];
    int qualified_count = 0;
    for (int i = 0; i < employee_count; i++) {
        int th = calculate_employee_total_hours(i);
        int tt = employee_list[i].weekly_target_hours;
        if (th >= tt) {
            qualified_employees[qualified_count].emp_id = employee_list[i].employee_id;
            qualified_employees[qualified_count].total_hours = th;
            qualified_count++;
        }
    }

    // Sort the qualified list
    qsort(qualified_employees, qualified_count, sizeof(emp_performance), compare_emp_performance);

    // Get rank
    int rank = get_employee_rank(emp_id, qualified_employees, qualified_count);

    // 3. Assign points based on rank and target status
    if (rank == 1) {
        points = 100;
    }
    else if (rank == 2) {
        points = 75;
    } 
    else if (rank == 3) {
        points = 50;
    } 
    else if (rank > 3) {
        // Met target but not in top 3
        if (total_hours > target_hours) {
            points = 20; // Exceeded target
        } 
        else {
            points = 10; // Exactly met target
        }
    } 
    else {
        // This case handles rank = -1 (not in qualified list) as a fallback case
        if (total_hours > target_hours) {
            points = 20;
        } 
        else {
            points = 10;
        }
    }
    
    printf("Employee %s (ID: %d) - Rank: %d (among qualifiers) - Reward: %d points.\n", emp.name, emp.employee_id, rank, points);
}


// Case 7: Update or Delete Employee
void update_or_delete_employee() {
    int emp_id;
    printf("Enter Employee ID: ");
    scanf("%d", &emp_id);

    int emp_index = find_employee_by_id(emp_id);
    if (emp_index == -1) {
        printf("Error: Employee ID %d not found.\n", emp_id);
        return;
    }

    printf("Employee found: %s\n", employee_list[emp_index].name);
    printf("1. Update Employee\n");
    printf("2. Delete Employee\n");
    printf("Enter choice: ");
    
    int choice;
    scanf("%d", &choice);

    if (choice == 1) {
        // --- UPDATE ---
        printf("What to update?\n");
        printf("1. Name\n");
        printf("2. Age\n");
        printf("3. Department\n");
        printf("4. Weekly Target Hours\n");
        printf("5. Weekly Working Hours\n");
        printf("Enter choice: ");
        int update_choice;
        scanf("%d", &update_choice);

        switch (update_choice) {
            case 1:
                printf("Enter new Name: ");
                scanf(" %[^\n]", employee_list[emp_index].name);
                break;
            case 2:
                printf("Enter new Age: ");
                scanf("%d", &employee_list[emp_index].age);
                break;
            case 3:
                printf("Enter new Department: ");
                scanf(" %[^\n]", employee_list[emp_index].department);
                break;
            case 4:
                printf("Enter new Weekly Target Hours: ");
                scanf("%d", &employee_list[emp_index].weekly_target_hours);
                break;
            case 5:
                printf("Enter new hours for 7 days (Mon-Sun): ");
                for (int d = 0; d < DAYS_IN_WEEK; d++) {
                    scanf("%d", &employee_list[emp_index].weekly_working_hours[d]);
                }
                break;
            default:
                printf("Invalid choice.\n");
                return;
        }
        printf("Employee details updated successfully.\n");

    } 
    else if (choice == 2) {
        // --- DELETE ---
        
        // 1. Remove employee from their team (if any)
        for (int i = 0; i < team_count; i++) {
            int flag = 0;
            int member_index = -1;
            for (int j = 0; j < team_list[i].member_count; j++) {
                if (team_list[i].member_ids[j] == emp_id) {
                    member_index = j;
                    flag = 1;
                    break;
                }
            }

            if (flag) {
                // Shift members in team's array left
                for (int k = member_index; k < team_list[i].member_count - 1; k++) {
                    team_list[i].member_ids[k] = team_list[i].member_ids[k + 1];
                }
                team_list[i].member_count--;
                printf("Employee removed from team %s.\n", team_list[i].team_name);
                break; // Employee can only be in one team
            }
        }

        // 2. Remove employee from employee_list (shift left)
        for (int i = emp_index; i < employee_count - 1; i++) {
            employee_list[i] = employee_list[i + 1];
        }
        employee_count--;
        printf("Employee %d deleted successfully.\n", emp_id);

    } 
    else {
        printf("Invalid choice.\n");
    }
}

// Case 8: Delete Team
void delete_team() {
    int team_id;
    printf("Enter Team ID to delete: ");
    scanf("%d", &team_id);

    int team_index = find_team_by_id(team_id);
    if (team_index == -1) {
        printf("Error: Team ID %d not found.\n", team_id);
        return;
    }

    // Employees are retained, so we just need to delete the team
    // Shift all teams after this one to the left
    for (int i = team_index; i < team_count - 1; i++) {
        team_list[i] = team_list[i + 1];
    }
    team_count--;
    
    printf("Team %d deleted successfully. Its members are now unassigned.\n", team_id);
}

// Case 9: Merge Teams
void merge_teams() {
    int team_id1, team_id2;
    printf("Enter ID of first team to merge: ");
    scanf("%d", &team_id1);
    printf("Enter ID of second team to merge: ");
    scanf("%d", &team_id2);

    if (team_id1 == team_id2) {
        printf("Error: Cannot merge a team with itself.\n");
        return;
    }

    int index1 = find_team_by_id(team_id1);
    int index2 = find_team_by_id(team_id2);

    if (index1 == -1 || index2 == -1) {
        printf("Error: One or both Team IDs not found.\n");
        return;
    }
    
    if (team_count >= MAX_TEAMS) {
        printf("Error: Team list is full. Cannot add merged team.\n");
        return;
    }

    team t1 = team_list[index1];
    team t2 = team_list[index2];

    // 1. Collect all members
    int all_members[MAX_TEAM_MEMBERS * 2];
    int member_count = 0;
    
    for (int i = 0; i < t1.member_count; i++) {
        all_members[member_count++] = t1.member_ids[i];
    }
    for (int i = 0; i < t2.member_count; i++) {
        all_members[member_count++] = t2.member_ids[i];
    }
    
    // 2.sort members by employee id
    qsort(all_members, member_count, sizeof(int), compare_integers);

    // 3.get new team details
    team new_team;
    new_team.member_count = 0;
    
    printf("Enter new Team ID for merged team: ");
    scanf("%d", &new_team.team_id);

    // Find insertion pos and check for duplicate ID
    int i = 0;
    while (i < team_count && team_list[i].team_id < new_team.team_id) {
        i++;
    }
    if (i < team_count && team_list[i].team_id == new_team.team_id) {
        printf("Error: Team ID %d already exists.\n", new_team.team_id);
        return;
    }
    
    printf("Enter new Team Name: ");
    scanf(" %[^\n]", new_team.team_name);
    printf("Enter new Weekly Team Target Hours: ");
    scanf("%d", &new_team.weekly_team_target);

    // 4. Add members to new team (up to 5, keeping lowest IDs)
    int num_to_add = (member_count > MAX_TEAM_MEMBERS) ? MAX_TEAM_MEMBERS : member_count;
    for (int k = 0; k < num_to_add; k++) {
        new_team.member_ids[k] = all_members[k];
        new_team.member_count++;
    }

    if (member_count > MAX_TEAM_MEMBERS) {
        printf("Warning: Combined team had %d members. Discarded %d members with the highest ids.\n",
               member_count, member_count - MAX_TEAM_MEMBERS);
    }

    // 5. delete original two teams
    internal_delete_team_by_id(team_id1);
    internal_delete_team_by_id(team_id2);

    // 6. Add the new team (must re-find insertion spot after deletions)
    i = 0;
    while (i < team_count && team_list[i].team_id < new_team.team_id) {
        i++;
    }
    // Shift right
    for (int j = team_count; j > i; j--) {
        team_list[j] = team_list[j - 1];
    }
    // Insert
    team_list[i] = new_team;
    team_count++;

    printf("Teams merged successfully into new team %s (ID: %d).\n", new_team.team_name, new_team.team_id);
}


// Case 10: Display All Team Info
void display_team_info() {
    if (team_count == 0) {
        printf("No teams to display.\n");
        return;
    }
    
    // Generate ranking info first
    team_performance ranks[MAX_TEAMS];
    for (int i = 0; i < team_count; i++) {
        ranks[i].team_index = i;
        ranks[i].total_hours = calculate_team_total_hours(i);
    }
    qsort(ranks, team_count, sizeof(team_performance), compare_team_ranks);


    printf("\n--- Displaying All Team Information ---\n");
    for (int i = 0; i < team_count; i++) {
        team t = team_list[i];
        
        // Find current rank
        int current_rank = -1;
        for (int r = 0; r < team_count; r++) {
            if (ranks[r].team_index == i) {
                current_rank = r + 1;
                break;
            }
        }

        printf("\n----------------------------------------\n");
        printf("Team Name: %s (ID: %d)\n", t.team_name, t.team_id);
        
        printf("Members:\n");
        if (t.member_count == 0) {
            printf("  (No members)\n");
        }
        for (int j = 0; j < t.member_count; j++) {
            int emp_id = t.member_ids[j];
            int emp_index = find_employee_by_id(emp_id);
            if (emp_index != -1) {
                printf("  - %s (ID: %d)\n", employee_list[emp_index].name, emp_id);
                printf("    Individual Hours: %d\n", calculate_employee_total_hours(emp_index));
            } 
            else {
                printf("  - (Error: Unknown Member ID: %d)\n", emp_id);
            }
        }
        
        printf("Team Goal: %d hours\n", t.weekly_team_target);
        printf("Team Total Worked: %d hours\n", calculate_team_total_hours(i));
        printf("Team Rank: %d\n", current_rank);
        printf("----------------------------------------\n");
    }
}

//====================================================================
// AUTO FUNCTIONS
//====================================================================

//Save Employee Data 
void save_data() {
    FILE *emp_file = fopen("employees.txt", "w");
    if (emp_file == NULL) {
        printf("Error: Could not open employees.txt for writing.\n");
        return;
    }
    
    // Save employee count first
    fprintf(emp_file, "%d\n", employee_count);
    for (int i = 0; i < employee_count; i++) {
        employee e = employee_list[i];
        fprintf(emp_file, "%d\n", e.employee_id);
        fprintf(emp_file, "%s\n", e.name);
        fprintf(emp_file, "%d\n", e.age);
        fprintf(emp_file, "%s\n", e.department);
        fprintf(emp_file, "%d\n", e.weekly_target_hours);
        for (int d = 0; d < DAYS_IN_WEEK; d++) {
            fprintf(emp_file, "%d ", e.weekly_working_hours[d]);
        }
        fprintf(emp_file, "\n");
    }
    fclose(emp_file);
    printf("Employee data saved to employees.txt.\n");

    FILE *team_file = fopen("teams.txt", "w");
    if (team_file == NULL) {
        printf("Error: Could not open teams.txt for writing.\n");
        return;
    }

    // Save team count first
    fprintf(team_file, "%d\n", team_count);
    for (int i = 0; i < team_count; i++) {
        team t = team_list[i];
        fprintf(team_file, "%d\n", t.team_id);
        fprintf(team_file, "%s\n", t.team_name);
        fprintf(team_file, "%d\n", t.weekly_team_target);
        fprintf(team_file, "%d\n", t.member_count);
        for (int j = 0; j < t.member_count; j++) {
            fprintf(team_file, "%d ", t.member_ids[j]);
        }
        fprintf(team_file, "\n");
    }
    fclose(team_file);
    printf("Team data saved to teams.txt.\n");
}


void load_data() {
    FILE *emp_file = fopen("employees.txt", "r");
    if (emp_file == NULL) {
        printf("No employee data file found. Starting fresh.\n");
    } 
    else {
        fscanf(emp_file, "%d\n", &employee_count);
        if (employee_count > MAX_EMPLOYEES) employee_count = MAX_EMPLOYEES;
        for (int i = 0; i < employee_count; i++) {
            employee *e = &employee_list[i];
            fscanf(emp_file, "%d\n", &e->employee_id);
            fscanf(emp_file, " %[^\n]\n", e->name);
            fscanf(emp_file, "%d\n", &e->age);
            fscanf(emp_file, " %[^\n]\n", e->department);
            fscanf(emp_file, "%d\n", &e->weekly_target_hours);
            for (int d = 0; d < DAYS_IN_WEEK; d++) {
                fscanf(emp_file, "%d ", &e->weekly_working_hours[d]);
            }
            fscanf(emp_file, "\n");
        }
        fclose(emp_file);
        printf("Loaded %d employees from employees.txt.\n", employee_count);
    }

    FILE *team_file = fopen("teams.txt", "r");
    if (team_file == NULL) {
        printf("No team data file found. Starting fresh.\n");
    } 
    else {
        fscanf(team_file, "%d\n", &team_count);
        if (team_count > MAX_TEAMS) team_count = MAX_TEAMS;
        for (int i = 0; i < team_count; i++) {
            team *t = &team_list[i];
            fscanf(team_file, "%d\n", &t->team_id);
            fscanf(team_file, " %[^\n]\n", t->team_name);
            fscanf(team_file, "%d\n", &t->weekly_team_target);
            fscanf(team_file, "%d\n", &t->member_count);
            if (t->member_count > MAX_TEAM_MEMBERS) t->member_count = MAX_TEAM_MEMBERS;
            for (int j = 0; j < t->member_count; j++) {
                fscanf(team_file, "%d ", &t->member_ids[j]);
            }
            fscanf(team_file, "\n");
        }
        fclose(team_file);
        printf("Loaded %d teams from teams.txt.\n", team_count);
    }
}