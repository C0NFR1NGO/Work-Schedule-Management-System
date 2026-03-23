//Header File

#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

// --- Constants ---
#define MAX_EMPLOYEES 1000
#define MAX_TEAMS 100
#define MAX_TEAM_MEMBERS 5
#define DAYS_IN_WEEK 7
#define MAX_NAME_LEN 256

// --- Structures ---

// Stores information for one employee
typedef struct {
    int employee_id;
    char name[MAX_NAME_LEN];
    int age;
    char department[MAX_NAME_LEN];
    int weekly_working_hours[DAYS_IN_WEEK];
    int weekly_target_hours;
} employee;

// Stores information for one team
typedef struct {
    int team_id;
    char team_name[MAX_NAME_LEN];
    int member_ids[MAX_TEAM_MEMBERS];
    int member_count; // Number of members currently in the team
    int weekly_team_target;
} team;

// Helper structure for sorting employees by performance
typedef struct {
    int emp_id;
    int total_hours;
} emp_performance;

// Helper structure for sorting teams by performance
typedef struct {
    int team_index; // Index in the global team_list
    int total_hours;
} team_performance;


// --- Global Variable Declarations (defined in main.c) ---
extern employee employee_list[MAX_EMPLOYEES];
extern team team_list[MAX_TEAMS];
extern int employee_count;
extern int team_count;

// --- Function Prototypes ---

// File Handling
void load_data();
void save_data();

// Core Functionality
void add_employee();
void create_team();
void top_5_employees();
void check_team_achievement();
void generate_team_ranking();
void check_employee_reward();
void update_or_delete_employee();
void delete_team();
void merge_teams();
void display_team_info();

// Helper Functions
int calculate_employee_total_hours(int emp_index);
int calculate_team_total_hours(int team_index);
int find_team_by_id(int team_id);
int find_employee_by_id(int emp_id);
int get_employee_rank(int emp_id, emp_performance* list, int count);
void internal_delete_team_by_id(int team_id); // Helper for merge_teams
int is_employee_in_team(int emp_id); // Helper for create_team

// Comparison Functions for qsort
int compare_team_ranks(const void *a, const void *b);
int compare_emp_performance(const void *a, const void *b);
int compare_integers(const void *a, const void *b); // Helper for merge_teams

#endif // HEADER_H