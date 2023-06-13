#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>


// Defines constants for max amount of arrays and max amount of characters used
#define MAX_ARRAY 512
#define MAX_CHARACTERS 512


double fmax(double x, double y);
double fmin(double x, double y);


// Creates function that removes quotation marks from the values in the csv files
void remove_quotes(char *str)
{


 // Declares variables to be used in loops
 int i, j;


 // Creates a for loop in which i and j are initialized to 0 and the loop continues until it reaches the end of the string
 for (i = 0, j = 0; str[i] != '\0'; i++)
 {


   // Check if the current character is not a quote
   if (str[i] != '"')
   {


     // If the current character is not a quote, copy it to a new string at position j
     str[j++] = str[i];
   }
 }


 // Add a null character to the end of the new string to terminate it
 str[j] = '\0';
}




// Create a function to check if a certain parameter matches the value
bool match_parameter(const char *param, const char *value)
{


 //Check if the parameter is "All" or matches the value, if the two strings are equal strcmp returns 0
 return strcmp(param, "All") == 0 || strcmp(param, value) == 0;
}


// Create a function to get the averages when gievn a set of values
double get_avg(double values[], int count)
{


 // Declares variables used in calculation
 double sum = 0;
 double average = 0;


 // If the amount of values given is 0 the average of the set of numbers is also 0
 if (count == 0)
 { // avoid divide-by-zero error
   return average;
 }


 // A for loop in which the values are added up and set to equal sum
 for (int i = 0; i < count; i++)
 { // calculate sum
   sum += values[i];
 }


 // Determines the average of the set of numbers
 average = sum / count;


 return average;
}


// Creates a function to filter by specific location, age group and reference date
double *values(const char *location, const char *age, const char *year, int *count)
{


 // Declare variables to be used in this function
 int rowcount, concernedrowcount[18];
 char *token;


 // delimiter to disregard new lines and commas, however if a [\"] is included, the blank columns are omitted
 char delimiter[] = ",\n";  


 // need only be 1d array beacuse double array
 double numValue[MAX_ARRAY];
 double numDate[MAX_ARRAY];


 // Declares arrays used to store the date, location, age, sex and value
 char arrayDate[MAX_ARRAY][MAX_CHARACTERS], arrayGeo[MAX_ARRAY][MAX_CHARACTERS], arrayAge[MAX_ARRAY][MAX_CHARACTERS], arraySex[MAX_ARRAY][MAX_CHARACTERS], arrayValue[MAX_ARRAY][MAX_CHARACTERS];
 char row[MAX_CHARACTERS];
 double *values = NULL;
 *count = 0;
 // reading files
 FILE *file;
 file = fopen("statscan_diabetes.csv", "r");


 // ensuring that the file exists
 if (file == NULL)
 {


   printf("File does not exist\n");
   return NULL;
 }


 rowcount = 0;
 bool firstrow = true;


 // while loop that loops to end of file, more or less concerned with rows
 while (feof(file) != true)
 { // running until end of file


   fgets(row, MAX_CHARACTERS, file); // getting the string (entire row)
   if (firstrow)
   {
     firstrow = false;
     continue;
   }


   token = strtok(row, delimiter); // getting first token before loop.


   // while loop that loops each row
   int i = 0;
   while (token != NULL)
   {
     // switch statement attempting to push tokens into arrays --> part to change into the new method hopefully
     switch (i)
     {


     // This case represents the first column in the csv file which contains the reference dates
     case 0:


       // Copies the string values into its corresponding array
       strcpy(arrayDate[rowcount], token);


       // Removes the quotes from the string values in the array
       remove_quotes(arrayDate[rowcount]);
       break;


     // This case represents the second column in the csv file which contains the locations
     case 1:


        // Copies the string values into its corresponding array
       strcpy(arrayGeo[rowcount], token);


       // Removes the quotes from the string values in the array
       remove_quotes(arrayGeo[rowcount]);
       break;


     // This case represents the 4 column in the csv file which contains the age groups
     case 3:


       // Copies the string values into its corresponding array
       strcpy(arrayAge[rowcount], token);


       // Removes the quotes from the string values in the array
       remove_quotes(arrayAge[rowcount]);
       break;


     // This case represents the 5 column in the csv file which contains the sex of the population
     case 4:


       // Copies the string values into ints corresponding array
       strcpy(arraySex[rowcount], token);


       // Removes the quotes from the string values in the array
       remove_quotes(arraySex[rowcount]);
       break;


     // This case represents the 14 column in the csv file which contains the percentage value
     case 13:


       // Copies the string values into ints corresponding array
       strcpy(arrayValue[rowcount], token);


       // Removes the quotes from the string values in the array
       remove_quotes(arrayValue[rowcount]);


       // The string values in the value array are converted to double values and are stored in a new array
       numValue[rowcount] = atof(arrayValue[rowcount]);
       break;


     // Default case
     default:
       break;
     }


     // Checks if the parameters are met
     if (match_parameter(year, arrayDate[rowcount]) && match_parameter(location, arrayGeo[rowcount]) && match_parameter(age, arrayAge[rowcount]) && strlen(arrayValue[rowcount]) > 0)
     {
       (*count)++;
       values = realloc(values, sizeof(double) * (*count));
       values[(*count) - 1] = atof(arrayValue[rowcount]);
     }


     i++;


     // Always at bottom, delimiter is " ,\" " (removes commas and quotation marks), //runs till NULL
     token = strtok(NULL, delimiter);
   }


   // Only counting rows that are not solely comprised of newline
   if (strcmp(row, "\n"))
   {
     rowcount++;
   }
 }


 fclose(file); // file closing


 printf("\n"); // skips line
 return values;
}


// Main function
int main()
{
 printf("Group Number 41 CPS 188 Term Project\n------------------------------------");
 printf("\n- Ryan Wo\n- Ashwin Sundaresan\n- Genard Domingo\n- Ahmed Rajput\n\n");
 // Question 1a


 // Declares variables used to answer question 1a
 double ontarioAverage, quebecAverage, bcAverage, albertaAverage;


 int count = 0;


 // Opens two files to write data in for the GNUPlot
 FILE *outputFile;
 outputFile = fopen("output.txt", "w");
 FILE *resultsFile;
 resultsFile = fopen("result.txt", "w");
  // Prints header
 printf("(1a) Provincial Averages All Age Groups");
 printf("\n---------------------------------------");


 // Gets the averages of each province for all years and all age groups and prints them
 ontarioAverage = get_avg(values("Ontario", "All", "All", &count), count);
 printf("Ontario: %.2lf%%", ontarioAverage);


 quebecAverage = get_avg(values("Quebec", "All", "All", &count), count);
 printf("Quebec: %.2lf%%", quebecAverage);


 bcAverage = get_avg(values("British Columbia", "All", "All", &count), count);
 printf("British Columbia: %.2lf%%", bcAverage);


 albertaAverage = get_avg(values("Alberta", "All", "All", &count), count);
 printf("Alberta: %.2lf%%", albertaAverage);


 // Question 1b


 // Creeates necessary variable for question 1b
 double canadaAverage;
 // Prints header
 printf("\n\n(1b) National average all age groups");
 printf("\n------------------------------------");


 // Gets the average for Canada (excluding territories) and prints it
 canadaAverage = get_avg(values("Canada (excluding territories)", "All", "All", &count), count);
 printf("Canada: %.2lf%%", canadaAverage);


 // Question 1c


 // Creates necessary variables for question 1c
 double ontario_2015, quebec_2015, bc_2015, alberta_2015, canada_2015;
 double ontario_2016, quebec_2016, bc_2016, alberta_2016, canada_2016;
 double ontario_2017, quebec_2017, bc_2017, alberta_2017, canada_2017;
 double ontario_2018, quebec_2018, bc_2018, alberta_2018, canada_2018;
 double ontario_2019, quebec_2019, bc_2019, alberta_2019, canada_2019;
 double ontario_2020, quebec_2020, bc_2020, alberta_2020, canada_2020;
 double ontario_2021, quebec_2021, bc_2021, alberta_2021, canada_2021;


 // Prints header for averages in 2015
 printf("\n\n(1c) Provincial and National Averages All Age Groups (2015)");
 printf("\n-----------------------------------------------------------");


 // Gets the averages for each province and Canada (excluding territories) in 2015
 ontario_2015 = get_avg(values("Ontario", "All", "2015", &count), count);
 printf("Ontario: %.2lf%%", ontario_2015);


 quebec_2015 = get_avg(values("Quebec", "All", "2015", &count), count);
 printf("Quebec: %.2lf%%", quebec_2015);


 bc_2015 = get_avg(values("British Columbia", "All", "2015", &count), count);
 printf("British Columbia: %.2lf%%", bc_2015);


 alberta_2015 = get_avg(values("Alberta", "All", "2015", &count), count);
 printf("Alberta: %.2lf%%", alberta_2015);


 canada_2015 = get_avg(values("Canada (excluding territories)", "All", "2015", &count), count);
 printf("Canada: %.2lf%%", canada_2015);


 // Prints header for averages in 2016
 printf("\n\nProvincial and National Averages All Age Groups (2016)");
 printf("\n------------------------------------------------------");


 // Gets the averages for each province and Canada (excluding territories) in 2016
 ontario_2016 = get_avg(values("Ontario", "All", "2016", &count), count);
 printf("Ontario: %.2lf%%", ontario_2016);


 quebec_2016 = get_avg(values("Quebec", "All", "2016", &count), count);
 printf("Quebec: %.2lf%%", quebec_2016);


 bc_2016 = get_avg(values("British Columbia", "All", "2016", &count), count);
 printf("British Columbia: %.2lf%%", bc_2016);


 alberta_2016 = get_avg(values("Alberta", "All", "2016", &count), count);
 printf("Alberta: %.2lf%%", alberta_2016);


 canada_2016 = get_avg(values("Canada (excluding territories)", "All", "2016", &count), count);
 printf("Canada: %.2lf%%", canada_2016);


 // Prints header for averages in 2017
 printf("\n\nProvincial and National Averages All Age Groups (2017)");
 printf("\n------------------------------------------------------");


 // Gets the averages for each province and Canada (excluding territories) in 2017
 ontario_2017 = get_avg(values("Ontario", "All", "2017", &count), count);
 printf("Ontario: %.2lf%%", ontario_2017);


 quebec_2017 = get_avg(values("Quebec", "All", "2017", &count), count);
 printf("Quebec: %.2lf%%", quebec_2017);


 bc_2017 = get_avg(values("British Columbia", "All", "2017", &count), count);
 printf("British Columbia: %.2lf%%", bc_2017);


 alberta_2017 = get_avg(values("Alberta", "All", "2017", &count), count);
 printf("Alberta: %.2lf%%", alberta_2017);


 canada_2017 = get_avg(values("Canada (excluding territories)", "All", "2017", &count), count);
 printf("Canada: %.2lf%%", canada_2017);


 // Prints header for averages in 2018
 printf("\n\nProvincial and National Averages All Age Groups (2018)");
 printf("\n------------------------------------------------------");
  // Gets the averages for each province and Canada (excluding territories) in 2018
 ontario_2018 = get_avg(values("Ontario", "All", "2018", &count), count);
 printf("Ontario: %.2lf%%", ontario_2018);


 quebec_2018 = get_avg(values("Quebec", "All", "2018", &count), count);
 printf("Quebec: %.2lf%%", quebec_2018);


 bc_2018 = get_avg(values("British Columbia", "All", "2018", &count), count);
 printf("British Columbia: %.2lf%%", bc_2018);


 alberta_2018 = get_avg(values("Alberta", "All", "2018", &count), count);
 printf("Alberta: %.2lf%%", alberta_2018);


 canada_2018 = get_avg(values("Canada (excluding territories)", "All", "2018", &count), count);
 printf("Canada: %.2lf%%", canada_2018);


 // Prints header for averages in 2019
 printf("\n\nProvincial and National Averages All Age Groups (2019)");
 printf("\n------------------------------------------------------");


 // Gets the averages for each province and Canada (excluding territories) in 2019
 ontario_2019 = get_avg(values("Ontario", "All", "2019", &count), count);
 printf("Ontario: %.2lf%%", ontario_2019);


 quebec_2019 = get_avg(values("Quebec", "All", "2019", &count), count);
 printf("Quebec: %.2lf%%", quebec_2019);


 bc_2019 = get_avg(values("British Columbia", "All", "2019", &count), count);
 printf("British Columbia: %.2lf%%", bc_2019);


 alberta_2019 = get_avg(values("Alberta", "All", "2019", &count), count);
 printf("Alberta: %.2lf%%", alberta_2019);


 canada_2019 = get_avg(values("Canada (excluding territories)", "All", "2019", &count), count);
 printf("Canada: %.2lf%%", canada_2019);


 // Prints header for averages in 2020
 printf("\n\nProvincial and National Averages All Age Groups (2020)");
 printf("\n------------------------------------------------------");


 // Gets the averages for each province and Canada (excluding territories) in 2020
 ontario_2020 = get_avg(values("Ontario", "All", "2020", &count), count);
 printf("Ontario: %.2lf%%", ontario_2020);


 quebec_2020 = get_avg(values("Quebec", "All", "2020", &count), count);
 printf("Quebec: %.2lf%%", quebec_2020);


 bc_2020 = get_avg(values("British Columbia", "All", "2020", &count), count);
 printf("British Columbia: %.2lf%%", bc_2020);


 alberta_2020 = get_avg(values("Alberta", "All", "2020", &count), count);
 printf("Alberta: %.2lf%%", alberta_2020);
  canada_2020 = get_avg(values("Canada (excluding territories)", "All", "2020", &count), count);
 printf("Canada: %.2lf%%", canada_2020);


 // Prints header for averages in 2021
 printf("\n\nProvincial and National Averages All Age Groups (2021)");
 printf("\n------------------------------------------------------");


 // Gets the averages for each province and Canada (excluding territories) in 2021
 ontario_2021 = get_avg(values("Ontario", "All", "2021", &count), count);
 printf("Ontario: %.2lf%%", ontario_2021);


 quebec_2021 = get_avg(values("Quebec", "All", "2021", &count), count);
 printf("Quebec: %.2lf%%", quebec_2021);


 bc_2021 = get_avg(values("British Columbia", "All", "2021", &count), count);
 printf("British Columbia: %.2lf%%", bc_2021);


 alberta_2021 = get_avg(values("Alberta", "All", "2021", &count), count);
 printf("Alberta: %.2lf%%", alberta_2021);


 canada_2021 = get_avg(values("Canada (excluding territories)", "All", "2021", &count), count);
 printf("Canada: %.2lf%%", canada_2021);


 // Writes the header for the text file
 fprintf(outputFile, "#Year\t\tOntario\tQuebec    British Columbia   Alberta      Canada\n");


 // Writes the values contained in the textfile
 fprintf(outputFile,"2015        %.2lf       %.2lf     %.2lf               %.2lf         %.2lf\n", ontario_2015, quebec_2015, bc_2015, alberta_2015, canada_2015);
 fprintf(outputFile,"2016        %.2lf       %.2lf      %.2lf               %.2lf         %.2lf\n", ontario_2016, quebec_2016, bc_2016, alberta_2016, canada_2016);
 fprintf(outputFile,"2017        %.2lf       %.2lf      %.2lf              %.2lf        %.2lf\n", ontario_2017, quebec_2017, bc_2017, alberta_2017, canada_2017);
   fprintf(outputFile,"2018        %.2lf       %.2lf     %.2lf               %.2lf        %.2lf\n", ontario_2018, quebec_2018, bc_2018, alberta_2018, canada_2018);
   fprintf(outputFile,"2019        %.2lf       %.2lf     %.2lf              %.2lf        %.2lf\n", ontario_2019, quebec_2019, bc_2019, alberta_2019, canada_2019);
   fprintf(outputFile,"2020        %.2lf       %.2lf     %.2lf               %.2lf        %.2lf\n", ontario_2020, quebec_2020, bc_2020, alberta_2020, canada_2020);
   fprintf(outputFile,"2021        %.2lf       %.2lf     %.2lf              %.2lf         %.2lf\n", ontario_2021, quebec_2021, bc_2021, alberta_2021, canada_2021);




 // Closes the File
 fclose(outputFile);
  // Question 1d


 // Declares variables to be used in question 1d
 double ontario_35to49, quebec_35to49, bc_35to49, alberta_35to49, canada_35to49;
 double ontario_50to64, quebec_50to64, bc_50to64, alberta_50to64, canada_50to64;
 double ontario_65plus, quebec_65plus, bc_65plus, alberta_65plus, canada_65plus;


 // prints header for averages of 35 to 49 year old population
 printf("\n\n(1d) Provincial and National Averages 35 to 49 Years Old");
 printf("\n--------------------------------------------------------");


 // Gets the averages for each province and Canada (excluding territories) for population in range 35 to 49 years old
 ontario_35to49 = get_avg(values("Ontario", "35 to 49 years", "All", &count), count);
 printf("Ontario: %.2lf%%", ontario_35to49);


 quebec_35to49 = get_avg(values("Quebec", "35 to 49 years", "All", &count), count);
 printf("Quebec: %.2lf%%", quebec_35to49);


 bc_35to49 = get_avg(values("British Columbia", "35 to 49 years", "All", &count), count);
 printf("British Columbia: %.2lf%%", bc_35to49);


 alberta_35to49 = get_avg(values("Alberta", "35 to 49 years", "All", &count), count);
 printf("Alberta: %.2lf%%", alberta_35to49);


 canada_35to49 = get_avg(values("Canada (excluding territories)", "35 to 49 years", "All", &count), count);
 printf("Canada: %.2lf%%", canada_35to49);


 // Prints header for averages of 50 to 64 year old population
 printf("\n\nProvincial and National Averages 50 to 64 Years Old");
 printf("\n---------------------------------------------------");


 // Gets the averages for each province and Canada (excluding territories) for population in range 50 to 64 years old
 ontario_50to64 = get_avg(values("Ontario", "50 to 64 years", "All", &count), count);
 printf("Ontario: %.2lf%%", ontario_50to64);


 quebec_50to64 = get_avg(values("Quebec", "50 to 64 years", "All", &count), count);
 printf("Quebec: %.2lf%%", quebec_50to64);


 bc_50to64 = get_avg(values("British Columbia", "50 to 64 years", "All", &count), count);
 printf("British Columbia: %.2lf%%", bc_50to64);


 alberta_50to64 = get_avg(values("Alberta", "50 to 64 years", "All", &count), count);
 printf("Alberta: %.2lf%%", alberta_50to64);


 canada_50to64 = get_avg(values("Canada (excluding territories)", "50 to 64 years", "All", &count), count);
 printf("Canada: %.2lf%%", canada_50to64);


 // Prints header for averages of 50 to 64 year old population
 printf("\n\nProvincial and National Averages 65 and Over");
 printf("\n--------------------------------------------");


 ontario_65plus = get_avg(values("Ontario", "65 years and over", "All", &count), count);
 printf("Ontario: %.2lf%%", ontario_65plus);


 // Gets the averages for each province and Canada (excluding territories) for population in range 565 years and over
 quebec_65plus = get_avg(values("Quebec", "65 years and over", "All", &count), count);
 printf("Quebec: %.2lf%%", quebec_65plus);


 bc_65plus = get_avg(values("British Columbia", "65 years and over", "All", &count), count);
 printf("British Columbia: %.2lf%%", bc_65plus);


 alberta_65plus = get_avg(values("Alberta", "65 years and over", "All", &count), count);
 printf("Alberta: %.2lf%%", alberta_65plus);


 canada_65plus = get_avg(values("Canada (excluding territories)", "65 years and over", "All", &count), count);
 printf("Canada: %.2lf%%", canada_65plus);


 // Prints header for the file
 fprintf(resultsFile, "#AgeGroup  Percentage\n");


 // Prints the values in the file
 fprintf(resultsFile, "35-49     %.2lf\n", canada_35to49);
 fprintf(resultsFile, "50-64     %.2lf\n", canada_50to64);
 fprintf(resultsFile, "65+       %.2lf\n", canada_65plus);
  // Closes the file


 fclose(resultsFile);
 // Question 2


 // Declares variables used in question 2
 double highest, lowest;


 // Creates if statements to cycle through each set of averages to determine which oen amongst them is the highest
 // The highest average is then set to a new variable and is then printed
 if (ontarioAverage > quebecAverage && ontarioAverage > bcAverage && ontarioAverage > albertaAverage)
 {
   highest = ontarioAverage;
   printf("\n\n(2) Highest Average is Ontario with %.2lf%%", highest);
 }


 else if (quebecAverage > ontarioAverage && quebecAverage > bcAverage && quebecAverage > albertaAverage)
 {
   highest = quebecAverage;
   printf("\n\n(2) Highest Average is Quebec with %.2lf%%", highest);
 }


 else if (bcAverage > ontarioAverage && bcAverage > quebecAverage && bcAverage > albertaAverage)
 {
   highest = bcAverage;
   printf("\n\n(2) Highest Average is British Columbia with %.2lf%%", highest);
 }


 else if (albertaAverage > ontarioAverage && albertaAverage > quebecAverage && albertaAverage > bcAverage)
 {
   highest = albertaAverage;
   printf("\n\n(2) Highest Average is Alberta with %.2lf%%", highest);
 }


 // Creates if statements to cycle through each set of averages to determine which oen amongst them is the lowest
 // The lowest average is then set to a new variable and is then printed
 if (ontarioAverage < quebecAverage && ontarioAverage < bcAverage && ontarioAverage < albertaAverage)
 {
   lowest = ontarioAverage;
   printf("\nLowest average is Ontario with %.2lf%%", lowest);
 }


 else if (quebecAverage < ontarioAverage && quebecAverage < bcAverage && quebecAverage < albertaAverage)
 {
   lowest = quebecAverage;
   printf("\nLowest average is Quebec with %.2lf%%", lowest);
 }


 else if (bcAverage < ontarioAverage && bcAverage < quebecAverage && bcAverage < albertaAverage)
 {
   lowest = bcAverage;
   printf("\nLowest average is British Columbia with %.2lf%%", lowest);
 }


 else if (albertaAverage < ontarioAverage && albertaAverage < bcAverage && albertaAverage < quebecAverage)
 {
   lowest = albertaAverage;
   printf("\nLowest Average is Alberta with %.2lf%%", lowest);
 }


 // Question 3


 // Declares new arrays in which the averages and provinces are stored in
 double percentages[] = {ontarioAverage, quebecAverage, bcAverage, albertaAverage};
 char *province_names[] = {"Ontario", "Quebec", "British Columbia", "Alberta"};


 // Prints header for the averages above the national average
 printf("\n\n(3) Province(s) with diabetes percentages above the national average (%.2lf%%)\n", canadaAverage);
 printf("-----------------------------------------------------------------------------\n");


 // Creates a for loop in which the averages in the array are compared to the national average to determine which ones are higher
 for (int i = 0; i < 4; i++)
 {
   if (percentages[i] > canadaAverage)
     printf("%s (%.2lf%%)\n", province_names[i], percentages[i]);
 }


 // Prints header for the averages above the national average
 printf("\nProvince(s) with diabetes percentages below the national average (%.2lf%%)\n", canadaAverage);
 printf("-------------------------------------------------------------------------\n");
  // Creates a for loop in which the averages in the array are compared to the national average to determine which ones are lower
 for (int i = 0; i < 4; i++)
 {
   if (percentages[i] < canadaAverage)
     printf("%s (%.2lf%%)\n", province_names[i], percentages[i]);
 }


 // Question 4
 // Declares arrays to be used in question 4
double year_percentages[] = {ontario_2015, quebec_2015, bc_2015, alberta_2015, ontario_2016, quebec_2016, bc_2016, alberta_2016, ontario_2017, quebec_2017, bc_2017,
alberta_2017, ontario_2018, quebec_2018, bc_2018, alberta_2018,
ontario_2019, quebec_2019, bc_2019, alberta_2019, ontario_2020,
quebec_2020, bc_2020, alberta_2020, ontario_2021, quebec_2021, bc_2021, alberta_2021};


char *province_date[] = {"Ontario 2015", "Quebec 2015", "British Columbia 2015", "Alberta 2015", "Ontario 2016", "Quebec 2016", "British Columbia 2016", "Alberta 2016", "Ontario 2017", "Quebec 2017", "British Columbia 2017", "Alberta 2017","Ontario 2018", "Quebec 2018", "British Columbia 2018", "Alberta 2018", "Ontario 2019", "Quebec 2019", "British Columbia 2019", "Alberta 2019", "Ontario 2020", "Quebec 2020", "British Columbia 2020", "Alberta 2020", "Ontario 2021", "Quebec 2021", "British Columbia 2021", "Alberta 2021"};


// Determine the number of values in the year_percentages array
double num_values = sizeof(year_percentages) / sizeof(double);


// Find the largest and smallest diabetes percentages
double largest = 0.0;
double smallest = 100.0;
for (int i = 0; i < num_values; i++)
{
   largest = fmax(largest, year_percentages[i]);
   smallest = fmin(smallest, year_percentages[i]);
}
// Print the provinces with the highest and lowest diabetes percentages
printf("\n(4) Province(s) with highest percentage of diabetes\n");
printf("---------------------------------------------------\n");
for (int i = 0; i < num_values; i++)
{
   if (year_percentages[i] == largest)
       printf("%s (%.2lf%%)\n", province_date[i], largest);
}
printf("\nProvince(s) with lowest percentage of diabetes\n");
printf("----------------------------------------------\n");
for (int i = 0; i < num_values; i++)
{
   if (year_percentages[i] == smallest)
       printf("%s (%.2lf%%)\n", province_date[i], smallest);
}


return 0;
 }
