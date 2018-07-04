#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <pty.h>
#include <termios.h>
#include <fcntl.h>

#include <sys/select.h>
#include <sys/wait.h>

#include "structs.h"

int main(){

    char info[10000]; info[0] = '\0';
    FILE * read_info = fopen("welcome.txt", "rt");
    if(read_info == NULL){ 
        strcat(info, "Could not load info! Quick commands:\n'spy log user@host' connects through ssh\n'spy on-off' turns on logging of executions\n'spy show' shows logged executions\n");
    }else{
        // read here
        fseek(read_info, 0L, SEEK_END);
        int t_to_read = ftell(read_info);
        rewind(read_info);
        int characters = fread(info, sizeof(char), t_to_read, read_info);
        fprintf(stdout, "%s\n", info);
    }

    char command[MAX_CMD];
    command[0] = '\0';


    while(strncmp(command, "spy out", 7) != 0){

        fgets(command, MAX_CMD, stdin);

                
        if(strncmp(command, "spy help", 7) == 0){
            fprintf(stdout, "%s\n\n", info);
        }

        else if(command[0] != '\0' && command[0] != '\n'){
            fprintf(stdout, "Unrecognized command. Use 'spy help' for a list of commands\n");
        }

        // Log with credentials
        if(strncmp(command, "spy log", 7) == 0){

            int master;
            pid_t pid;

            pid = forkpty(&master, NULL, NULL, NULL);

            // Check if we can fork process
            if (pid < 0) {
                return 1;
            }

            // Child process, establish ssh connection here
            else if (pid == 0) {

                char *args[] = { "estebanpw@pistacho.ac.uma.es" };

                // make ssh connection
                execlp("ssh", "ssh", "-t", "-t", "estebanpw@pistacho.ac.uma.es", (char *) 0);
            }

            // Parent process
            else {
                // Remove echo from terminal (only show once what is written)
                struct termios tios;
                tcgetattr(master, &tios);
                tios.c_lflag &= ~(ECHO | ECHONL);
                tcsetattr(master, TCSAFLUSH, &tios);

                while (1) {

                    // File descriptors
                    fd_set read_fd;
                    fd_set write_fd;
                    fd_set except_fd;

                    // Initialize them every time, since select will write on them
                    FD_ZERO(&read_fd);
                    FD_ZERO(&write_fd);
                    FD_ZERO(&except_fd);

                    // Obtain the descriptor from the master
                    FD_SET(master, &read_fd);
                    // Obtain the descriptor for the input from stdin
                    FD_SET(STDIN_FILENO, &read_fd);

                    // Set which descriptors we want to check
                    select(master+1, &read_fd, &write_fd, &except_fd, NULL);

                    char input;
                    char output;

                    // Bit from child is set, read and write to master
                    if (FD_ISSET(master, &read_fd))
                    {
                        // Read what is being sent back to terminal
                        if (read(master, &output, 1) != -1){
                                                
                            // Write output to stdout
                            write(STDOUT_FILENO, &output, 1);
                        }else
                            break;
                    }

                    // Bit from master is set, then write to child
                    if (FD_ISSET(STDIN_FILENO, &read_fd))
                    {
                        // Read input
                        read(STDIN_FILENO, &input, 1);
                        // Write it to child
                        write(master, &input, 1);
                    }
                }
            }
        }
    }
    return 0;
}