#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TASKS 10
#define MAX_NAME_LEN 40

enum month {january, february, march, april, may, june, july, august, september,
    october, november, december};

const char *month_names[] = {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
};//more efficient as compiler only needs to allocate 12 pointer to memory, takes up less memory

typedef struct {// basic struct for each task
    char name[MAX_NAME_LEN];
    int start_month;
    int end_month;
    int num_dependencies;
    int dependencies[MAX_TASKS];
} task_t;

void sampleGannt(task_t tasks[], int num_tasks);
int get_int_input(char *prompt, int min, int max);
void editGannt(task_t *tasks, int num_tasks);
void generate_gantt(task_t *tasks, int num_tasks);
void add_task(task_t *tasks, int index, int num_tasks);
int taskIndex(task_t *tasks, int num_tasks);
void printDependentTasks(task_t tasks[], int taskId, int visitedTasks[]);
int checkIfCircular(task_t *tasks, int dependentTaskIndex, int taskToBeChecked) ;
void clrscr();
void ASCIIArt();
int main()
{

    task_t tasks[MAX_TASKS];// array of structs
    int num_tasks; // the number of tasks entered by the user
    char choice[3]; // the user's choice of using the sample chart or creating their own
    char userChoice[7]; // the user's choice of editing the chart, running a test, or quitting the program
    int i, index;



    printf("Welcome to the Gantt Generator\n"); // Print welcome message and prompt user
    do {

        printf("Would you like to use the test example or create your own Gantt from scratch? (yes or no)\n");
        // Read user input and check if they chose the sample Gantt diagram.
        scanf("%s", choice);

    } while ((strcmp(choice, "no") != 0) && (strcmp(choice, "yes") != 0));
    // If yes, set the number of tasks to 10 and create the sample Gantt diagram.

    if(strcmp(choice, "yes") == 0)
    {
        num_tasks = 10;

        sampleGannt(tasks, num_tasks);

        generate_gantt(tasks, num_tasks);
    }
        // If no, prompt the user to input the number of tasks they want to add and add each task to the array.
    else if(strcmp(choice, "no") == 0){
        num_tasks = get_int_input("How many tasks would you like to add?\n", 1, MAX_TASKS);
        for (i = 0; i < num_tasks; i++)
        {
            add_task(tasks, i, num_tasks);
        }
        // Generate the Gantt diagram.
        generate_gantt(tasks, num_tasks);
    }


    // Loop until the user chooses to quit the program.
    do{
        // Prompt the user to choose between editing the Gantt diagram, running a test or quitting the program.
        printf("If you wish to edit the Gantt please type 'edit' / If you wish to run a test, type 'test' or to exit type 'quit' and press enter to execute\n");
        // Read user input and check their choice.
        scanf("%6s", userChoice);//safer
        if(strcmp("edit", userChoice)==0)
        {
            // If the user chose to edit the Gantt diagram, call the editGannt function to allow them to edit the dependencies.
            editGannt(tasks, num_tasks);
            clrscr(); // Clear the previous Gantt diagram before printing the edited one.
            generate_gantt(tasks, num_tasks); // Generate the updated Gantt diagram.
        }
        else if(strcmp("test", userChoice)==0)
        {
            // need to change
            int visitedTasks[MAX_TASKS]={0};
            index=taskIndex(tasks, num_tasks);
            if(index>=0) {
                printDependentTasks(tasks, index, visitedTasks) ;//starts at 0
            }
        }




    }while(strcmp("quit", userChoice)!=0);

    ASCIIArt();
    return 0;
}
// Creates a Sample Gantt chart if the user selects this option
void sampleGannt(task_t tasks[], int num_tasks)
{
    // initializing the tasks array to a sample output that the user may generate

    strcpy(tasks[0].name, "Find_Bank");
    tasks[0].start_month = 1;
    tasks[0].end_month = 2;
    tasks[0].num_dependencies = 0;


    strcpy(tasks[1].name, "Get_mortgage_approval");
    tasks[1].start_month = 2;
    tasks[1].end_month = 2;
    tasks[1].num_dependencies = 1;
    tasks[1].dependencies[0] = 0;

    strcpy(tasks[2].name, "Draw_Down");
    tasks[2].start_month = 4;
    tasks[2].end_month = 7;
    tasks[2].num_dependencies = 2;
    tasks[2].dependencies[0] = 0;
    tasks[2].dependencies[1] = 1;

    strcpy(tasks[3].name, "Build_Foundation");
    tasks[3].start_month = 3;
    tasks[3].end_month = 4;
    tasks[3].num_dependencies = 1;
    tasks[3].dependencies[0] = 2;

    strcpy(tasks[4].name, "Build_Walls");
    tasks[4].start_month = 4;
    tasks[4].end_month = 5;
    tasks[4].num_dependencies = 0;


    strcpy(tasks[5].name, "Roof_and_Ceiling");
    tasks[5].start_month = 5;
    tasks[5].end_month = 6;
    tasks[5].num_dependencies = 2;
    tasks[5].dependencies[0] = 3;
    tasks[5].dependencies[1] = 4;

    strcpy(tasks[6].name, "Plumbing");
    tasks[6].start_month = 7;
    tasks[6].end_month = 7;
    tasks[6].num_dependencies = 1;
    tasks[6].dependencies[0] = 5;


    strcpy(tasks[7].name, "Electrics");
    tasks[7].start_month = 8;
    tasks[7].end_month = 9;
    tasks[7].num_dependencies = 1;
    tasks[7].dependencies[0] = 6;

    strcpy(tasks[8].name, "Inspect_Building");
    tasks[8].start_month = 9;
    tasks[8].end_month = 10;
    tasks[8].num_dependencies = 1;
    tasks[8].dependencies[0] = 4;

    strcpy(tasks[9].name, "Snagging");
    tasks[9].start_month = 9;
    tasks[9].end_month = 10;
    tasks[9].num_dependencies = 2;
    tasks[9].dependencies[0] = 7;
    tasks[9].dependencies[1] = 8;
}

//The function loops until the user enters a valid input within the specified range.
int get_int_input(char *prompt, int min, int max)
{
    int input;
    do {
        //Prompt the user with the given prompt string and the range of acceptable inputs.
        printf("%s (%d-%d): ", prompt, min, max);
        //Read an integer input from the user
        scanf("%d", &input);
        //Loop until a valid input within the specified range is entered.
    } while (input < min || input > max);
    //Return the validated input.
    return input;
}

// This function adds a task to an array of tasks
void add_task(task_t *tasks, int index, int num_tasks)
{
    // Declare variables to hold information about the new task
    char name[MAX_NAME_LEN];
    int start_month, end_month, num_dependencies, dep_task;

    // Prompt the user to enter the name of the new task
    printf("Please enter the task name: ");
    // Read in the task name from standard input
    scanf("%s", name);
    // Initialize the new task's number of dependencies to 0
    tasks[index].num_dependencies = 0;
    // Copy the task name to the new task's name field
    strcpy(tasks[index].name, name);
    // Prompt the user to enter the start and end months for the new task
    // Get the start month an End month input from the user and validate that it is between 1 and 12
    start_month = get_int_input("Start month", 1, 12);//make sure only number between 1 and 12
    end_month = get_int_input("End month", start_month, 12);
    // Set the start and end months for the new task
    tasks[index].start_month = start_month;
    tasks[index].end_month = end_month;
    // Prompt the user to enter the number of dependencies and store in variable
    do {
        printf("Enter how many dependencies this task has\n");
        scanf("%d", &num_dependencies);
    }while(num_dependencies<0 || num_dependencies>=num_tasks);//edge cases

    tasks[index].num_dependencies = num_dependencies;

    // Loop through each dependency and prompt the user to enter the index of the dependent task.
    for (int i = 0; i < num_dependencies; i++)
    {
        // Prompt the user to enter the ID of a dependent task
        do {
            printf("Enter dependent task\n");
            // Read the user's input and store it in the variable "dep_task"
            scanf("%d", &dep_task);
        }while(dep_task<1 || dep_task>num_tasks);
        // Stores the dependencies index starts at 0 so we minus 1;
        tasks[index].dependencies[i] = dep_task-1;
    }
}

// This function generates a Gantt chart for a set of tasks
void generate_gantt(task_t *tasks, int num_tasks)
{
    int i, j, k;
    // Print the header for the Gantt chart
    printf("\n");
    printf("Gantt chart:\n");
    printf("\n");

    // Print the month names along the top of the chart
    printf("                         ");
    //enum used
    for (i = january; i <= december; i++){
        printf("|%9s", month_names[i]);
    }
    printf("| dependencies |\n");

    // Print a line of dashes to separate the header from the chart
    for (i = 0; i < 27; i++) {
        printf("------");
    }
    printf("\n");

    // Loop through all the tasks and print their information in the chart
    for (i = 0; i < num_tasks; i++)
    {
        // Print the name of the current task
        printf("%-25s", tasks[i].name);

        // Print a column for each month, with "XXX" in the columns corresponding to the task's start and end months
        for (j = 0; j < 12; j++)
        {
            // If the current month is within the task's start and end months, print "XXX"
            if (j >= tasks[i].start_month-1 && j < tasks[i].end_month)
            {
                printf("|   XXX   ");
            }
            else
            {
                // Otherwise, print a blank space
                printf("|         ");
            }
        }
        // If the current task has dependencies, print them
        if (tasks[i].num_dependencies > 0) {
            printf("|");
            for (k = 0; k < tasks[i].num_dependencies; k++)
            {
                // Print the dependencies and not index
                printf(" %d ", tasks[i].dependencies[k]+1);
            }
            // Print a blank space of appropriate length based on the number of dependencies
            printf("%*s  |", 12 - tasks[i].num_dependencies*3, "");
        }
            // if no dependencies print blank space
        else
        {
            printf("|              |");
        }
        printf("\n");
    }

    // Print a horizontal line at the end
    for (i = 0; i < 27; i++) {
        printf("------");
    }
    printf("\n");
}

// allows the user to edit a task from the gantt chart
void editGannt(task_t tasks[], int num_tasks)
{
    char taskToEdit[MAX_NAME_LEN]; // stores the task the user would like to edit

    // Prompt the user to enter the name of the task they want to edit
    printf("Please enter task you would like to edit\n");
    scanf("%s", taskToEdit);

    // Loop through all the tasks
    for(int i=0; i<num_tasks; i++)
    {
        // Check if the name of the current task matches the name entered by the user
        if(strcmp(tasks[i].name, taskToEdit)==0)
        {
            // If the task is found, print a message and call the add_task function to edit the task
            printf("Task has been found\n");
            add_task(tasks, i, num_tasks);
        }
    }
    printf("Task has not been found\n");
}

// Command clears screen
void clrscr()
{
    system("cls");
}

void ASCIIArt()
{
    printf("  _____                          __________\n");
    printf(" / ____|                        |___    ___|\n");
    printf("| (___          _______             |  |\n");
    printf(" \\___ \\        |_______|            |  |\n");
    printf(" ____) |                            |  |\n");
    printf("|_____/                       -------  |\n");
    printf("                             \\________/\n");



}

int taskIndex(task_t *tasks, int num_tasks) {

    // Declare a character array called testTask
    char testTask[MAX_NAME_LEN];

    // Prompt the user to enter a task name to test and store it in testTask
    printf("Please enter the task name to test");
    scanf("%39s", testTask);

    // Iterate through each task in the array
    for(int i=0; i<num_tasks; i++) {
        // Compare the task name with the testTask using the strcmp() function
        if(strcmp(tasks[i].name, testTask)==0) {
            // If a match is found, print a message indicating that the task has been found and return the index of the task in the array
            printf("Task has been found\n");
            return i;
        }
    }
    // If no match is found after iterating through all tasks, print a message indicating that the task was not found and return -1
    printf("Task not found please try again\n");
    return -1;
}



void printDependentTasks(task_t *tasks, int taskIndex, int visitedTasks[]) {// there is no gaurd statement
    int taskToBeChecked;

    printf("%d -> ", taskIndex+1);
    visitedTasks[taskIndex] = 1;//the starting task is given a one
    int resultFromCircular;
    for(int i=0; i < tasks[taskIndex].num_dependencies; i++) { //for loop until reaches number of dependencies
        int dependentTaskIndex = tasks[taskIndex].dependencies[i];//goes through dependent tasks and quit

        if(visitedTasks[dependentTaskIndex] == 0) {
            printDependentTasks(tasks, dependentTaskIndex, visitedTasks);

        }

        else
        {

            taskToBeChecked=dependentTaskIndex;
            printf("( !!!! warning potential circular dependency !!!!!) \n");
            resultFromCircular=checkIfCircular(tasks, dependentTaskIndex, taskToBeChecked);

            if(resultFromCircular==1) {
                printf("( !!!! circular dependency found !!!!!) \n");
            }
            else if(resultFromCircular==0) {
                printf("( !!!!  circular dependency not found !!!!!) \n");
            }
        }

    }
}



int checkIfCircular(task_t *tasks, int dependentTaskIndex, int taskToBeChecked) {
    //breaks if there are no dependencies,
    for (int i = 0; i < tasks[dependentTaskIndex].num_dependencies; i++) {
        int childTaskIndex = tasks[dependentTaskIndex].dependencies[i];
        //if childTaskIndex equal to taskToBeChecked then there must be a loop
        if (childTaskIndex == taskToBeChecked) {
            return 1;
        }
            //if recursive fucntion returns a one there
        else if (checkIfCircular(tasks, childTaskIndex, taskToBeChecked) == 1) {
            return 1;
        }
    }
    return 0;
}