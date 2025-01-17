#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX_PASS_LEN 30
#define ADMIN_PASSWORD "mr_ghost"

int generateUniqueId(const char *filename)
{
    FILE *file = fopen(filename, "r+"); // Open file in read/write mode
    int id = 0;

    if (file == NULL)
    {                                // If the file does not exist
        file = fopen(filename, "w"); // Create a new file
        if (file == NULL)
        {
            printf("Error: Could not create file %s\n", filename);
            exit(1);
        }
        fprintf(file, "%d", 1); // Initialize the ID counter to 1
        fclose(file);
        return 1; // Return the first ID
    }

    // Read the current ID from the file
    fscanf(file, "%d", &id);
    rewind(file); // Move the file pointer to the beginning

    // Increment the ID and write it back to the file
    fprintf(file, "%d", id + 1);
    fclose(file);

    return id + 1; // Return the new unique ID
}

//-----------------------------Patient Info Start---------------------------------//

// Define a structure for patient data
struct Patient
{
    int p_id;
    char p_name[50];
    int age;
    char disease[50];
};

// Function to add a patient record
void add_patient()
{
    struct Patient patient;
    FILE *file;

    // Open file in append mode
    file = fopen("patients.txt", "a");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    // Input patient details
    printf("Enter Patient ID: ");
    scanf("%d", &patient.p_id);
    getchar(); // Clear newline character from buffer
    printf("Enter Patient Name: ");
    fgets(patient.p_name, sizeof(patient.p_name), stdin);
    patient.p_name[strcspn(patient.p_name, "\n")] = '\0'; // Remove newline
    printf("Enter Patient Age: ");
    scanf("%d", &patient.age);
    getchar(); // Clear newline character from buffer
    printf("Enter Disease: ");
    fgets(patient.disease, sizeof(patient.disease), stdin);
    patient.disease[strcspn(patient.disease, "\n")] = '\0'; // Remove newline

    // Write patient data to the file
    fprintf(file, "%d,%s,%d,%s\n", patient.p_id, patient.p_name, patient.age, patient.disease);

    fclose(file);
    printf("Patient record added successfully!\n");
}

// Function to display all patient records
void display_patients()
{
    struct Patient patient;
    FILE *file;

    // Open file in read mode
    file = fopen("patients.txt", "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    printf("\nPatient Records:\n");
    printf("ID\tName\t\tAge\tDisease\n");
    printf("----------------------------------------\n");

    // Read and display each patient record
    while (fscanf(file, "%d,%49[^,],%d,%49[^\n]\n", &patient.p_id, patient.p_name, &patient.age, patient.disease) == 4)
    {
        printf("%d\t%s\t\t%d\t%s\n", patient.p_id, patient.p_name, patient.age, patient.disease);
    }

    fclose(file);
}

void search_patient()
{
    int search_id;
    struct Patient patient;
    FILE *file = fopen("patients.txt", "r"); // Open in read mode

    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    printf("Enter Patient ID to search: ");
    scanf("%d", &search_id);

    int found = 0; // Flag to check if patient is found

    // Read data from the file and search for the given ID
    while (fscanf(file, "%d,%49[^,],%d,%49[^\n]\n",
                  &patient.p_id, patient.p_name, &patient.age, patient.disease) == 4)
    {
        if (patient.p_id == search_id)
        {
            // Display patient details if found
            printf("\nPatient Found:\n");
            printf("ID: %d\nName: %s\nAge: %d\nDisease: %s\n",
                   patient.p_id, patient.p_name, patient.age, patient.disease);
            found = 1;
            break;
        }
    }

    if (!found)
    {
        printf("Patient with ID %d not found.\n", search_id);
    }

    fclose(file); // Close the file
}

void delete_patient()
{
    int delete_id;
    struct Patient patient;
    FILE *file = fopen("patients.txt", "r");  // Open the original file in read mode
    FILE *temp_file = fopen("temp.txt", "w"); // Open a temporary file in write mode

    if (file == NULL || temp_file == NULL)
    {
        perror("Error opening file");
        return;
    }

    printf("Enter Patient ID to delete: ");
    scanf("%d", &delete_id);

    int found = 0; // Flag to check if the patient exists

    // Read each record and write it to the temp file, except the one to delete
    while (fscanf(file, "%d,%49[^,],%d,%49[^\n]\n",
                  &patient.p_id, patient.p_name, &patient.age, patient.disease) == 4)
    {
        if (patient.p_id == delete_id)
        {
            found = 1; // Mark as found but skip writing it to temp file
        }
        else
        {
            // Write all other records to the temp file
            fprintf(temp_file, "%d,%s,%d,%s\n",
                    patient.p_id, patient.p_name, patient.age, patient.disease);
        }
    }

    fclose(file);
    fclose(temp_file);

    if (found)
    {
        // Replace the original file with the updated temp file
        remove("patients.txt");
        rename("temp.txt", "patients.txt");
        printf("Patient with ID %d deleted successfully.\n", delete_id);
    }
    else
    {
        // If the patient was not found, delete the temp file
        remove("temp.txt");
        printf("Patient with ID %d not found.\n", delete_id);
    }
}

//--------------------------- Doctor Info start-----------------------------------//

// Define the structure for a doctor
struct Doctor
{
    int id;                  // Unique ID for the doctor
    char name[50];           // Doctor's name
    char specialization[50]; // Specialization field
    char time_schedule[50];  // Time schedule (e.g., "10:00 AM - 4:00 PM")
    float fees;              // Consultation fees
};

// Function to add a new doctor to the file
void addDoctor()
{
    const char *idFile = "doctor_id.txt";
    const char *dataFile = "doctors.txt";

    int id = generateUniqueId(idFile);

    struct Doctor doc;
    doc.id = id;

    printf("Enter Doctor's Name: ");
    scanf(" %[^\n]s", doc.name);

    printf("Enter Specialization: ");
    scanf(" %[^\n]s", doc.specialization);

    printf("Enter Time Schedule (e.g., 10:00 AM - 4:00 PM): ");
    scanf(" %[^\n]s", doc.time_schedule);

    printf("Enter Consultation Fees: ");
    scanf("%f", &doc.fees);

    // Save the doctor's information to the file
    FILE *file = fopen(dataFile, "a");
    if (file == NULL)
    {
        printf("Error: Could not open %s\n", dataFile);
        return;
    }

    fprintf(file, "%d,%s,%s,%s,%.2f\n",
            doc.id, doc.name, doc.specialization, doc.time_schedule, doc.fees);

    fclose(file);
    printf("Doctor added successfully! ID: %d\n", doc.id);
}

// Function to display all doctors from the file
void viewDoctors()
{
    const char *dataFile = "doctors.txt";

    FILE *file = fopen(dataFile, "r");
    if (file == NULL)
    {
        printf("Error: Could not open %s\n", dataFile);
        return;
    }

    struct Doctor doc;

    printf("\nDoctor Information:\n");
    printf("ID\tName\t\tSpecialization\t\tTime Schedule\t\tFees\n");
    printf("-------------------------------------------------------------------------------\n");

    while (fscanf(file, "%d,%49[^,],%49[^,],%49[^,],%f\n",
                  &doc.id, doc.name, doc.specialization,
                  doc.time_schedule, &doc.fees) == 5)
    {
        printf("%d\t%s\t\t%s\t\t\t%s\t\t%.2f\n",
               doc.id, doc.name, doc.specialization, doc.time_schedule, doc.fees);
    }

    fclose(file);
}

// Function to search for a doctor by ID
void searchDoctor()
{
    FILE *file = fopen("doctors.txt", "r");
    if (!file)
    {
        printf("Error: Could not open doctors.txt\n");
        return;
    }

    int searchChoice;
    printf("\nSearch Doctor by:\n1. ID\n2. Specialty\nEnter your choice: ");
    scanf("%d", &searchChoice);

    if (searchChoice == 1)
    {
        int searchId, id;
        char name[50], specialty[50], time[50];
        float fees;
        printf("Enter Doctor ID: ");
        scanf("%d", &searchId);

        int found = 0;
        while (fscanf(file, "%d,%49[^,],%49[^,],%49[^,],%f\n", &id, name, specialty, time, &fees) != EOF)
        {
            if (id == searchId)
            {
                printf("\nDoctor Found:\n");
                printf("ID: %d\nName: %s\nSpecialty: %s\nTime: %s\nFees: %.2f\n", id, name, specialty, time, fees);
                found = 1;
                break;
            }
        }

        if (!found)
        {
            printf("No doctor found with ID %d.\n", searchId);
        }
    }
    else if (searchChoice == 2)
    {
        char searchSpecialty[50], name[50], specialty[50], time[50];
        int id;
        float fees;
        printf("Enter Doctor Specialty: ");
        getchar(); // To clear the buffer
        fgets(searchSpecialty, sizeof(searchSpecialty), stdin);
        searchSpecialty[strcspn(searchSpecialty, "\n")] = '\0'; // Remove newline character

        int found = 0;
        printf("\nDoctors with Specialty '%s':\n", searchSpecialty);
        printf("ID\tName\t\tSpecialty\t\tTime\t\tFees\n");
        printf("-------------------------------------------------------------------\n");
        while (fscanf(file, "%d,%49[^,],%49[^,],%49[^,],%f\n", &id, name, specialty, time, &fees) != EOF)
        {
            if (strcasecmp(specialty, searchSpecialty) == 0)
            {
                printf("%d\t%s\t\t%s\t\t%s\t\t%.2f\n", id, name, specialty, time, fees);
                found = 1;
            }
        }

        if (!found)
        {
            printf("No doctors found with specialty '%s'.\n", searchSpecialty);
        }
    }
    else
    {
        printf("Invalid choice. Please select 1 or 2.\n");
    }

    fclose(file);
}

// Function to delete a doctor by ID
void deleteDoctor()
{
    FILE *file = fopen("doctors.txt", "r");
    if (!file)
    {
        printf("Error: Could not open doctors.txt\n");
        return;
    }

    FILE *tempFile = fopen("temp.txt", "w");
    if (!tempFile)
    {
        printf("Error: Could not create a temporary file.\n");
        fclose(file);
        return;
    }

    int deleteId, id;
    char name[50], specialty[50], time[50];
    float fees;

    printf("Enter the ID of the doctor to delete: ");
    scanf("%d", &deleteId);

    int found = 0;
    while (fscanf(file, "%d,%49[^,],%49[^,],%49[^,],%f\n", &id, name, specialty, time, &fees) != EOF)
    {
        if (id == deleteId)
        {
            found = 1; // Mark the record for deletion
        }
        else
        {
            // Write all records except the one to be deleted into the temporary file
            fprintf(tempFile, "%d,%s,%s,%s,%.2f\n", id, name, specialty, time, fees);
        }
    }

    fclose(file);
    fclose(tempFile);

    if (found)
    {
        // Replace original file with the updated file
        remove("doctors.txt");
        rename("temp.txt", "doctors.txt");
        printf("Doctor with ID %d has been deleted successfully.\n", deleteId);
    }
    else
    {
        // Remove the temporary file if no record was deleted
        remove("temp.txt");
        printf("Doctor with ID %d not found.\n", deleteId);
    }
}

//----------------------------------Appointment Info Start------------------------------//

#define MAX 100 // Maximum number of appointments

// Structure for an appointment
struct Appointment
{
    int a_id;
    char a_name[50];
    int age;
    char disease[50];
    char payment_status[10]; // "Paid" or "Unpaid"
};

// Queue structure
struct Queue
{
    struct Appointment appointments[MAX];
    int front;
    int rear;
};

// Initialize the queue
void initializeQueue(struct Queue *q)
{
    q->front = -1;
    q->rear = -1;
}

// Check if the queue is empty
int isEmpty(struct Queue *q)
{
    return q->front == -1;
}

// Check if the queue is full
int isFull(struct Queue *q)
{
    return q->rear == MAX - 1;
}

// Enqueue a new appointment
void enqueue(struct Queue *q, struct Appointment a)
{
    if (isFull(q))
    {
        printf("The appointment queue is full! Cannot add more patients.\n");
        return;
    }
    if (isEmpty(q))
    {
        q->front = 0;
    }
    q->rear++;
    q->appointments[q->rear] = a;
    printf("Patient %s with ID %d has been added to the appointment queue.\n", a.a_name, a.a_id);
}

void loadAppointments(struct Queue *q)
{
    FILE *file = fopen("appointments.txt", "r"); // Open in read mode
    if (file == NULL)
    {
        printf("Error: Could not open appointments.txt\n");
        return;
    }

    q->front = 0; // Initialize front of the queue
    q->rear = -1; // Initialize rear of the queue

    // Temporary variables to hold appointment data while reading from file
    int id, age;
    char name[50], disease[50], payment_status[10];

    // Read each line from the file and load into the queue
    while (fscanf(file, "%d,%49[^,],%d,%49[^,],%9[^\n]\n", &id, name, &age, disease, payment_status) == 5)
    {
        q->rear++;
        q->appointments[q->rear].a_id = id;
        strcpy(q->appointments[q->rear].a_name, name);
        q->appointments[q->rear].age = age;
        strcpy(q->appointments[q->rear].disease, disease);
        strcpy(q->appointments[q->rear].payment_status, payment_status);
    }

    fclose(file); // Close the file
    printf("Appointments loaded successfully.\n");
}

// Dequeue the next appointment (FIFO)
void dequeue(struct Queue *q)
{
    if (isEmpty(q))
    {
        printf("No appointments! Queue is empty.\n");
        return;
    }
    printf("Patient %s with ID %d has been called for their appointment.\n",
           q->appointments[q->front].a_name, q->appointments[q->front].a_id);

    if (q->front == q->rear)
    {
        q->front = q->rear = -1; // Queue becomes empty
    }
    else
    {
        q->front++;
    }
}

// Delete a specific appointment by ID
void deleteAppointmentByID(struct Queue *q, int id)
{
    if (isEmpty(q))
    {
        printf("No appointments! Queue is empty.\n");
        return;
    }

    int found = 0;
    for (int i = q->front; i <= q->rear; i++)
    {
        if (q->appointments[i].a_id == id)
        {
            found = 1;
            // Shift remaining elements to the left to fill the gap
            for (int j = i; j < q->rear; j++)
            {
                q->appointments[j] = q->appointments[j + 1];
            }
            q->rear--; // Adjust the rear pointer
            printf("Appointment with ID %d has been deleted.\n", id);
            break;
        }
    }

    if (!found)
    {
        printf("Appointment with ID %d not found.\n", id);
    }

    // If the queue becomes empty after deletion
    if (q->front > q->rear)
    {
        q->front = q->rear = -1;
    }
}

// Display all appointments in the queue
void displayQueue(struct Queue *q)
{
    if (isEmpty(q))
    {
        printf("No appointments in the queue.\n");
        return;
    }
    printf("\nCurrent Appointment Queue:\n");
    printf("ID\tName\t\tAge\tDisease\t\tPayment Status\n");
    printf("-------------------------------------------------------------\n");
    for (int i = q->front; i <= q->rear; i++)
    {
        printf("%d\t%s\t\t%d\t%s\t\t%s\n",
               q->appointments[i].a_id, q->appointments[i].a_name,
               q->appointments[i].age, q->appointments[i].disease,
               q->appointments[i].payment_status);
    }
}

//-------------------------------------Start of Resources Info------------------------------//

// Define a structure to store resource information
struct Resource
{
    int id;
    char name[50];
    int quantity;
    char type[30]; // e.g., Medicine, Equipment, Room
};

// Function to add a new resource
void add_resource()
{
    struct Resource resource;
    FILE *file = fopen("resources.txt", "a"); // Open in append mode

    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    printf("Enter Resource ID: ");
    scanf("%d", &resource.id);
    getchar(); // Clear the buffer

    printf("Enter Resource Name: ");
    fgets(resource.name, sizeof(resource.name), stdin);
    resource.name[strcspn(resource.name, "\n")] = '\0'; // Remove newline

    printf("Enter Quantity: ");
    scanf("%d", &resource.quantity);
    getchar(); // Clear the buffer

    printf("Enter Resource Type (e.g., Medicine, Equipment, Room): ");
    fgets(resource.type, sizeof(resource.type), stdin);
    resource.type[strcspn(resource.type, "\n")] = '\0'; // Remove newline

    // Write resource data to the text file in a readable format
    fprintf(file, "%d,%s,%d,%s\n", resource.id, resource.name, resource.quantity, resource.type);

    fclose(file);
    printf("Resource added successfully!\n");
}

// Function to display all resources
void display_resources()
{
    struct Resource resource;
    FILE *file = fopen("resources.txt", "r"); // Open in read mode

    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    printf("\nHospital Resources:\n");
    printf("ID\tName\t\tQuantity\tType\n");
    printf("----------------------------------------------\n");

    // Read and display each resource from the file
    while (fscanf(file, "%d,%49[^,],%d,%29[^\n]\n",
                  &resource.id, resource.name, &resource.quantity, resource.type) == 4)
    {
        printf("%d\t%s\t\t%d\t\t%s\n",
               resource.id, resource.name, resource.quantity, resource.type);
    }

    fclose(file);
}

// Function to search for a resource by ID
void search_resource()
{
    int search_id;
    struct Resource resource;
    FILE *file = fopen("resources.txt", "r"); // Open in read mode

    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    printf("Enter Resource ID to search: ");
    scanf("%d", &search_id);

    int found = 0;
    while (fscanf(file, "%d,%49[^,],%d,%29[^\n]\n",
                  &resource.id, resource.name, &resource.quantity, resource.type) == 4)
    {
        if (resource.id == search_id)
        {
            printf("\nResource Found:\n");
            printf("ID: %d\n", resource.id);
            printf("Name: %s\n", resource.name);
            printf("Quantity: %d\n", resource.quantity);
            printf("Type: %s\n", resource.type);
            found = 1;
            break;
        }
    }

    if (!found)
    {
        printf("Resource with ID %d not found.\n", search_id);
    }

    fclose(file);
}

// Function to update the quantity of a resource
void update_resource_quantity()
{
    int update_id, new_quantity;
    struct Resource resource;
    FILE *file = fopen("resources.txt", "r+"); // Open in read-write mode

    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    FILE *temp_file = fopen("temp.txt", "w"); // Temporary file for updated data
    if (temp_file == NULL)
    {
        perror("Error creating temporary file");
        fclose(file);
        return;
    }

    printf("Enter Resource ID to update: ");
    scanf("%d", &update_id);

    int found = 0;
    while (fscanf(file, "%d,%49[^,],%d,%29[^\n]\n",
                  &resource.id, resource.name, &resource.quantity, resource.type) == 4)
    {
        if (resource.id == update_id)
        {
            printf("Current Quantity: %d\n", resource.quantity);
            printf("Enter New Quantity: ");
            scanf("%d", &new_quantity);
            resource.quantity = new_quantity;
            found = 1;
        }
        fprintf(temp_file, "%d,%s,%d,%s\n", resource.id, resource.name,
                resource.quantity, resource.type);
    }

    fclose(file);
    fclose(temp_file);

    // Replace old file with the updated one
    remove("resources.txt");
    rename("temp.txt", "resources.txt");

    if (found)
    {
        printf("Resource quantity updated successfully!\n");
    }
    else
    {
        printf("Resource with ID %d not found.\n", update_id);
    }
}

//-----------------------------------------Start of staff Info--------------------------//

struct Staff
{
    int id;
    char name[50];
    char position[50];
};

// Function prototypes
void addStaff();
void viewStaff();
void deleteStaff(int staffID);

// Function to add a new staff member
void addStaff()
{
    struct Staff s;
    FILE *file = fopen("staff.txt", "a"); // Open file in append mode

    if (file == NULL)
    {
        printf("Error: Could not open staff.txt\n");
        return;
    }

    printf("Enter Staff ID: ");
    scanf("%d", &s.id);
    printf("Enter Staff Name: ");
    scanf(" %[^\n]", s.name); // Read until newline
    printf("Enter Position: ");
    scanf(" %[^\n]", s.position);

    // Write staff data to file
    fprintf(file, "%d,%s,%s\n", s.id, s.name, s.position);
    fclose(file);
    printf("Staff member added successfully!\n");
}

// Function to view all staff members
void viewStaff()
{
    FILE *file = fopen("staff.txt", "r"); // Open file in read mode

    if (file == NULL)
    {
        printf("Error: Could not open staff.txt\n");
        return;
    }

    struct Staff s;
    printf("\n--- List of Staff Members ---\n");
    printf("ID\tName\t\tPosition\n");

    // Read each staff member from the file and display it
    while (fscanf(file, "%d,%49[^,],%49[^\n]\n", &s.id, s.name, s.position) == 3)
    {
        printf("%d\t%-15s\t%s\n", s.id, s.name, s.position);
    }

    fclose(file);
}

// Function to delete a staff member by ID
void deleteStaff(int staffID)
{
    FILE *file = fopen("staff.txt", "r"); // Open file in read mode
    FILE *temp = fopen("temp.txt", "w");  // Temporary file for updated data

    if (file == NULL || temp == NULL)
    {
        printf("Error: Could not open file.\n");
        return;
    }

    int id, found = 0;
    char name[50], position[50];

    // Read each line and copy all except the one to delete
    while (fscanf(file, "%d,%49[^,],%49[^\n]\n", &id, name, position) == 3)
    {
        if (id == staffID)
        {
            found = 1; // Skip this line
            printf("Staff with ID %d deleted successfully.\n", staffID);
        }
        else
        {
            fprintf(temp, "%d,%s,%s\n", id, name, position); // Copy remaining data to temp file
        }
    }

    if (!found)
    {
        printf("Staff with ID %d not found.\n", staffID);
    }

    fclose(file);
    fclose(temp);

    // Replace the original file with updated temp file
    remove("staff.txt");
    rename("temp.txt", "staff.txt");
}

//------------------------Function for clear Terminal----------------------

void clearScreen()
{
#ifdef _WIN32
    system("cls"); // For Windows
#else
    system("clear"); // For Linux/macOS
#endif
}

//-------------------------------Menu Functions----------------------------//

void patient_menu()
{

    int choice2;
    while (1)
    {
        printf("\n--------Patient Menu----------\n");
        printf("\n1. Add Patient Record\n");
        printf("2. Display Patient Records\n");
        printf("3. Search Patients by ID\n");
        printf("4. Delete Patient by ID\n");
        printf("5. Back\n");
        printf("Enter your choice: ");
        scanf("%d", &choice2);

        switch (choice2)
        {
        case 1:
            add_patient();
            break;
        case 2:
            display_patients();
            break;
        case 3:
            search_patient();
            break;
        case 4:
            delete_patient();
            break;
        case 5:
            return;
        default:
            printf("Invalid choice, please try again.\n");
        }
    }
}

void doctor_menu()
{
    int choice3;

    while (1)
    {
        printf("\n--------Doctor's Menu---------\n");
        printf("1. Add Doctor\n");
        printf("2. Display Doctors\n");
        printf("3. Search Doctor by ID\n");
        printf("4. Delete Doctor by ID\n");
        printf("5. Back\n");
        printf("Enter your choice: ");
        scanf("%d", &choice3);

        switch (choice3)
        {
        case 1:
            addDoctor();
            break;

        case 2:
            viewDoctors();
            break;

        case 3:
            searchDoctor();
            break;

        case 4:
            deleteDoctor();
            break;

        case 5:
            return;

        default:
            printf("Invalid choice! Please try again.\n");
        }
    }
}

void appoint_menu()
{
    struct Queue queue;
    initializeQueue(&queue);

    int choice4, id;
    struct Appointment a;

    while (1)
    {
        printf("\n----------Appointment Menu----------\n");
        printf("1. Load Appointment\n");
        printf("2. Add Appointment (Enqueue)\n");
        printf("3. Call Next Patient (Dequeue)\n");
        printf("4. Display Appointments\n");
        printf("5. Delete Appointment by ID\n");
        printf("6. Back\n");
        printf("Enter your choice: ");
        scanf("%d", &choice4);

        switch (choice4)
        {
        case 1:
            loadAppointments(&queue);
            break;
        case 2:
            printf("Enter Patient ID: ");
            scanf("%d", &a.a_id);
            getchar(); // Clear input buffer

            printf("Enter Patient Name: ");
            fgets(a.a_name, sizeof(a.a_name), stdin);
            a.a_name[strcspn(a.a_name, "\n")] = '\0'; // Remove newline

            printf("Enter Patient Age: ");
            scanf("%d", &a.age);
            getchar(); // Clear input buffer

            printf("Enter Disease: ");
            fgets(a.disease, sizeof(a.disease), stdin);
            a.disease[strcspn(a.disease, "\n")] = '\0'; // Remove newline

            enqueue(&queue, a);
            break;

        case 3:
            dequeue(&queue);
            break;

        case 4:
            displayQueue(&queue);
            break;

        case 5:
            printf("Enter Appointment ID to delete: ");
            scanf("%d", &id);
            deleteAppointmentByID(&queue, id);
            break;

        case 6:
            return;

        default:
            printf("Invalid choice! Please try again.\n");
        }
    }
}

void staff_menu()
{

    int choice5, staffID;

    while (1)
    {
        printf("\n--- Staff Menu ---\n");
        printf("1. Add Staff\n");
        printf("2. View Staff\n");
        printf("3. Delete Staff\n");
        printf("4. Back\n");
        printf("Enter your choice: ");
        scanf("%d", &choice5);

        switch (choice5)
        {
        case 1:
            addStaff();
            break;
        case 2:
            viewStaff();
            break;
        case 3:
            printf("Enter Staff ID to delete: ");
            scanf("%d", &staffID);
            deleteStaff(staffID);
            break;
        case 4:
            return; // Back to previous
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }
}

void resource_menu()
{

    int choice1;

    while (1)
    {
        printf("\n------ Resource Menu ------\n");
        printf("1. Add Resource\n");
        printf("2. Display All Resources\n");
        printf("3. Search Resource by ID\n");
        printf("4. Update Resource Quantity\n");
        printf("5. Back\n");
        printf("Enter your choice: ");
        scanf("%d", &choice1);

        switch (choice1)
        {
        case 1:
            add_resource();
            break;
        case 2:
            display_resources();
            break;
        case 3:
            search_resource();
            break;
        case 4:
            update_resource_quantity();
            break;
        case 5:
            return;
        default:
            printf("Invalid choice! Please try again.\n");
        }
    }
}

void adminPnael()
{
    int choice1;
    char password[MAX_PASS_LEN];
    printf("Enter admin password: ");
    scanf("%s", password);

    if (strcmp(password, ADMIN_PASSWORD) != 0)
    {
        printf("Incorrect password.\n");
        return;
    }
    while (1)
    {
        printf("\n--------Welcome to Admin panel--------\n");
        printf("1. Manage Patient\n");
        printf("2. Manage Doctor\n");
        printf("3. Manage Appointment\n");
        printf("4. Manage Resource\n");
        printf("5. Manage Staff \n");
        printf("6. Back\n");
        printf("Enter your choice: ");
        scanf("%d", &choice1);

        switch (choice1)
        {
        case 1:
            clearScreen();
            patient_menu();
            break;
        case 2:
            clearScreen();
            doctor_menu();
            break;
        case 3:
            clearScreen();
            appoint_menu();
            break;
        case 4:
            clearScreen();
            resource_menu();
            break;
        case 5:
            clearScreen();
            staff_menu();
            break;
        case 6:
            return;
        default:
            printf("Invalid Choice");
        }
    }
}

//-----------------------------------Doctor Panel--------------------------------

// Structure to store doctor details
struct Doctor_
{
    int id;
    char name[50];
    char specialty[50];
    char time[50]; // Doctor's available time
    float fees;
};

// Structure to store appointment details
struct Appointment_
{
    int patientId;
    int doctorId;
    char patientName[50];
    char disease[50];
    int age;
    int isPaid;
};

// Structure to store report details
struct Report
{
    int patientId;
    char reportDetails[200];
};

// Function to load doctor details from file
void loadDoctors(struct Doctor_ doctors[], int *doctorCount)
{
    FILE *file = fopen("doctors.txt", "r");
    if (!file)
    {
        printf("Error: Could not open doctors.txt\n");
        return;
    }
    *doctorCount = 0;
    while (fscanf(file, "%d,%49[^,],%49[^,],%49[^,],%f\n",
                  &doctors[*doctorCount].id, doctors[*doctorCount].name,
                  doctors[*doctorCount].specialty, doctors[*doctorCount].time,
                  &doctors[*doctorCount].fees) != EOF)
    {
        (*doctorCount)++;
    }
    fclose(file);
}

// Function to load appointments for a doctor from file
void loadAppointments_(struct Appointment_ appointments[], int *appointmentCount, int doctorId)
{
    FILE *file = fopen("appointments.txt", "r");
    if (!file)
    {
        printf("Error: Could not open appointments.txt\n");
        return;
    }

    *appointmentCount = 0; // Initialize the count to 0

    // Read the file line by line
    while (1)
    {
        int doctorIdRead, age;
        char patientName[50], disease[50], paymentStatus[10];

        // Read a line from the file
        int result = fscanf(file, "%d,%49[^,],%d,%49[^,],%9s\n",
                            &doctorIdRead, patientName, &age, disease, paymentStatus);

        // Break the loop if we hit the end of the file or a read error
        if (result != 5)
        {
            break;
        }

        // Only store appointments for the specified doctorId
        if (doctorIdRead == doctorId)
        {
            appointments[*appointmentCount].doctorId = doctorIdRead;
            strcpy(appointments[*appointmentCount].patientName, patientName);
            appointments[*appointmentCount].age = age;
            strcpy(appointments[*appointmentCount].disease, disease);

            // Check if the payment status is "Paid" or unpaid (e.g., "0")
            if (strcmp(paymentStatus, "Paid") == 0)
            {
                appointments[*appointmentCount].isPaid = 1; // Mark as paid
            }
            else
            {
                appointments[*appointmentCount].isPaid = 0; // Mark as unpaid
            }

            (*appointmentCount)++;
        }
    }

    fclose(file); // Close the file after reading
}

// Function to create report for a patient
void createReport(int patientId)
{
    // Open the patients file to find the patient's name
    FILE *patientsFile = fopen("patients.txt", "r");
    if (!patientsFile)
    {
        printf("Error: Could not open patients.txt\n");
        return;
    }

    char patientName[50];
    int id, found = 0;
    while (fscanf(patientsFile, "%d,%49[^,],%*d,%*[^,],%*d\n", &id, patientName) == 2)
    {
        if (id == patientId)
        {
            found = 1;
            break;
        }
    }
    fclose(patientsFile);

    if (!found)
    {
        printf("Patient ID %d not found in the system.\n", patientId);
        return;
    }

    // Open the reports file to append the new report
    FILE *file = fopen("reports.txt", "a");
    if (!file)
    {
        printf("Error: Could not open reports.txt\n");
        return;
    }

    char report[300];
    printf("Enter report details for Patient ID %d (%s): ", patientId, patientName);
    getchar(); // Clear the newline left by previous input
    fgets(report, sizeof(report), stdin);

    // Remove newline character from report if present
    size_t len = strlen(report);
    if (len > 0 && report[len - 1] == '\n')
    {
        report[len - 1] = '\0';
    }

    // Write the patient ID, name, and report to the file
    fprintf(file, "%d,%s,%s\n", patientId, patientName, report);
    fclose(file);

    printf("Report created successfully for Patient ID %d (%s)\n", patientId, patientName);
}

// Function to view a patient's report
void viewReport(int patientId)
{
    // Open patients.txt to retrieve the patient's name
    FILE *patientsFile = fopen("patients.txt", "r");
    if (!patientsFile)
    {
        printf("Error: Could not open patients.txt\n");
        return;
    }

    char patientName[50];
    int id, nameFound = 0;

    // Search for the patient's name using patient ID
    while (fscanf(patientsFile, "%d,%49[^,],%*d,%*[^,],%*d\n", &id, patientName) == 2)
    {
        if (id == patientId)
        {
            nameFound = 1;
            break;
        }
    }
    fclose(patientsFile);

    if (!nameFound)
    {
        printf("Patient ID %d not found in the system.\n", patientId);
        return;
    }

    // Open reports.txt to retrieve the report
    FILE *file = fopen("reports.txt", "r");
    if (!file)
    {
        printf("Error: Could not open reports.txt\n");
        return;
    }

    char report[200];
    int found = 0;

    // Search for the report using patient ID
    while (fscanf(file, "%d,%[^\n]\n", &id, report) != EOF)
    {
        if (id == patientId)
        {
            printf("\nReport for Patient ID %d (%s):\n%s\n", patientId, patientName, report);
            found = 1;
            break;
        }
    }

    if (!found)
    {
        printf("No report found for Patient ID %d (%s)\n", patientId, patientName);
    }

    fclose(file);
}

// Function to display assigned patients for a doctor
void viewAppointedPatients(int doctorId)
{
    struct Appointment_ appointments[100]; // Temporary array to hold filtered appointments
    int appointmentCount = 0;

    // Load appointments for the given doctor ID
    loadAppointments_(appointments, &appointmentCount, doctorId);

    if (appointmentCount == 0)
    {
        printf("\nNo appointments for you at the moment.\n");
        return;
    }

    printf("\nYour Appointed Patients:\n");
    printf("----------------------------------------------------\n");
    printf("Name          | Age | Disease       | Payment Status\n");
    printf("----------------------------------------------------\n");

    for (int i = 0; i < appointmentCount; i++)
    {
        printf("%-12s | %-3d | %-12s | %-6s\n",
               appointments[i].patientName,
               appointments[i].age,
               appointments[i].disease,
               appointments[i].isPaid ? "Paid" : "Pending");
    }
}

// Doctor Panel function to interact with doctor
void doctorPanel()
{
    int doctorId;
    printf("Enter your Doctor ID: ");
    scanf("%d", &doctorId);

    struct Doctor_ doctors[100];
    int doctorCount = 0;
    loadDoctors(doctors, &doctorCount);

    // Find doctor by ID
    struct Doctor_ *doctor = NULL;
    for (int i = 0; i < doctorCount; i++)
    {
        if (doctors[i].id == doctorId)
        {
            doctor = &doctors[i];
            break;
        }
    }

    if (doctor == NULL)
    {
        printf("Invalid Doctor ID.\n");
        return;
    }

    printf("\nWelcome, Dr. %s\n", doctor->name);
    printf("Specialty: %s\n", doctor->specialty);
    printf("Available Time: %s\n", doctor->time);
    printf("Consultation Fee: %.2f\n", doctor->fees);

    int choice;
    do
    {
        printf("\n=== Doctor Panel ===\n");
        printf("1. View Appointed Patients\n");
        printf("2. Create Report\n");
        printf("3. View Patient Report\n");
        printf("4. Back\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            viewAppointedPatients(doctorId);
            break;
        case 2:
            printf("\nEnter Patient ID to create a report: ");
            int patientId;
            scanf("%d", &patientId);
            createReport(patientId);
            break;
        case 3:
            printf("\nEnter Patient ID to view their report: ");
            int patientIdToView;
            scanf("%d", &patientIdToView);
            viewReport(patientIdToView);
            break;
        case 4:
            printf("Exiting Doctor Panel.\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 4);
}

//------------------------------------USER PANEL------------------------------------------

void viewDoctors_()
{
    const char *dataFile = "doctors.txt";

    FILE *file = fopen(dataFile, "r");
    if (file == NULL)
    {
        printf("Error: Could not open %s\n", dataFile);
        return;
    }

    struct Doctor doc;

    printf("\nDoctor Information:\n");
    printf("ID\tName\t\tSpecialization\t\tTime Schedule\t\tFees\n");
    printf("-------------------------------------------------------------------------------\n");

    while (fscanf(file, "%d,%49[^,],%49[^,],%49[^,],%f\n",
                  &doc.id, doc.name, doc.specialization,
                  doc.time_schedule, &doc.fees) == 5)
    {
        printf("%d\t%s\t\t%s\t\t\t%s\t\t%.2f\n",
               doc.id, doc.name, doc.specialization, doc.time_schedule, doc.fees);
    }

    fclose(file);
}

void bookAppointment()
{
    int doctorId;
    char patientName[50], disease[50];
    int age;

    viewDoctors(); // Display available doctors
    printf("Enter the Doctor ID to book an appointment with: ");
    scanf("%d", &doctorId);

    // Collect user details
    printf("Enter your name: ");
    scanf(" %[^\n]", patientName); // Allows spaces in name
    printf("Enter your age: ");
    scanf("%d", &age);
    printf("Enter your disease: ");
    scanf(" %[^\n]", disease);

    // Save appointment to file
    FILE *file = fopen("appointments.txt", "a");
    if (file == NULL)
    {
        printf("Error: Could not open appointments.txt\n");
        return;
    }
    fprintf(file, "%d,%s,%d,%s,%d\n", doctorId, patientName, age, disease, 0); // 0 for unpaid
    fclose(file);

    printf("Appointment booked successfully. Please proceed to payment.\n");
}

// Function to generate a 4-digit OTP
int generateOTP()
{
    srand(time(NULL));           // Seed for randomness
    return rand() % 9000 + 1000; // Generates a number between 1000 and 9999
}

void makePayment()
{
    char patientName[50];
    char phoneNumber[15];
    char paymentMethod[10];

    printf("Enter your name: ");
    getchar(); // Clear newline from previous input
    fgets(patientName, sizeof(patientName), stdin);
    patientName[strcspn(patientName, "\n")] = '\0'; // Remove newline character

    printf("Enter your phone number: ");
    scanf("%s", phoneNumber);

    printf("Select Payment Method (Bkash/Nagad): ");
    scanf("%s", paymentMethod);

    // Generate and display OTP
    int otp = generateOTP();
    printf("Your OTP is: %d\n", otp);

    int userOTP;
    printf("Enter the OTP to confirm payment: ");
    scanf("%d", &userOTP);

    if (userOTP != otp)
    {
        printf("Incorrect OTP. Payment failed.\n");
        return;
    }

    // OTP verified, proceed to update payment status
    FILE *file = fopen("appointments.txt", "r");
    if (file == NULL)
    {
        printf("Error: Could not open appointments.txt\n");
        return;
    }

    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL)
    {
        printf("Error: Could not create a temporary file.\n");
        fclose(file);
        return;
    }

    int doctorId, age, found = 0;
    char name[50], disease[50], status[10];

    // Update payment status for the first unpaid record matching the name
    while (fscanf(file, "%d,%49[^,],%d,%49[^,],%9[^\n]\n", &doctorId, name, &age, disease, status) == 5)
    {
        if (found == 0 && strcmp(name, patientName) == 0 && strcmp(status, "Paid") != 0)
        {
            printf("Payment for %s is now marked as completed.\n", patientName);
            strcpy(status, "Paid");
            found = 1; // Mark that we've updated one record
        }
        fprintf(tempFile, "%d,%s,%d,%s,%s\n", doctorId, name, age, disease, status);
    }

    fclose(file);
    fclose(tempFile);

    if (found)
    {
        remove("appointments.txt");
        rename("temp.txt", "appointments.txt");
        printf("Payment record updated successfully.\n");
    }
    else
    {
        remove("temp.txt");
        printf("No unpaid appointments found for %s.\n", patientName);
    }
}

void userPanel()
{
    int choice;
    while (1)
    {
        printf("\n==== Welcome to General User Panel =====\n");
        printf("1. View Doctors\n2. Book Appointment\n3. Make Payment\n4. Back\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            clearScreen();
            viewDoctors_();
            break;
        case 2:
            clearScreen();
            bookAppointment();
            break;
        case 3:
            clearScreen();
            makePayment();
            break;
        case 4:
            clearScreen();
            return;
        default:
            printf("Invalid choice. Try again.\n");
        }
    }
}

//---------------------------------------------Patient Panel---------------------------------------

void viewPatientReport(int patientId)
{
    FILE *file = fopen("reports.txt", "r"); // Open the reports file in read mode
    if (file == NULL)
    {
        printf("Error: Could not open reports.txt\n");
        return;
    }

    int id;
    char name[50];
    char report[500]; // To store the report content
    int found = 0;

    // Read the file and look for the patient's report
    while (fscanf(file, "%d,%49[^,],%499[^\n]\n", &id, name, report) == 3) // Read ID, name, and report
    {
        if (id == patientId) // Check if the ID matches the given Patient ID
        {
            printf("\n--- Patient Report ---\n");
            printf("Patient ID: %d\n", id);
            printf("Patient Name: %s\n", name);
            printf("Report:\n%s\n", report);
            found = 1;
            break;
        }
    }

    fclose(file); // Close the file

    if (!found)
    {
        printf("No report found for Patient ID: %d\n", patientId);
    }
}

void patientPanel()
{
    int choice, id;

    while (1)
    {
        printf("\n--- Patient Panel ---\n");
        printf("1. View My Report\n");
        printf("2. Back\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("Enter your Patient ID: ");
            scanf("%d", &id);
            viewPatientReport(id);
            break;
        case 2:
            printf("Exiting Patient Panel.\n");
            return; // Exit the patient panel
        default:
            printf("Invalid choice! Please try again.\n");
        }
    }
}

// -------------------------------------Main function-------------------------------//

int main()
{
    int choice, pass;

    while (1)
    {
        clearScreen();
        printf("=================================================\n");
        printf("=  WELCOME TO ARCH HOSPITAL MANAGEMENT SYSTEM   =\n");
        printf("=================================================\n");
        printf("\n\t[1] General User Panel\n");
        printf("\t[2] Admin User Panel\n");
        printf("\t[3] Doctor Panel\n");
        printf("\t[4] Patient Panel\n");
        printf("\t[5] Exit\n");
        printf("\tEnter Choice (1-5): ");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            clearScreen();
            userPanel();
            break;
        case 2:
            clearScreen();
            printf("\n=== Welcome to Admin Panel ====\n\n");
            printf("Enter Password to access Admin Pannel...!\n\n");
            adminPnael();
            break;
        case 3:
            clearScreen();
            printf("\n=== Welcome to Doctor Panel ====\n\n");
            printf("Enter ID to access Doctor Pannel...!\n\n");
            doctorPanel();
            break;
        case 4:
            clearScreen();
            printf("\n=== Welcome to Patient Panel ====\n\n");
            patientPanel();
            break;
        case 5:
            printf("\nExiting the System...\n");
            exit(0);
            break;
        default:
            printf("Invalid Choice");
            break;
        }
    }

    return 0;
}
