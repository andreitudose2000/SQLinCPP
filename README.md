# SQLinCPP
C++ OOP code that imitates basic SQL DDL, DML and DQL functionality.


This code creates a console application where the user is told the available commands and he can write SQL-like commands. The algorithms will interpret them and execute the appropiate modifications to the created database. The database is stored in the computer when the user exits the console application and loaded when he starts it back again.
The code is written C++ and uses its features: OOP concepts, STL containers, I/O, I/O with files.
The application uses the Romanian language for its user interface.

**Available commands:**
*(note: keywords in square brackets are optional syntax)*

> CREATE TABLE table_name [DACA NU EXISTA] ((col_name_1, col_type, size, default_value), (col_name_2, col_type, size, default_value), ... (col_name_n, col_type, size, default_value))
col_type can be: integer, float, text
*"DACA NU EXISTA" translates to "if it doesn't exist" in Romanian*

> DROP TABLE table_name

> DISPLAY TABLE table_name

> INSERT INTO table_name VALUES (value_1, value_2, ... value_n)

> DELETE FROM table_name WHERE col_name = input_value

> SELECT (col_name_1, col_name_2, ... col_name_n) FROM table_name [WHERE col_name = input_value]
> SELECT ALL FROM table_name [WHERE col_name = input_value]

> UPDATE table_name SET col_name = input_value WHERE col_name = input_value 

> IMPORT table_name file_name.CSV

> EXIT 



**Data storing functionalities:**
* Information about the created tables are retained in a text file that is updated at the end of the program. This file is automatically loaded at the program startup and the tables are created in memory.
* At the end of the program, the records inserted in each of the tables are written to a corresponding binary (.dat) file. These files are automatically loaded at the program startup and the records are inserted in the corresponding tables.
  
**Other functionalities:**
* The user can import data from CSV files, as an equivalent to manually inserting them using the INSERT command. The default CSV separator is the comma character.
* The application can be run with command line args representing the names of text files which contain SQL commands (those listed in the **Available commands** section), with a max number of 5 files. The program will interpret them as if they were written by the user. 
  Example: `ProiectPOO.exe command_file_1.txt command_file_2.txt`
* Each SELECT or DISPLAY command generates a file automatically numbered (SELECT_1, SELECT_2, ..., DISPLAY_1, DISPLAY_2, ...) in which the result of the command is written.
