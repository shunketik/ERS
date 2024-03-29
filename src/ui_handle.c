#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<conio.h>
#include<time.h>

#include "../include/ui_handle.h"
#include "../include/file_handle.h"

/*
	*private functions
	*used by this file ui_menu.c only 
*/

static int IsEqualStr(char str1[], char str2[])
{
	if(strlen(str1) != strlen(str2))
	{
		return 0;
	}
	
	int i;
	for(i = 0; i < str1[i] != '\0'; ++i)
	{
		if(str1[i] != str2[i])
		{
			return 0;
		}	
	}
	
	return 1;
}

int ReadStr(char *input)
{
	int i;	
	for(i = 0; i < 1000; ++i)
	{
		scanf("%c", &input[i]);
		
		//if user hits enter, breaks the loop
		if(input[i] == 10)
		{
			break;
		}
	}
	input[i] = '\0';
	
	//if user types --mm it takes the user to main menu instatntly  
	if(IsEqualStr(input, "--mm"))
	{
		return 0;
	}
	
	return 1;
}

int ReadChar(char *ch)
{
	*ch = getch();
	
	//if user press ctrl+Z, it will take the user to main menu instantly
	if(*ch == 26)
	{
		return 0;
	}
	
	return 1;
}

int ToInt(char digitChar)
{
	return digitChar - '0';
}

int IsDouble(char str[])
{
	int numberOfDecimalPoints = 0;
	
	if(strlen(str) < 1) 
	{
		return 0;
	}
	
	int i;
	for(i = 0; str[i] != '\0'; i++)
	{
		if(!isdigit(str[i]) && str[i] != '.')
		{
			return 0;
		}
		else if(str[i] == '.')
		{
			if(++numberOfDecimalPoints > 1)
			{
				return 0;
			}		
		}
	}
	
	return 1;
}

int IsLeapYear(int year)
{
	if(year % 4 == 0)
	{
		if(year % 100 == 0)
		{
			if(year % 400 == 0)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 1;
		}
	}
	else
	{
		return 0;
	}
}

int IsValidDate(char dateStr[])
{		
	//checks for dd/mm/yyyy format
	if(dateStr[2] != '/' || dateStr[5] != '/' || strlen(dateStr) != 10)
	{
		return 0;
	}
	
	//checks for non-digit char
	int i;
	for(i = 0; i < 10; i++)
	{
		if(i != 2 && i != 5)//char in index 2 and 5 is '/'
		{
			if(!isdigit(dateStr[i]))
			{
				return 0;
			}
		}
	}
	
	//splits the argument dateStr into day, month and year and convert to equivalent integer
	int day = (ToInt(dateStr[0]) * 10) + ToInt(dateStr[1]);
	int month = (ToInt(dateStr[3]) * 10) + ToInt(dateStr[4]);
	int year = (ToInt(dateStr[6]) * 1000) 
			+ (ToInt(dateStr[7]) * 100)
			+ (ToInt(dateStr[8]) * 10) 
			+ ToInt(dateStr[9]);
			
	//gets current year
	char currentYearStr[5];
	time_t timer;
	struct tm* time_info;
	time(&timer);
	time_info = localtime(&timer);
	strftime(currentYearStr, 5, "%Y", time_info);
	int currentYear = (ToInt(currentYearStr[0]) * 1000) 
					+ (ToInt(currentYearStr[1]) * 100)
					+ (ToInt(currentYearStr[2]) * 10) 
					+ ToInt(currentYearStr[3]);

	//checks for other invalid cases
	if(year > currentYear || month > 12 || month < 1 || day > 31 || day < 1)
	{
		return 0;
	}
	else if(month == 2)
	{
		if((IsLeapYear(year) && day > 29) || (!IsLeapYear(year) && day > 28))
		{
			return 0;
		}
	}
	else if(month % 2 == 0 && month != 8 && day > 30)
	{
		return 0;
	}
	
	return 1;
}


/*
	*public functions
	*used by any file that declares ui_handle.h as header
*/

char UI_OnMainMenu()
{
	system("cls");//clears the screen
	
	printf("Enter the corresponding number to open that option:\n\n");
	printf("\t1 -> View Current Record\n");
	printf("\t2 -> Add New Record\n");
	printf("\t3 -> Edit Record\n");
	printf("\t4 -> Delete Record\n");
	printf("\t5 -> Exit\n");
	
	char userInput = getch();
	
	return userInput;
}

void UI_ShowRecord()
{
	system("cls");
	
	if(File_ExistFile("records.bin"))
	{
		if(File_ReadAll())
		{
			printf("\nYou have reached the  end of data. Press any key to go back to main menu.");		
			getch();
		}
	}
	else
	{
		UI_ShowErrorMsg("No record found.");
	}
}

void UI_AddRecord()
{	
	struct EmployeeData employee;
	
	char userInput;
	
	int continueAdding = 1;
	
	while(continueAdding)
	{
		system("cls");
		
		printf("Enter the employee information:\n\n");
		printf("\tName: ");		
		if(!ReadStr(employee.name)) 
		{
			break;
		}
		
		while(continueAdding)
		{
			printf("\tID: ");	
			if(ReadStr(employee.id))
			{
				if(File_ExistRecord(employee.id))
				{
					UI_ShowErrorMsg("This ID already belongs to another employee. Please provide an unique ID.");
				}
				else
				{
					break;
				}
			}
			else
			{
				continueAdding = 0;
			}
		}
		
		if(continueAdding)
		{
			printf("\tPosition: ");			
			if(!ReadStr(employee.position))
			{
				break;
			}
		}
		
		while(continueAdding)
		{
			printf("\tJoining date(dd/mm/yyyy): ");		
			if(ReadStr(employee.joiningDate))
			{
				if(IsValidDate(employee.joiningDate))
				{
					break;
				}
				else
				{
					UI_ShowErrorMsg("The date is not valid. Please try again");
				}
			}
			else
			{
				continueAdding = 0;
			}
		}
			
		while(continueAdding)
		{			
			printf("\tChoose status:\n");
			printf("\t\t1 -> Current Employee\n");
			printf("\t\t2 -> Previous Employee\n");			
			if(ReadChar(&userInput))
			{
				if(userInput == '1')
				{
					strcpy(employee.status, "Current employee");			
					strcpy(employee.resigningDate, "N/A");				
					printf("\tStatus: Current employee\n");			
					break;
				}
				else if(userInput == '2')
				{
					strcpy(employee.status, "Previous employee");
					printf("\tStatus: Previous employee\n");
						
					while(continueAdding)
					{
						printf("\tResgining date(dd/mm/yyyy): ");
						if(ReadStr(employee.resigningDate))
						{
							if(IsValidDate(employee.resigningDate))
							{
								break;
							}
							else
							{
								UI_ShowErrorMsg("The date is not valid. Please try again.");
							}
						}
						else
						{
							continueAdding = 0;
						}
					}
					
					break;
				}
				else
				{
					UI_ShowErrorMsg("Invalid input. Please try again.");
				}			
			}
			else
			{
				continueAdding = 0;
			}			
		}
		
		while(continueAdding)
		{
			printf("\tSalary ($): ");	
			if(ReadStr(employee.salary)) 
			{
				if(IsDouble(employee.salary))
				{
					break;
				}
				else
				{
					UI_ShowErrorMsg("Invalid salary. Please try again.");
				}
			}
			else
			{
				continueAdding = 0;
			}
		}
		
		if(continueAdding)
		{
			int confirmed = UI_UserConfirmed("save_record");
			
			if(confirmed)
			{
				if(File_Append(employee))
				{
					UI_ShowSuccessMsg("Employee information is successfully saved.");
				}
				else
				{
					UI_ShowErrorMsg("Something went wrong. Record could not be saved.");
				}
			}
			else if(!confirmed)
			{
				printf("Attention: The employee information was not saved.");
			}
			else
			{
				continueAdding = 0;
			}
		}

		while(continueAdding)
		{
			if(UI_UserConfirmed("add_another_record"))
			{
				break;
			}
			else
			{
				continueAdding = 0;
			}
		}
	}
}

void UI_EditRecord()
{
	system("cls");
	
	struct EmployeeData employee;
	
	char employeeId[1001];
	char userInput;
	
	int continueEdit = 1;
	int allInfoCollected = 0;
	
	while(continueEdit)
	{	
		printf("Enter the ID of the employee whose record you want to edit: ");
		
		if(ReadStr(employeeId))
		{
			if(File_ExistRecord(employeeId) && File_ReadOne(employeeId))
			{				
				if(UI_UserConfirmed("edit_record"))
				{
					printf("Enter new information for this employee: \n");
					printf("(Hint: type [same] id you do not want to change a field)\n");
					printf("\tName: ");	
					if(!ReadStr(employee.name)) 
					{
						break;
					}
							
					while(continueEdit)
					{
						printf("\tEmployee ID: ");
						if(ReadStr(employee.id))
						{
							if(!IsEqualStr(employee.id, "same") && File_ExistRecord(employee.id))
							{
								UI_ShowErrorMsg("The ID already belongs to another employee. Please provide an unique ID.");
							}
							else
							{
								break;
							}
						}
						else
						{
							continueEdit = 0;
						}
					}
							
					printf("\tPosition: ");
					if(!ReadStr(employee.position))
					{
						continueEdit = 0;
					}
							
					while(continueEdit)
					{
						printf("\tJoining date(dd/mm/yyyy): ");
						if(ReadStr(employee.joiningDate))
						{
							if(IsEqualStr(employee.joiningDate, "same")|| IsValidDate(employee.joiningDate))
							{
								break;
							}
							else
							{
								UI_ShowErrorMsg("The date is not valid. Please try again.");
							}
						}
						else
						{
							continueEdit = 0;
						}
					}
								
					while(continueEdit)
					{	
						printf("\tChoose status:\n");
						printf("\t\t1 -> Current Employee\n");
						printf("\t\t2 -> Previous Employee\n");									
						if(ReadChar(&userInput))
						{
							if(userInput == '1')
							{
								strcpy(employee.status, "Current employee");
								strcpy(employee.resigningDate, "N/A");
								printf("\tStatus: Current employee\n");										
								break;
							}
							else if(userInput == '2')
							{
								strcpy(employee.status, "Previous employee");
								printf("\tStatus: Previous employee\n");
											
								while(continueEdit)
								{
									printf("\tResgining date(dd/mm/yyyy): ");
									if(ReadStr(employee.resigningDate))
									{
										if(IsEqualStr(employee.resigningDate, "same")|| IsValidDate(employee.resigningDate))
										{
											break;
										}
										else
										{
											UI_ShowErrorMsg("The date is not valid. Please try again.");
										}
									}
									else
									{
										continueEdit = 0;
									}
								}
											
								break;
							}
							else
							{
								UI_ShowErrorMsg("Invalid input. Please try again.");
							}
						}
						else
						{
							continueEdit = 0;
						}			
					}
						
					while(continueEdit)
					{
						printf("\tSalary ($): ");
						if(ReadStr(employee.salary)) 
						{
							if(IsEqualStr(employee.salary, "same") || IsDouble(employee.salary))
							{
								allInfoCollected = 1;
								break;
							}
							else
							{
								UI_ShowErrorMsg("Invalid salary. Please try again.");
							}
						}
						else
						{
							continueEdit = 0;
						}
					}
						
					if(allInfoCollected)
					{
						if(File_Update(employeeId, employee))
						{
							UI_ShowSuccessMsg("The record has been successfully updated.");
						}
						else
						{
							UI_ShowErrorMsg("Sorry, could not update the information.");
						}
					}
				}
				else
				{
					printf("The employee information was not updated.\n\n");
				}
					
				if(UI_UserConfirmed("edit_another_record"))
				{
					system("cls");
				}
				else
				{
					break;
				}			
			}
			else
			{
				char message[1001] = "No employee found with ID ";
				strcat(message, employeeId);
				UI_ShowErrorMsg(message);
			}
		}
		else
		{
			break;
		}
	}
}

void UI_DeleteRecord()
{
	char employeeId[1001];
	
	int continueDelete = 1;
	
	system("cls");
	
	while(continueDelete)
	{	
		printf("Enter the ID of the employee whose record you want to delete: ");
		
		if(ReadStr(employeeId))
		{
			if(File_ExistRecord(employeeId) && File_ReadOne(employeeId))
			{				
				if(UI_UserConfirmed("delete_record"))
				{
					if(File_DeleteRecord(employeeId))
					{
						UI_ShowSuccessMsg("The record is permanently deleted");
					}
					else
					{
						UI_ShowErrorMsg("Something went wrong. Could not delete the file");
					}
				}
				else
				{
					printf("The employee information was not changed.\n\n");
				}
					
				if(UI_UserConfirmed("delete_another_record"))
				{
					system("cls");
				}
				else
				{
					break;
				}			
			}
			else
			{
				char message[1001] = "No employee found with ID ";
				strcat(message, employeeId);
				UI_ShowErrorMsg(message);
			}
		}
		else
		{
			break;
		}
	}
}

int UI_UserConfirmed(char confirmFor[])
{	
	char userInput;
	char message[100];
	if(IsEqualStr(confirmFor, "exit"))
	{			
		strcpy(message, "Are you sure you want to exit?\n");
	}
	else if(IsEqualStr(confirmFor, "save_record"))
	{	
		strcpy(message, "Do you want to save the record?\n");		
	}
	else if(IsEqualStr(confirmFor, "add_another_record"))
	{	
		strcpy(message, "Do you want to add another record?\n");
	}
	else if(IsEqualStr(confirmFor, "edit_record"))
	{	
		strcpy(message, "Do you want to edit this record?\n");
	}
	else if(IsEqualStr(confirmFor, "edit_another_record"))
	{	
		strcpy(message, "Do you want to edit another record?\n");
	}
	else if(IsEqualStr(confirmFor, "delete_record"))
	{
		strcpy(message, "Do you want to delete this record permanently?\n");
	}
	else if(IsEqualStr(confirmFor, "delete_another_record"))
	{
		strcpy(message, "Do you want to delete another record?\n");
	}

	while(1)
	{	
		printf("\t%s", message);
		printf("\t\t1 -> Yes\n");
		printf("\t\t2 -> No\n");
		
		if(ReadChar(&userInput))
		{
			if(userInput == '1')
			{				
				return 1;
			}
			else if(userInput == '2')
			{
				return 0;
			}
			else
			{
				UI_ShowErrorMsg("Invalid input. Please try again.");
			}	
		}
		else 
		{
			return 0;		
		}
	}
}

int UI_Display(struct UI_EmployeeData employee, int employeeNum)
{
	//we want to show no more than 3 employee data at a time
	//so we will clear the screen after every 3 employee data is shown
	if(employeeNum % 3 == 1)
	{
		system("cls");
	}
	
	if(employeeNum > -1) printf("Employee Number: %d\n", employeeNum);
	printf("\tName          : %s\n", employee.name);
	printf("\tID            : %s\n", employee.id);
	printf("\tPosition      : %s\n", employee.position);
	printf("\tJoining Date  : %s\n", employee.joiningDate);
	printf("\tStatus        : %s\n", employee.status);
	printf("\tResigning Date: %s\n", employee.resigningDate);
	printf("\tSalary        : %s\n", employee.salary);
	printf("________________________________________________\n\n");

	
	if(employeeNum % 3 == 0)
	{
		char input;
		
		printf("Press any key to load more or [ctrl]+[z} to go back to main manu.");		
		if(!ReadChar(&input))
		{
			return 0;
		}
	}
	
	return 1;
}

void UI_ShowSuccessMsg(char message[])
{
	printf("\033[0;32m");//set text color to green
	printf("%s\n\n", message);
	printf("\033[0m");//set text color to default
}

void UI_ShowErrorMsg(char message[])
{
	printf("\033[0;31m");//set text color to red
	printf("Alert: %s\n\n", message);	
	printf("\033[0m");//set text color to default
}
