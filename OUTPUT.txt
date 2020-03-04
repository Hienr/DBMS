//---------------------------------------------------------------------
// INITIAL RUN OF SAMPLE.TXT :
//---------------------------------------------------------------------
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
                   Relational Database System
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 * Implementation of a database that will respond to SQL-like
 * commands. They include :
 * >> CREATE|MAKE TABLE <TABLENAME> FIELDS <FIELDNAME(S) . . .>
 * >> INSERT INTO <TABLE NAME> VALUES <VALUE(S) . . . >
 * >> SELECT *|<FIELD(S) . . .> FROM <TABLENAME> WHERE <CONDITION>

 * <CONDITION> include : <FIELDNAME> <RELATIONAL_OPERATOR> <VALUE>
 * chain conditions together with a <LOGICAL OPERATOR>
 * <RELATIONAL OPERATOR> : = < > <= >=
 * <LOGICAL OPERATOR> : AND OR
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Command : batch sample.txt
//****************************************************************************
//              CREATE AND INSERT
//****************************************************************************

//---- employee table ----------
[1] : make table employee fields  last,                 first,                  dep

---------------------------------------------------------------------------
          last         first           dep
---------------------------------------------------------------------------

---------------------------------------------------------------------------


[2] : insert into employee values Blow,                 Joe,                    CS

---------------------------------------------------------------------------
          last         first           dep
---------------------------------------------------------------------------
    1.         Blow          Joe           CS

---------------------------------------------------------------------------


[3] : insert into employee values Johnson,      "Jimmy",                Chemistry

---------------------------------------------------------------------------
          last         first           dep
---------------------------------------------------------------------------
    1.         Blow          Joe           CS
    2.      Johnson        Jimmy    Chemistry

---------------------------------------------------------------------------


[4] : insert into employee values Yang,                 Bo,                     CS

---------------------------------------------------------------------------
          last         first           dep
---------------------------------------------------------------------------
    1.         Blow          Joe           CS
    2.      Johnson        Jimmy    Chemistry
    3.         Yang           Bo           CS

---------------------------------------------------------------------------


[5] : insert into employee values "Jackson",    Billy,                  Math

---------------------------------------------------------------------------
          last         first           dep
---------------------------------------------------------------------------
    1.         Blow          Joe           CS
    2.      Johnson        Jimmy    Chemistry
    3.         Yang           Bo           CS
    4.      Jackson        Billy         Math

---------------------------------------------------------------------------


[6] : insert into employee values Johnson,      Billy,                  "Phys Ed"

---------------------------------------------------------------------------
          last         first           dep
---------------------------------------------------------------------------
    1.         Blow          Joe           CS
    2.      Johnson        Jimmy    Chemistry
    3.         Yang           Bo           CS
    4.      Jackson        Billy         Math
    5.      Johnson        Billy      Phys Ed

---------------------------------------------------------------------------


[7] : insert into employee values "Van Gogh",   "Jim Bob",              "Phys Ed"

---------------------------------------------------------------------------
          last         first           dep
---------------------------------------------------------------------------
    1.         Blow          Joe           CS
    2.      Johnson        Jimmy    Chemistry
    3.         Yang           Bo           CS
    4.      Jackson        Billy         Math
    5.      Johnson        Billy      Phys Ed
    6.     Van Gogh      Jim Bob      Phys Ed

---------------------------------------------------------------------------



[8] : select * from employee

---------------------------------------------------------------------------
          last         first           dep
---------------------------------------------------------------------------
    1.         Blow          Joe           CS
    2.      Johnson        Jimmy    Chemistry
    3.         Yang           Bo           CS
    4.      Jackson        Billy         Math
    5.      Johnson        Billy      Phys Ed
    6.     Van Gogh      Jim Bob      Phys Ed

---------------------------------------------------------------------------



//----- student table ----------
[9] : make table student fields         fname,                  lname,          major,                     age

---------------------------------------------------------------------------
         fname         lname         major           age
---------------------------------------------------------------------------

---------------------------------------------------------------------------


[10] : insert into student values       Flo,                    Yao,            CS,                         20

---------------------------------------------------------------------------
         fname         lname         major           age
---------------------------------------------------------------------------
    1.          Flo          Yao           CS           20

---------------------------------------------------------------------------


[11] : insert into student values       "Flo",                  "Jackson",      Math,                       21

---------------------------------------------------------------------------
         fname         lname         major           age
---------------------------------------------------------------------------
    1.          Flo          Yao           CS           20
    2.          Flo      Jackson         Math           21

---------------------------------------------------------------------------


[12] : insert into student values       Calvin,                 Woo,            Physics,                    22

---------------------------------------------------------------------------
         fname         lname         major           age
---------------------------------------------------------------------------
    1.          Flo          Yao           CS           20
    2.          Flo      Jackson         Math           21
    3.       Calvin          Woo      Physics           22

---------------------------------------------------------------------------


[13] : insert into student values       "Anna Grace",   "Del Rio",      CS,                                 22

---------------------------------------------------------------------------
         fname         lname         major           age
---------------------------------------------------------------------------
    1.          Flo          Yao           CS           20
    2.          Flo      Jackson         Math           21
    3.       Calvin          Woo      Physics           22
    4.   Anna Grace      Del Rio           CS           22

---------------------------------------------------------------------------


[14] : select * from student

---------------------------------------------------------------------------
         fname         lname         major           age
---------------------------------------------------------------------------
    1.          Flo          Yao           CS           20
    2.          Flo      Jackson         Math           21
    3.       Calvin          Woo      Physics           22
    4.   Anna Grace      Del Rio           CS           22

---------------------------------------------------------------------------





//****************************************************************************
//              SIMPLE SELECT
//****************************************************************************

[15] : select * from student

---------------------------------------------------------------------------
         fname         lname         major           age
---------------------------------------------------------------------------
    1.          Flo          Yao           CS           20
    2.          Flo      Jackson         Math           21
    3.       Calvin          Woo      Physics           22
    4.   Anna Grace      Del Rio           CS           22

---------------------------------------------------------------------------



//------- simple strings -------------------
[16] : select * from student where lname = Jackson

---------------------------------------------------------------------------
         fname         lname         major           age
---------------------------------------------------------------------------
    1.          Flo      Jackson         Math           21

---------------------------------------------------------------------------



//----- quoted strings ---------------------
[17] : select * from student where lname = "Del Rio"

---------------------------------------------------------------------------
         fname         lname         major           age
---------------------------------------------------------------------------
    1.   Anna Grace      Del Rio           CS           22

---------------------------------------------------------------------------



//-------- non-existing string ------------------
[18] : select * from student where lname = "Does Not Exist"

---------------------------------------------------------------------------
         fname         lname         major           age
---------------------------------------------------------------------------

---------------------------------------------------------------------------



//****************************************************************************
//              RELATIONAL OPERATORS:
//****************************************************************************

//.................
//:Greater Than   :
//.................
[19] : select * from student where lname > Yang

---------------------------------------------------------------------------
         fname         lname         major           age
---------------------------------------------------------------------------
    1.          Flo          Yao           CS           20

---------------------------------------------------------------------------




//. . . . . . . . . . . . . (Greater Than: Non-existing) . . . . . . . . . . . . . . . . . . . . .
[20] : select * from student where age > 50

---------------------------------------------------------------------------
         fname         lname         major           age
---------------------------------------------------------------------------

---------------------------------------------------------------------------



//. . . . . . . . . . . . . (Greater than: last item) . . . . . . . . . . . . . . . . . .
[21] : select * from student where age  > 53

---------------------------------------------------------------------------
         fname         lname         major           age
---------------------------------------------------------------------------

---------------------------------------------------------------------------



//. . . . . . . . . . . . . (Greater Than: Passed last item) . . . . . . . . . . . . . . . . . . . . .
[22] : select * from student where age > 54

---------------------------------------------------------------------------
         fname         lname         major           age
---------------------------------------------------------------------------

---------------------------------------------------------------------------



//.................
//:Greater Equal  :
//.................
[23] : select * from student where lname >= Yang

---------------------------------------------------------------------------
         fname         lname         major           age
---------------------------------------------------------------------------
    1.          Flo          Yao           CS           20

---------------------------------------------------------------------------


//. . . . . .  (Greater Equal non-existing: ) . . . . . . . . . . .
[24] : select * from employee where last >= Jack

---------------------------------------------------------------------------
          last         first           dep
---------------------------------------------------------------------------
    1.      Johnson        Jimmy    Chemistry
    2.         Yang           Bo           CS
    3.      Jackson        Billy         Math
    4.      Johnson        Billy      Phys Ed
    5.     Van Gogh      Jim Bob      Phys Ed

---------------------------------------------------------------------------




//.................
//:Less Than      :
//.................


//. . . . . . . . . . . . . (Less Than: Non-existing) . . . . . . . . . . . . . . . . . . . . .
[25] : select * from student where lname < Jackson

---------------------------------------------------------------------------
         fname         lname         major           age
---------------------------------------------------------------------------
    1.   Anna Grace      Del Rio           CS           22

---------------------------------------------------------------------------



//. . . . . . . . . . . . . (Less than: first item) . . . . . . . . . . . . . . . . . .
[26] : select * from student where age  < 20

---------------------------------------------------------------------------
         fname         lname         major           age
---------------------------------------------------------------------------

---------------------------------------------------------------------------




//. . . . . . . . . . . . . (Less Than: before first item) . . . . . . . . . . . . . . . . . . . . .

[27] : select * from student where age  < 19

---------------------------------------------------------------------------
         fname         lname         major           age
---------------------------------------------------------------------------

---------------------------------------------------------------------------




//.................
//:Less Equal     :
//.................

[28] : select * from student where lname <= Smith

---------------------------------------------------------------------------
         fname         lname         major           age
---------------------------------------------------------------------------
    1.          Flo      Jackson         Math           21
    2.   Anna Grace      Del Rio           CS           22

---------------------------------------------------------------------------



//. . . . . .  (Less Equal non-existing: ) . . . . . . . . . . .
[29] : select * from employee where last <= Peach

---------------------------------------------------------------------------
          last         first           dep
---------------------------------------------------------------------------
    1.         Blow          Joe           CS
    2.      Johnson        Jimmy    Chemistry
    3.      Jackson        Billy         Math
    4.      Johnson        Billy      Phys Ed

---------------------------------------------------------------------------



//****************************************************************************
//              LOGICAL OPERATORS
//****************************************************************************


//.................
//:AND            :
//.................

[30] : select * from student where fname = "Flo" and lname = "Yao"

---------------------------------------------------------------------------
         fname         lname         major           age
---------------------------------------------------------------------------
    1.          Flo          Yao           CS           20

---------------------------------------------------------------------------




//.................
//:OR            :
//.................
[31] : select * from student where fname = Flo or lname = Jackson

---------------------------------------------------------------------------
         fname         lname         major           age
---------------------------------------------------------------------------
    1.          Flo          Yao           CS           20
    2.          Flo      Jackson         Math           21

---------------------------------------------------------------------------




//.................
//:OR AND         :
//.................
[32] : select * from student where fname = Flo or major = CS and age <= 23

---------------------------------------------------------------------------
         fname         lname         major           age
---------------------------------------------------------------------------
    1.          Flo          Yao           CS           20
    2.          Flo      Jackson         Math           21
    3.   Anna Grace      Del Rio           CS           22

---------------------------------------------------------------------------




//.................
//:AND OR AND     :
//.................

[33] : select * from student where age <30 and major=CS or major = Physics and lname = Jackson

---------------------------------------------------------------------------
         fname         lname         major           age
---------------------------------------------------------------------------
    1.          Flo          Yao           CS           20
    2.   Anna Grace      Del Rio           CS           22

---------------------------------------------------------------------------




//.................
//:OR AND OR      :
//.................

[34] : select * from student where lname = Yang or major = CS and age < 23 or lname = Jackson

---------------------------------------------------------------------------
         fname         lname         major           age
---------------------------------------------------------------------------
    1.          Flo          Yao           CS           20
    2.          Flo      Jackson         Math           21
    3.   Anna Grace      Del Rio           CS           22

---------------------------------------------------------------------------













||| File reached end of file

Command : x
Press <RETURN> to close this window...

//---------------------------------------------------------------------
// RESSESSION RUN OF SAMPLE.TXT :
//---------------------------------------------------------------------

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
                   Relational Database System
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 * Implementation of a database that will respond to SQL-like
 * commands. They include :
 * >> CREATE|MAKE TABLE <TABLENAME> FIELDS <FIELDNAME(S) . . .>
 * >> INSERT INTO <TABLE NAME> VALUES <VALUE(S) . . . >
 * >> SELECT *|<FIELD(S) . . .> FROM <TABLENAME> WHERE <CONDITION>

 * <CONDITION> include : <FIELDNAME> <RELATIONAL_OPERATOR> <VALUE>
 * chain conditions together with a <LOGICAL OPERATOR>
 * <RELATIONAL OPERATOR> : = < > <= >=
 * <LOGICAL OPERATOR> : AND OR
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Command : select * from employee
[1] : select * from employee

---------------------------------------------------------------------------
          last         first           dep
---------------------------------------------------------------------------
    1.         Blow          Joe           CS
    2.      Johnson        Jimmy    Chemistry
    3.         Yang           Bo           CS
    4.      Jackson        Billy         Math
    5.      Johnson        Billy      Phys Ed
    6.     Van Gogh      Jim Bob      Phys Ed

---------------------------------------------------------------------------


Command : select * from student
[2] : select * from student

---------------------------------------------------------------------------
         fname         lname         major           age
---------------------------------------------------------------------------
    1.          Flo          Yao           CS           20
    2.          Flo      Jackson         Math           21
    3.       Calvin          Woo      Physics           22
    4.   Anna Grace      Del Rio           CS           22

---------------------------------------------------------------------------


Command : select fname lname from student where major = S
[3] : select fname lname from student where major = S

---------------------------------------------------------------------------
         fname         lname
---------------------------------------------------------------------------

---------------------------------------------------------------------------


Command : select fname lname from student where major = CS
[4] : select fname lname from student where major = CS

---------------------------------------------------------------------------
         fname         lname
---------------------------------------------------------------------------
    1.          Flo          Yao
    2.   Anna Grace      Del Rio

---------------------------------------------------------------------------


Command : select age from student where fame > A
[5] : select age from student where fame > A

---------------------------------------------------------------------------
           age
---------------------------------------------------------------------------
    1.           20
    2.           21
    3.           22
    4.           22

---------------------------------------------------------------------------


Command : select * from student where major < Zoology
[6] : select * from student where major < Zoology

---------------------------------------------------------------------------
         fname         lname         major           age
---------------------------------------------------------------------------
    1.          Flo          Yao           CS           20
    2.          Flo      Jackson         Math           21
    3.       Calvin          Woo      Physics           22
    4.   Anna Grace      Del Rio           CS           22

---------------------------------------------------------------------------


Command : insert into student values Raymond Hien CS 20
[7] : insert into student values Raymond Hien CS 20

---------------------------------------------------------------------------
         fname         lname         major           age
---------------------------------------------------------------------------
    1.          Flo          Yao           CS           20
    2.          Flo      Jackson         Math           21
    3.       Calvin          Woo      Physics           22
    4.   Anna Grace      Del Rio           CS           22
    5.      Raymond         Hien           CS           20

---------------------------------------------------------------------------


Command : select * from student
[8] : select * from student

---------------------------------------------------------------------------
         fname         lname         major           age
---------------------------------------------------------------------------
    1.          Flo          Yao           CS           20
    2.          Flo      Jackson         Math           21
    3.       Calvin          Woo      Physics           22
    4.   Anna Grace      Del Rio           CS           22
    5.      Raymond         Hien           CS           20

---------------------------------------------------------------------------


Command : x
Press <RETURN> to close this window...

//---------------------------------------------------------------------
// INVALID COMMANDS FROM INVALID_COMMANDS.TXT :
//---------------------------------------------------------------------

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
                   Relational Database System
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 * Implementation of a database that will respond to SQL-like
 * commands. They include :
 * >> CREATE|MAKE TABLE <TABLENAME> FIELDS <FIELDNAME(S) . . .>
 * >> INSERT INTO <TABLE NAME> VALUES <VALUE(S) . . . >
 * >> SELECT *|<FIELD(S) . . .> FROM <TABLENAME> WHERE <CONDITION>

 * <CONDITION> include : <FIELDNAME> <RELATIONAL_OPERATOR> <VALUE>
 * chain conditions together with a <LOGICAL OPERATOR>
 * <RELATIONAL OPERATOR> : = < > <= >=
 * <LOGICAL OPERATOR> : AND OR
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Command : batch invalid_commands.txt
//********* invalid commands *************************\//
||| Invalid Command was inputted
 : create
||| Invalid Command was inputted
 : create table
||| Invalid Command was inputted
 : create table invalid
||| Invalid Command was inputted
 : create table invalid field

||| Invalid Command was inputted
 : insert
||| Invalid Command was inputted
 : insert into
||| Invalid Command was inputted
 : insert into invalid
||| Invalid Command was inputted
 : insert into invalid value
||| Could not process an insertion to a table that does
not exist
||| Invalid Command was inputted
 : select
||| Invalid Command was inputted
 : select *
||| Invalid Command was inputted
 : select * from
||| A file did not exist when attemping to access
||| Invalid Command was inputted
 : select * from invalid wh
||| Invalid Command was inputted
 : select * from invalid where
||| Invalid Command was inputted
 : select * from invalid where age
||| Invalid Command was inputted
 : select * from invalid where age dsfs

||| A file did not exist when attemping to access
||| A file did not exist when attemping to access
||| Invalid Command was inputted
 : select * frm table

||| Invalid Command was inputted
 : sdfssdd
||| Invalid Command was inputted
 : 32423
||| File reached end of file

Command : x
Press <RETURN> to close this window...

//---------------------------------------------------------------------
// Interactive Run :
//---------------------------------------------------------------------

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
                   Relational Database System
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 * Implementation of a database that will respond to SQL-like
 * commands. They include :
 * >> CREATE|MAKE TABLE <TABLENAME> FIELDS <FIELDNAME(S) . . .>
 * >> INSERT INTO <TABLE NAME> VALUES <VALUE(S) . . . >
 * >> SELECT *|<FIELD(S) . . .> FROM <TABLENAME> WHERE <CONDITION>

 * <CONDITION> include : <FIELDNAME> <RELATIONAL_OPERATOR> <VALUE>
 * chain conditions together with a <LOGICAL OPERATOR>
 * <RELATIONAL OPERATOR> : = < > <= >=
 * <LOGICAL OPERATOR> : AND OR
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Command : make table regalemp fields FNAME LNAME WKHOURS WAGE
[1] : make table regalemp fields FNAME LNAME WKHOURS WAGE

---------------------------------------------------------------------------
         FNAME         LNAME       WKHOURS          WAGE
---------------------------------------------------------------------------

---------------------------------------------------------------------------


Command : insert into regalemp values "Raymond" "Hien" 10 "$13"
[2] : insert into regalemp values "Raymond" "Hien" 10 "$13"

---------------------------------------------------------------------------
         FNAME         LNAME       WKHOURS          WAGE
---------------------------------------------------------------------------
    1.      Raymond         Hien           10          $13

---------------------------------------------------------------------------


Command : insert into regalemp values Kaylynn Patipaksiri 25 "13"
[3] : insert into regalemp values Kaylynn Patipaksiri 25 "13"

---------------------------------------------------------------------------
         FNAME         LNAME       WKHOURS          WAGE
---------------------------------------------------------------------------
    1.      Raymond         Hien           10          $13
    2.      Kaylynn  Patipaksiri           25           13

---------------------------------------------------------------------------


Command : insert into regalemp values "Alissa" "Lo" 40 "$18"
[4] : insert into regalemp values "Alissa" "Lo" 40 "$18"

---------------------------------------------------------------------------
         FNAME         LNAME       WKHOURS          WAGE
---------------------------------------------------------------------------
    1.      Raymond         Hien           10          $13
    2.      Kaylynn  Patipaksiri           25           13
    3.       Alissa           Lo           40          $18

---------------------------------------------------------------------------


Command : insert into regalemp values Chris Ludeman 40 15
[5] : insert into regalemp values Chris Ludeman 40 15

---------------------------------------------------------------------------
         FNAME         LNAME       WKHOURS          WAGE
---------------------------------------------------------------------------
    1.      Raymond         Hien           10          $13
    2.      Kaylynn  Patipaksiri           25           13
    3.       Alissa           Lo           40          $18
    4.        Chris      Ludeman           40           15

---------------------------------------------------------------------------


Command : insert into regalemp values Ondre Ozzy TERMINATED 14
[6] : insert into regalemp values Ondre Ozzy TERMINATED 14

---------------------------------------------------------------------------
         FNAME         LNAME       WKHOURS          WAGE
---------------------------------------------------------------------------
    1.      Raymond         Hien           10          $13
    2.      Kaylynn  Patipaksiri           25           13
    3.       Alissa           Lo           40          $18
    4.        Chris      Ludeman           40           15
    5.        Ondre         Ozzy   TERMINATED           14

---------------------------------------------------------------------------


Command : insert into regalemp values Kacky Anderson TERMINATED $10
||| There was a mismatch in fields
Command : insert into regalemp values Kacky Anderson TERMINATED "$10"
[7] : insert into regalemp values Kacky Anderson TERMINATED "$10"

---------------------------------------------------------------------------
         FNAME         LNAME       WKHOURS          WAGE
---------------------------------------------------------------------------
    1.      Raymond         Hien           10          $13
    2.      Kaylynn  Patipaksiri           25           13
    3.       Alissa           Lo           40          $18
    4.        Chris      Ludeman           40           15
    5.        Ondre         Ozzy   TERMINATED           14
    6.        Kacky     Anderson   TERMINATED          $10

---------------------------------------------------------------------------


Command : insert into regalemp Jason Huang "Previous EMP" "$13"
||| Invalid Command was inputted
 : insert into regalemp Jason Huang "Previous EMP" "$13"
Command : insert into regalemp values Jason Huang "Previous EMP" "$13"
[8] : insert into regalemp values Jason Huang "Previous EMP" "$13"

---------------------------------------------------------------------------
         FNAME         LNAME       WKHOURS          WAGE
---------------------------------------------------------------------------
    1.      Raymond         Hien           10          $13
    2.      Kaylynn  Patipaksiri           25           13
    3.       Alissa           Lo           40          $18
    4.        Chris      Ludeman           40           15
    5.        Ondre         Ozzy   TERMINATED           14
    6.        Kacky     Anderson   TERMINATED          $10
    7.        Jason        Huang Previous EMP          $13

---------------------------------------------------------------------------


Command : insert into regalemp values Kaylah Medina TERMINATED "$13"
[9] : insert into regalemp values Kaylah Medina TERMINATED "$13"

---------------------------------------------------------------------------
         FNAME         LNAME       WKHOURS          WAGE
---------------------------------------------------------------------------
    1.      Raymond         Hien           10          $13
    2.      Kaylynn  Patipaksiri           25           13
    3.       Alissa           Lo           40          $18
    4.        Chris      Ludeman           40           15
    5.        Ondre         Ozzy   TERMINATED           14
    6.        Kacky     Anderson   TERMINATED          $10
    7.        Jason        Huang Previous EMP          $13
    8.       Kaylah       Medina   TERMINATED          $13

---------------------------------------------------------------------------


Command : insert into
||| Invalid Command was inputted
 : insert into
Command : select * from regalemp where WKHOURS = TERMINATED
[10] : select * from regalemp where WKHOURS = TERMINATED

---------------------------------------------------------------------------
         FNAME         LNAME       WKHOURS          WAGE
---------------------------------------------------------------------------
    1.        Ondre         Ozzy   TERMINATED           14
    2.        Kacky     Anderson   TERMINATED          $10
    3.       Kaylah       Medina   TERMINATED          $13

---------------------------------------------------------------------------


Command : select * from regalemp where WKHOURS < 40
[11] : select * from regalemp where WKHOURS < 40

---------------------------------------------------------------------------
         FNAME         LNAME       WKHOURS          WAGE
---------------------------------------------------------------------------
    1.      Raymond         Hien           10          $13
    2.      Kaylynn  Patipaksiri           25           13

---------------------------------------------------------------------------


Command : select fname from regalemp where WKHOURS = "PREVIOUS EMP"
[12] : select fname from regalemp where WKHOURS = "PREVIOUS EMP"

---------------------------------------------------------------------------
         fname
---------------------------------------------------------------------------

---------------------------------------------------------------------------


Command : select fname from regalemp where WKHOURS ="Previous Emp"
[13] : select fname from regalemp where WKHOURS ="Previous Emp"

---------------------------------------------------------------------------
         fname
---------------------------------------------------------------------------

---------------------------------------------------------------------------


Command : select fname from regalemp where WKHOURS="Previous EMP"
[14] : select fname from regalemp where WKHOURS="Previous EMP"

---------------------------------------------------------------------------
         fname
---------------------------------------------------------------------------
    1.        Jason

---------------------------------------------------------------------------


Command : x
Press <RETURN> to close this window...
