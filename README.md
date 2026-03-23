# Work Schedule Management System

> A robust, command-line utility built in C to manage employees, organize teams, and track weekly working hours and performance goals.

## 📖 Description

The Work Schedule Management System is a C-based application designed to help managers and administrators keep track of employee hours, team assignments, and overall productivity. It allows users to perform CRUD (Create, Read, Update, Delete) operations on employee and team records, calculate weekly performance against set targets, and generate rankings. 

The system features automatic data persistence, saving and loading records from text files (`employees.txt` and `teams.txt`) so no data is lost between sessions.

## ✨ Features

* **Employee Management:** Add new employees, update their details (name, age, department, working hours), or remove them from the system.
* **Team Organization:** Create teams (up to 5 members), assign weekly targets, delete teams, and merge two existing teams into a new one.
* **Performance Tracking:** Automatically calculates total weekly hours and evaluates if teams or individuals met their target hours.
* **Rankings & Leaderboards:** View the top 5 performing employees and generate a leaderboard of teams sorted by total hours worked.
* **Automated Rewards:** Calculates reward points for employees based on whether they met their targets and how they rank among their peers.
* **Data Persistence:** Automatically loads existing data on startup and saves changes after every operation.

## 🛠️ Tech Stack

* **Language:** C
* **Data Storage:** Flat-file storage (`.txt` files)
* **Algorithms:** Binary Search (for ID lookups), Quicksort (`qsort` for ranking and sorting)

## 📁 Project Structure

* `main.c`: The entry point and interactive menu loop of the application.
* `functions.c`: The core logic, including helper functions, searching, sorting, and file I/O.
* `header.h`: Constants, struct definitions (`employee`, `team`), and function prototypes.
* `employees.txt`: Automatically generated database for storing employee records.
* `teams.txt`: Automatically generated database for storing team records.

## 🚀 Getting Started

Follow these instructions to compile and run the project on your local machine.

### Prerequisites

You need a C compiler (like GCC) installed on your system.

### Installation & Execution

1. **Clone the repository:**
   ```bash
   git clone [https://github.com/yourusername/work-schedule-management.git](https://github.com/yourusername/work-schedule-management.git)
