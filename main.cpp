#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <bits/stdc++.h>

using namespace std;
//parsing the command.
char ** get_words(char* input)
{
    char ** command = new char *[10] ;
    char* parse = strtok(input," ");
    int index = 0;
    while(parse != NULL)
    {
        command[index] = parse;
        parse = strtok(NULL, " ");
        index++;
    }
    command[index] = NULL;
    return command;

}
//write in log file without clearing it.
void signal_handler(int signal)
{
    std::ofstream file;
    file.open("logger.txt", std::ios_base::app);
    file << "Child process was terminated"<<endl;
    file.close();
}
// initiate the file and clearing it.
void new_file()
{
    ofstream file;
    file.open("logger.txt");
    file << "";
    file.close();
}

int main()
{
    new_file();
    pid_t child_pid;
    signal(SIGCHLD, signal_handler);
    char ** command;
    while(1)
    {
        // reading command from user.
        string input;
        cout << "\nShell :> ";
        getline(cin, input);
        bool asyncronized = false;
        //to know if to run commands in background or not.
        if(input[input.size() - 1] == '&')
        {
            asyncronized = true;
            input[input.size() - 1] = NULL;
        }
        // converting the string to char array.
        char str[input.length() + 1];
        for(int i = 0 ; i < input.length(); i++)
        {
            str[i] = input[i];
        }
        str[input.length()] = NULL;

        command = get_words(str);
        if(!command[0])
        {
            delete[]command;
            continue;
        }
        //terminate the shell.
        if(strcasecmp(command[0],"exit") == 0)
        {
            exit(0);
        }
        child_pid = fork();
        //executing the command.
        if(child_pid == 0)
        {
            if( execvp(command[0],command)<0);
            perror(command[0]);
        }
        else if(child_pid < 0)
        {
            perror(command[0]);
        }
        else
        {
            if(!asyncronized)
            {
                wait(NULL);
            }

        }
        delete[]command;
    }
    return 0;
}
