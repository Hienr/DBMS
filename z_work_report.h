#ifndef Z_WORK_REPORT_H
#define Z_WORK_REPORT_H
/*
    ------------------------------ Final Project ------------------------------

    Project : Database
    Name: Raymond Hien
    Class : CS08 Fundamentals of Data Structures
    Professor: Sassan Barkeshli
    Details: Program implements a database that responds to SQL-like commands

    Data Structures Used:
        - Maps/Multimaps based on a B+ Tree
        - State Machines
        - Stacks/Queues based on linked lists

    Features:
        -Not Implemented:

        -Implemented:

        /* Program features the SQL class that offers two distinct modes :
         * Single-line commands/Batch Processing
         *
         * Single-Line commands will simply ask user for a command and
         * perform said command.
         * Batch processing will take a file name on disk and perform the
         * commands given.
         *
         * Program will ask user for an input, generate a parsed version,
         * and interpret the parsed command to perform a function.
         *
         * Functions of the database include SQL-like commands such as
         * (and limited to) :
         *
         * [1] create|make table <TABLENAME> fields <FIELDNAMES. . .>
         * [2] insert into <TABLENAME> values <VALUES. . .>
         * [3] select <FIELDNAMES...|*> from <TABLENAME> where <EXPRESSION>
         * [4] batch <FILENAME>
         *
         * an EXPRESSION can include : <FIELDNAME> <RELATIONALOP> <VALUE>
         *                              <LOGICAL OPERATOR>
         * <VALUE> : A string of alphanumeric characters, or a string of alphanumeric
                    characters and spaces enclosed by double quotation marks:
                    "Jean Luise", Finch, 1923
         * <RELATIONALOP> : =, <, >, <=, >=
         * <LOGICAL OPERATOR>: and, or
         * <FILENAME> Any string that must include an extension eg: (.bin)
         *
         * Program implements a state machine to interpret commands. State
         * machines will be implemented for a Tokenizer to simply return
         * tokens. As well as a parser to interpret tokens as apart of
         * a valid SQL command.
         *
         * Program implements Maps/Multimaps as the primary data structure
         * used to map certain values, and database indexing.
         *
         * Program implements Stacks/Queues to evaluate expressions given from
         * a ( select * from table where <EXPRESSION> ). A seperate RPN/Set
         * class will manage evaluations of expressions. Set will take in
         * a string, and call on RPN to get a postfix expression. Set will
         * then evaluate the postfix expression.
         *
         * Program uses binary files for disk storage of database records.
         * Program uses text files to record name of fields, tables, or other
         * information that needs to be remembered from session to session.
         *
         * Program will be error checking any command given such as
         * invalid input/commands. After each processing of a command, program
         * will echo back: command, table, command #, and possible error msg.
         *
         * Program saves all echoed outputs to command line to a txt file
         * on disk as well.


        -Partly implemented:

    Bugs     :  features that are implemented are bug-free.


    Reflections:

        If there was ever a time to have a love and hate relationship, this
        project would be the time to have one. This project was one of the
        coolest things I have ever worked on, and actually meant a lot to me.
        I had multiple of these huge projects when I took CS03A, and each time
        I couldn't ever produce a project I felt accomplished in. They always
        ended up with a bunch of band-aid fixes, crashing everywhere,
        difficulty parsing, unfinished, and just simply bad code.

        This project was really a personal test of mine to show to myself that
        I could tackle projects like these. While I can't say the code I write
        is any good, I can say that this project definitely works and wasn't
        just built with band-aids. This is definitely my proudest moment so far
        in my undergrad.

        However I also hated this project, hated how difficult it was. I really
        like a good challenge, but this one definitely took a lot from me. It
        kinda makes me afraid seeing as this is my last year at PCC as I will
        be transferred out this Fall. I am afraid that this is not the hardest
        it will get, and it will just get harder. I poured more than 100+ hours
        in the last two weeks just on this project alone. And I know that when
        I transfer I will not have the luxury of all this time to be working
        on projects like these.

        The biggest thing I learned from this project is to not be afraid
        of changing code. Often times I would get stuck for hours trying
        to implement something with a current design/code. Only to end up
        changing it to have a better design to allow for different features.
        A lot of the hours spent, could have been spent better toward other
        things if I just recognized that my design was simply bad. Another
        important thing I learned was that designing a class to handle
        the entire program requires a lot of thought. Definitely should try
        to design a well thought out class as putting together every piece
        in a project this size is extremely difficult. And a well designed
        class could help ease a lot of the growing pains.

        The most challenging aspect of this program was honestly just
        debugging. Thankfully I had very little bugs with from my
        previous projects, but putting everything together eventually
        creates bugs. And trying to debug bug by bug got complicated very
        quickly. Thankfully the more time I spent on this project, the better
        I got at debugging it.

        Seeing as how this is the last project I will be turning in, I just
        wanted to say your class has been the most fun I have ever had. I
        learned so much more in your class than I did the last two semesters
        combined. My favorite part of the class was the all the assignments
        that involved trees (BST, B Tree, B+). The most coolest however
        (besides this one) would have to be learning about a state machine.
        The state machine is something I can forsee to be really useful for my
        future endeavors. Honestly it would have solved a lot of my problems
        trying to parse things last semester!

        It has been a pleasure to be your student.

        Thanks,
        Raymond hien
*/
#endif // Z_WORK_REPORT_H
