/*Nikos Tsianakas - 3/2/2015
"Student-Management"
Με το πρόγραμμα αυτό ο χρήστης μπορεί να κάνει εγγραφή ενός φοιτητή
ή και περισσότερων με την Επιλογή [1]. 
Έπειτα με την Επιλογή [2] μπορεί να κάνει αλλαγές σε όποιον φοιτητή επιλέξει 
και σε όποιο στοιχείο διαλέξει από το νέο μενού που εμφανίζεται. 
Η Επιλογή [3] δίνει τη δυνατότητα να προβάλει τη λίστα 
με όλους τους εγγεγραμμένους φοιτητές και να δει τα συγκεντρωτικά τους στοιχεία. 
Η Επιλογή [4] δίνει τη δυνατότητα να διαλέξει έναν φοιτητή 
και να προβάλλει αναλυτικά όλα τα στοιχεία και τη βαθμολογία του σε κάθε μάθημα. 
Τέλος, με την Επιλογή [5] παρουσιάζονται κάποια γενικά στατιστικά στοιχεία, 
ενώ για έξοδο από κάθε μενού υπάρχει η επιλογή [0].
*/


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#ifdef _MSC_VER
#define snprintf c99_snprintf

/*inline */int c99_vsnprintf(char* str, size_t size, const char* format, va_list ap)
{
	int count = -1;

	if (size != 0)
		count = _vsnprintf_s(str, size, _TRUNCATE, format, ap);
	if (count == -1)
		count = _vscprintf(format, ap);

	return count;
}

/*inline */int c99_snprintf(char* str, size_t size, const char* format, ...)
{
	int count;
	va_list ap;

	va_start(ap, format);
	count = c99_vsnprintf(str, size, format, ap);
	va_end(ap);

	return count;
}

#endif // _MSC_VER
/** Constants */
#define GRADES_LENGTH	3	// Number of courses.
#define MIN_GRADE		0	// Minimum value for grades.
#define MAX_GRADE		10	// Maximum value for grades.
#define MIN_YEAR		1900	// Minimum value for year.
#define MAX_YEAR		2100	// Maximum value for year.

#define NAME_LENGTH     50  // Max length of name string.
#define SURNAME_LENGTH  70  // Max length of surname string.
#define DATE_LENGTH     20  // Max length of a date string.
#define PERSON_LENGTH   128 // Max length of a person string.
#define INVALID_DATE    "<Invalid Date>"

enum Lessons {
	Lesson_1 = 1, Lesson_2 = 2, Lesson_3 = 3
};
/** Type Declarations. */

/** Represents a date. **/
struct Date {
	int day;
	int month;
	int year;
};

/** Represents a person. **/
typedef struct Student {

	long id;
	char *name;
	char *surname;
	struct Date birth;
	double grades[GRADES_LENGTH];
} STUDENT;

struct Choise {
	int entry;
};

typedef struct _node {
	STUDENT std;
	struct _node *next;
} node, *list;

/** Forward Declarations. */
int is_leap_year(int year);
int get_month_days(int year, int month);
int is_valid_date(struct Date date);
struct Date read_date();
STUDENT * new_record();
void create_list(list *L);
void insert_to_list(list *L, STUDENT k);
STUDENT *search_list(list L, int id);
void printReverse(list head);
int count_students(node *head);
double average_grades_perLesson(node *head, int lesson_id);
void print_total_average(node *students);
size_t date_to_string(char* string, size_t maxLength, struct Date date);
STUDENT* update_birth(STUDENT* person, struct Date date);
size_t person_to_string(char* string, size_t maxLength, const STUDENT* person);
size_t basic_info(char* string, size_t maxLength, const STUDENT* person);
double * read_grades();
STUDENT* find_student(list students);
STUDENT* update_grade(STUDENT* person, double grade, int lesson_id);
void print_average_per_student(list students);
void print_average_per_lesson(list students);
int print_update_student_menu(int entry2);
int print_main_menu(int entry);
int print_update_personal_elements_menu(int entry3);
char* update_student_name(char* newname, STUDENT* found_student);
char* update_student_surname(char* newsurname, STUDENT* found_student);
int print_update_grades_menu(int entry4, double* grade);
void calculate_statistics(list students);

int main(void) {
	long id = 0;
	double grade = 0.0;
	int entry = 0, entry2 = 0, entry3 = 0, entry4 = 0;
	char * newname = malloc(sizeof(char) * NAME_LENGTH);
	char * newsurname = malloc(sizeof(char) * SURNAME_LENGTH);
	list students; /* a list of students */

	/* make students an empty list */
	create_list(&students);

	printf("=================================================");
	printf("\n! ! ! WELCOME TO STUDENT MANAGEMENT PROGRAM ! ! !");
	printf("\n=================================================\n\n\n");

	//Select an option
	do {
		entry = print_main_menu(entry);
		if (entry == 1) {
			id++;
			//Create new student record
			STUDENT *new_student = new_record(id);
			//Insert new record to list
			insert_to_list(&students, *new_student);
			printf("\nStudent added successfully!");
			printf("\n-----------------------------------------------\n");
			printf("\nPress any key to continue...");
			getch();
			printf("\n\n\n\n");
		}

		else if (entry == 2) {
			if (students == NULL) {
				printf("\n\nNo entries found!\n");
				printf("Try option \"1\" first to enter a new student record.\n\n\n");
			}
			else
			{
				printf("\n\n----------------------------------------------------------------");
				printf("\n- - - - - - - - - - - - - MAKE CHANGES - - - - - - - - - - - - -");
				printf("\n----------------------------------------------------------------");
				printf(
					"\n\nHere is the list of students with the basic informations:\n\n");

				//Print the students list for the user to select
				printReverse(students);

				//Get student id to find and return the student
				STUDENT* found_student = find_student(students);

				if (NULL != found_student) {

					do {
						//Print the update student menu
						entry2 = print_update_student_menu(entry2);

						if (entry2 == 1) {
							//Print the update personal elements menu
							entry3 = print_update_personal_elements_menu(
								entry3);

							if (entry3 == 1) {
								//Update student name
								newname = update_student_name(newname,
									found_student);
								break;
							}
							else if (entry3 == 2) {
								//Update student surname
								newsurname = update_student_surname(newsurname,
									found_student);
								break;
							}
							else if (entry3 == 3) {
								//Update birth date
								struct Date birthdate = read_date();
								update_birth(found_student, birthdate);
								printf(
									"\n\nThe student birth date has been updated!");
								break;
							}
							else if (entry3 == 0) {
								break;
							}
							else {
								printf("\nInvalid option. Try Again:\n");
							}
						}


						else if (entry2 == 2)
						{
							//Print update grades menu
							entry4 = print_update_grades_menu(entry4, &grade);

							if (entry4 == 0)
							{
								break;
							}
							else if (entry4 == 1 || entry4 == 2 || entry4 == 3)
							{
								//Update student grades
								update_grade(found_student, grade, entry4 - 1);
								printf(
									"\n\nThe grade date has been updated!");
								break;
							}

							else
								printf("\nInvalid option. Try Again:\n");
						}

						else if (entry2 == 0)
						{
							break;
						}

						else {
							printf(
								"\nInvalid option. Select one number of the following options:\n");
						}

					} while (entry2 != 0);

				}
			}
			printf("\n----------------------------------------------------------------\n");
			printf("\nPress any key to continue...");
			getch();
			printf("\n\n\n\n");

		}
		else if (entry == 3) {
			if (students != NULL) {
				printf("\n\n--------------------------------------------------");
				printf("\n- - - - - - - - - STUDENT'S LIST - - - - - - - - -");
				printf("\n--------------------------------------------------");
				printf(
					"\n\nList of students with the basic informations:\n\n");
				printReverse(students);
			}
			else {
				printf("\nNo entries found!\n");
				printf(
					"Try option \"1\" first to enter a new student record!\n\n");
			}
			printf("--------------------------------------------------\n");
			printf("\nPress any key to continue...");
			getch();
			printf("\n\n\n\n");
		}

		else if (entry == 4) {
			printf("\n\n---------------------------------------------------------------");
			printf("\n- - - - - - - - - - - - - - DETAILS - - - - - - - - - - - - - -");
			printf("\n---------------------------------------------------------------");
			printf("\n");
			find_student(students);
			printf("---------------------------------------------------------------\n");
			printf("\nPress any key to continue...");
			getch();
			printf("\n\n\n\n");
		}

		else if (entry == 5) {
			printf("\n\n----------------------------------------------------");
			printf("\n- - - - - - - - - TOTAL STATISTICS - - - - - - - - -");
			printf("\n----------------------------------------------------");
			if (students != NULL) {
				//Calculate and print students statistics
				calculate_statistics(students);
			}
			else {
				printf("\nNo entries found!\n");
				printf(
					"Try option \"1\" first to enter a new student record!\n\n");
			}
			printf("\n----------------------------------------------------\n");
			printf("\nPress any key to continue...");
			getch();
			printf("\n\n\n\n");

		}

		else if (entry == 0) {
			printf("\n\nThank you for using Student Management Program!\n");
			printf("Have a nice day!\n\n\n");
			printf("\n\n");
			return 0;
		}
		else {
			printf(
				"\nInvalid option. Select one number of the following options: \n\n");
			printf("\nPress any key to continue...");
			getch();
			printf("\n\n\n\n");
		}

	} while (entry != 0);

	
	free(newname);
	free(newsurname);
	free(students);

	return 0;
}

STUDENT* find_student(list students) {

	char personString[PERSON_LENGTH + 1];
	long search_id;

label_menu_4:
	printf("\nPlease provide student ID: ");
	//Check if the option is right
	if (scanf("%ld", &search_id) != 1)
	{
		printf("\n!Invalid input! Try again.\n\n");
		fflush(stdin);
		goto label_menu_4;
	}

	STUDENT* found_student = search_list(students, search_id);
	if (NULL == found_student) {
		printf("\n!!!The student with ID = %ld was not found!!!\n", search_id);
		printf(
			"Remember, ID is the number of student next to \"#\"\n");
	}
	else {
		person_to_string(personString, PERSON_LENGTH, found_student);
		printf("%s\n", personString);
	}
	return found_student;
}

void printReverse(list head) {
	char personString[PERSON_LENGTH + 1];
	// Base case
	if (head == NULL)
		return;

	// print the list after head node
	printReverse(head->next);

	// After everything else is printed, print head
	basic_info(personString, PERSON_LENGTH, &head->std);
	printf("%s", personString);
}

void create_list(list *L) {
	*L = NULL;
}

/* insert a student record into the list */
void insert_to_list(list *L, STUDENT k) {
	node *p;

	/* get a free node */
	p = (node *)malloc(sizeof(node));

	/* put the record in it */
	p->std = k;

	/* link it to the first node in the list */
	p->next = *L;

	/* make the first node equal to this one
	* (so now the former first, p->next, is the second)
	*/
	*L = p;
}

STUDENT *search_list(list L, int id) {
	node *p;

	/* chase down the list until id is found, or end is reached */
	for (p = L; p && p->std.id != id; p = p->next)
		;

	/* if it's there, return a pointer to the record */
	if (p)
		return &p->std;
	else

		/* otherwise, NULL */
		return NULL;
}

int count_students(node *head) {
	int len = 0;
	while (head != NULL) {
		len++;
		head = head->next;
	}
	return len;
}

double average_grades_perLesson(node *head, int lesson_id) {
	double average_grade = 0.0;
	int student_number = count_students(head);

	while (head != NULL) {
		average_grade = average_grade + head->std.grades[lesson_id - 1];
		head = head->next;
	}
	return average_grade / student_number;

}

size_t basic_info(char* string, size_t maxLength, const STUDENT* person) {
	size_t count = 0;
	char dateString[DATE_LENGTH + 1];
	char *pString = dateString;
	double grade1 = person->grades[0];
	double grade2 = person->grades[1];
	double grade3 = person->grades[2];
	double average = (grade1 + grade2 + grade3) / 3.0;

	if (person != NULL) {
		// Produce the date string and fallback.
		if (!date_to_string(pString, DATE_LENGTH, person->birth))
			pString = INVALID_DATE;

		count =
			snprintf(string, maxLength,
			"\nStudent #%ld:\nName: %s %s\nBirth Date: %s\nAverage of all Lessons: %.2f\n",
			person->id, person->name, person->surname, pString,
			average);
		if (count >= maxLength)
			count = 0;  // 'string' was not big enough.
	}

	return count;
}

size_t person_to_string(char* string, size_t maxLength, const STUDENT* person) {
	size_t count = 0;
	char dateString[DATE_LENGTH + 1];
	char *pString = dateString;

	if (person != NULL) {
		// Produce the date string and fallback.
		if (!date_to_string(pString, DATE_LENGTH, person->birth))
			pString = INVALID_DATE;

		count =
			snprintf(string, maxLength,
			"\nStudent #%ld: %s %s\nBirth Date: %s\nGRADES-> Lesson1: %.2f, Lesson2: %.2f, Lesson3: %.2f",
			person->id, person->name, person->surname, pString,
			person->grades[0], person->grades[1],
			person->grades[2]);
		if (count >= maxLength)
			count = 0;  // 'string' was not big enough.
	}

	return count;
}

double * read_grades() {
	static double grades[GRADES_LENGTH];
	int i;
	//int grades[GRADES_LENGTH];
	for (i = 0; i < GRADES_LENGTH; i++) {
		printf("Lesson#%d: ", i + 1);
	label_menu_1_lesson:
		//Check if the option is right
		if (scanf("%lf", &grades[i]) != 1)
		{
			printf("!Invalid input! Enter again the number: ");
			fflush(stdin);
			goto label_menu_1_lesson;
		}

		if ((grades[i] < 0) || (grades[i] > 10)) {
			printf("!!!Grade out of bounds!!! Enter a value between 0-10.\n");

			i--;
		}
	}

	return grades;
}

//Struct CHOISE1
STUDENT * new_record(long id) {

	STUDENT *student;
	char * name = malloc(sizeof(char) * NAME_LENGTH);
	char * surname = malloc(sizeof(char) * SURNAME_LENGTH);
	double * grades;
	int i = 0;

	printf("\n\n-----------------------------------------------");
	printf("\n- - - - - - - - - NEW ENTRY - - - - - - - - - -");
	printf("\n-----------------------------------------------");
	printf("\nPlease enter informations of the new student:\n");
	printf("\n* Personal informations *\n");
label_menu_1_name:
	//Read name
	printf("Name: ");
	//Check if the option is right
	if (scanf("%s", name) != 1)
	{
		printf("!Invalid input! Try again.\n\n");
		fflush(stdin);
		goto label_menu_1_name;
	}
label_menu_1_surname:
	//Read surname
	printf("Surname: ");
	//Check if the option is right
	if (scanf("%s", surname) != 1)
	{
		printf("!Invalid input! Try again.\n\n");
		fflush(stdin);
		goto label_menu_1_surname;
	}
	//Read date
	struct Date birthdate = read_date();
	//Read grades
	printf("\n* Grades *\n");
	grades = read_grades();

	//Allocate memory to store the student
	student = malloc(sizeof(STUDENT));

	// If the allocation failed, display a message and exit.
	if (NULL == student) {
		printf("Could not allocate memory.\n\n");
		return 0;
	}

	student->name = name;
	student->surname = surname;
	for (i = 0; i < GRADES_LENGTH; i++) {
		student->grades[i] = *(grades + i);
	}

	student->birth = birthdate;
	student->id = id;
	return student;
}

/**
Checks if the specified year is a leap year.
- year: The year to check.
Return Value:
True (1), if 'year' is a leap year; false (0), otherwise.
**/
int is_leap_year(int year) {
	// Check if year is evenly divided by 4 and at the same time it is
	// either not evenly divisible by 100 or is also evenly divisible by 400.
	if ((year % 4 == 0) && ((year % 100 != 0) || (year % 400 == 0)))
		return 1;
	return 0;
}

/**
Gets the number of days in a given month.
- year: The year to check.
- month: The month to check.
Return Value:
The number of days in 'month' or 0 if 'month' is invalid.
**/
int get_month_days(int year, int month) {
	if (month == 2) {   // February
		if (is_leap_year(year))
			return 29;
		return 28;
	}
	if ((month == 4) || (month == 6) || (month == 9) || (month == 11))
		return 30;  // 30-Day months.
	if ((month >= 1) && (month <= 12))
		return 31;  // All other valid months.
	return 0;   // Invalid month.
}

/**
Checks if the specified date is valid.
- date: The date to check.
Return Value:
True (1), if 'date' is a valid; false (0), otherwise.
**/
int is_valid_date(struct Date date) {
	// First check if month is valid.
	if ((date.month < 1) || (date.month > 12))
		return 0;

	// Day check.
	if ((date.day < 1) || (date.day > get_month_days(date.year, date.month)))
		return 0;

	// No checks failed. Therefore its valid.
	return 1;
}

struct Date read_date() {
	struct Date date;

	// Year.
	printf("Year of birth: ");
	do {
	label_date_year:
		//Check if the option is right
		if (scanf("%d", &(date.year)) != 1)
		{
			printf("!Invalid input! Enter again an integer for year: ");
			fflush(stdin);
			goto label_date_year;
		}
		if ((date.year >= MIN_YEAR) && (date.year <= MAX_YEAR))
			break;
		printf("Invalid possible value for year! Make sure year has 4 digits: ");
	} while (1);

	// Month.
	printf("Month: ");
	do {
	label_date_month:
		//Check if the option is right
		if (scanf("%d", &(date.month)) != 1)
		{
			printf("!Invalid input! Enter again an integer for month: ");
			fflush(stdin);
			goto label_date_month;
		}
		if ((date.month >= 1) && (date.month <= 12))
			break;
		printf("Invalid value for month! Please try again for month (1-12): ");
	} while (1);

	// Day.
	printf("Day: ");
	do {
	label_date_day:
		//Check if the option is right
		if (scanf("%d", &(date.day)) != 1)
		{
			printf("!Invalid input! Enter again an integer for day: ");
			fflush(stdin);
			goto label_date_day;
		}
		if (is_valid_date(date))
			break;
		printf("Invalid value for day. Please try again: ");
	} while (1);

	return date;
}

/**
Produces a string representation (in the d/m/yyyy form) of the supplied
date. The resulting string is stored in the specified character array.
- string: A pointer to a string that will store the result.
- maxLength: The maximum number of characters to write, including the
terminating '\0'.
- date: The date to convert.
Return Value:
The number of characters written to 'string' not counting the
terminating '\0'. If the function fails (e.g. if the supplied
buffer is not large enough), it returns 0 and the contents of
'string' are undefined.
Remarks:
- Effectively the date when transformed to the d/m/yyyy string form
should be smaller than maxLenght - 1 characters.
- The function utilizes the snprintf() function which does most of
the work but is a C11 feature.
**/
size_t date_to_string(char* string, size_t maxLength, struct Date date) {
	size_t count = 0;

	if (is_valid_date(date)) {
		count = snprintf(string, maxLength, "%d/%d/%d", date.day, date.month,
			date.year);
		if (count >= maxLength)
			count = 0;  // 'string' was not big enough.
	}

	return count;
}

/********************** Person Management **********************/

/**
Updates the birth date of the specified person, to the
supplied value.
- person: The person whose birthdate should be updated.
- date: The date to which the 'person''s birth date should be updated to.
Return Value:
If all goes well, a pointer to the updated person (i.e. 'person'). If
an error ocuurs (e.g. invalid arguments), NULL.
**/
STUDENT* update_birth(STUDENT* person, struct Date date) {
	if ((person == NULL) || !is_valid_date(date))
		return NULL;
	person->birth = date;
	return person;
}

STUDENT* update_grade(STUDENT* person, double grade, int lesson_id) {
	if (person == NULL)
		return NULL;
	person->grades[lesson_id] = grade;
	return person;
}

void print_average_per_lesson(list students) {
	printf("Lesson #1: %.2f\n", average_grades_perLesson(students, 1));
	printf("Lesson #2: %.2f\n", average_grades_perLesson(students, 2));
	printf("Lesson #3: %.2f\n", average_grades_perLesson(students, 3));
}

void print_average_per_student(node *students) {

	double average = 0.0;
	int i = 0;
	if (students == NULL) {
		return;
	}

	print_average_per_student(students->next);

	for (i = 0; i < GRADES_LENGTH; i++) {
		average = average + students->std.grades[i];
	}
	printf("Average Grades for Student #%ld: %.2f\n", (students->std.id),
		average / GRADES_LENGTH);
	students = students->next;
}

void print_total_average(node *students) {

	double average_grade = 0.0;
	int student_number = count_students(students);
	int i = 0;

	while (students != NULL) {
		for (i = 0; i < GRADES_LENGTH; i++) {
			average_grade = average_grade + students->std.grades[i];
		}

		students = students->next;
	}

	printf("\nTotal average: %.2f",
		(average_grade / (student_number * GRADES_LENGTH)));

}

int print_main_menu(int entry) {
label_menu:
	printf("Select one of the following options:\n");
	printf("[1] Enter a new student record\n");
	printf("[2] Make changes to an existing entry\n");
	printf("[3] Display a list of all students\n");
	printf("[4] Display all details of a student\n");
	printf("[5] Show general statistics\n");
	printf("[0] Exit\n");
	printf("Enter your number of choise: ");
	//Check if the option is right
	if (scanf("%d", &entry) != 1)
	{
		printf("!Invalid input! Try again.\n\n");
		fflush(stdin);
		goto label_menu;
	}
	return entry;
}

int print_update_student_menu(int entry2) {
label_menu_2:
	printf("\nSelect one of the following options:\n");
	printf("[1] Update personal elements\n");
	printf("[2] Update grades\n");
	printf("[0] Exit\n");
	printf("Please select your choise: ");
	//Check if the option is right
	if (scanf("%d", &entry2) != 1)
	{
		printf("!Invalid input! Try again.\n\n");
		fflush(stdin);
		goto label_menu_2;
	}
	return entry2;
}

int print_update_personal_elements_menu(int entry3) {
label_menu_2_1:
	printf("\nSelect one of the following options:\n");
	printf("[1] Update name\n");
	printf("[2] Update surname\n");
	printf("[3] Update birth date\n");
	printf("[0] Exit\n");
	printf("Please select your choise: ");
	//Check if the option is right
	if (scanf("%d", &entry3) != 1)
	{
		printf("!Invalid input! Try again.\n\n");
		fflush(stdin);
		goto label_menu_2_1;
	}
	return entry3;
}

char* update_student_name(char* newname, STUDENT* found_student) {
label_menu_2_1_1:
	printf("\nEnter a new name: ");
	//Check if the option is right
	if (scanf("%s", newname) != 1)
	{
		printf("!Invalid input! Try again.\n\n");
		fflush(stdin);
		goto label_menu_2_1_1;
	}
	found_student->name = newname;
	printf("\n\nThe student name has been updated!");
	return newname;
}

char* update_student_surname(char* newsurname, STUDENT* found_student) {
label_menu_2_1_2:
	printf("\nEnter a new surname: ");
	//Check if the option is right
	if (scanf("%s", newsurname) != 1)
	{
		printf("!Invalid input! Try again.\n\n");
		fflush(stdin);
		goto label_menu_2_1_2;
	}
	found_student->surname = newsurname;
	printf("\n\nThe student surname has been updated!");
	return newsurname;
}

int print_update_grades_menu(int entry4, double* grade) {
label_menu_2_2:
	printf("\n[1] Update Lesson1 grade \n");
	printf("[2] Update Lesson2 grade \n");
	printf("[3] Update Lesson3 grade \n");
	printf("[0] Exit\n");
	printf("Please enter the ID of the lesson: ");
	//Check if the option is right
	if (scanf("%d", &entry4) != 1)
	{
		printf("!Invalid input! Try again:\n\n");
		fflush(stdin);
		goto label_menu_2_2;
	}

	if (entry4 == 1 || entry4 == 2 || entry4 == 3)
	{
	label_grade:
		printf("\nEnter the new grade for Lesson%d: ", entry4);
		//Check if the option is right
		if (scanf("%lf", &*grade) != 1)
		{
			printf("!Invalid input! Try again:\n\n");
			fflush(stdin);
			goto label_menu_2_2;
		}
		else if ((*grade < 1) || (*grade > 10))
		{
			printf("Invalid value! Grades must be between 1-10.\n\n");
			goto label_grade;
		}
	}

	return entry4;
}

void calculate_statistics(list students) {
	printf("\nNumber of recorded students: %d", count_students(students));
	printf("\n\n* Averages per Lesson *\n");
	print_average_per_lesson(students);
	printf("\n* Averages per Student *\n");
	print_average_per_student(students);
	print_total_average(students);
}
