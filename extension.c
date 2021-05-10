#include "extension.h"

void parse_command(char input[], char *argv[], int *wait)
{
    for (unsigned idx = 0; idx < 50; idx++)
    {
        argv[idx] = NULL;
    }

    // Check for trailing & and remove if exists
    if (input[strlen(input) - 1] == '&')
    {
        *wait = 1;
        input[strlen(input) - 1] = '\0';
    }
    else
    {
        *wait = 0;
    }

    // Perform tokenization on input string
    const char *delim = " ";
    unsigned idx = 0;

    char *token = strtok(input, delim);
    while (token != NULL)
    {
        argv[idx++] = token;
        token = strtok(NULL, delim);
    }

    argv[idx] = NULL;
}

void parse_redir(char *argv[], char *redir_argv[])
{
    unsigned idx = 0;
    redir_argv[0] = NULL;
    redir_argv[1] = NULL;

    while (argv[idx] != NULL)
    {

        // Check if command contains character <, >
        if (strcmp(argv[idx], "<") == 0 || strcmp(argv[idx], ">") == 0)
        {

            // Check for succeeded file name
            if (argv[idx + 1] != NULL)
            {

                // Move redirect type and file name to redirect arguments vector
                redir_argv[0] = strdup(argv[idx]);
                redir_argv[1] = strdup(argv[idx + 1]);
                argv[idx] = NULL;
                argv[idx + 1] = NULL;
            }
        }

        idx++;
    }
}

int parse_pipe(char *argv[], char *child01_argv[], char *child02_argv[])
{
    unsigned idx = 0, split_idx = 0;
    int contains_pipe = 0;

    while (argv[idx] != NULL)
    {

        // Check if user_command contains pipe character |
        if (strcmp(argv[idx], "|") == 0)
        {
            split_idx = idx;
            contains_pipe = 1;
        }
        idx++;
    }

    if (!contains_pipe)
    {
        return 0;
    }

    // Copy arguments before split pipe position to child01_argv[]
    for (idx = 0; idx < split_idx; idx++)
    {
        child01_argv[idx] = strdup(argv[idx]);
    }
    child01_argv[idx++] = NULL;

    // Copy arguments after split pipe position to child02_argv[]
    while (argv[idx] != NULL)
    {
        child02_argv[idx - split_idx - 1] = strdup(argv[idx]);
        idx++;
    }
    child02_argv[idx - split_idx - 1] = NULL;

    return 1;
}

void child(char *argv[], char *redir_argv[])
{
    int fd_out, fd_in;
    if (redir_argv[0] != NULL)
    {

        // Redirect output
        if (strcmp(redir_argv[0], ">") == 0)
        {

            // Get file description
            fd_out = creat(redir_argv[1], S_IRWXU);
            if (fd_out == -1)
            {
                perror("Redirect output failed");
                exit(EXIT_FAILURE);
            }

            // Replace stdout with output file
            dup2(fd_out, STDOUT_FILENO);

            // Check for error on close
            if (close(fd_out) == -1)
            {
                perror("Closing output failed");
                exit(EXIT_FAILURE);
            }
        }

        // Redirect input
        else if (strcmp(redir_argv[0], "<") == 0)
        {
            fd_in = open(redir_argv[1], O_RDONLY);
            if (fd_in == -1)
            {
                perror("Redirect input failed");
                exit(EXIT_FAILURE);
            }

            dup2(fd_in, STDIN_FILENO);

            if (close(fd_in) == -1)
            {
                perror("Closing input failed");
                exit(EXIT_FAILURE);
            }
        }
    }

    // Execute user command in child process
    if (execvp(argv[0], argv) == -1)
    {
        perror("Fail to execute command");
        exit(EXIT_FAILURE);
    }
}

void parent(pid_t child_pid, int wait)
{
    int status;
    switch (wait)
    {

    // Parent and child are running concurrently
    case 0:
    {
        waitpid(child_pid, &status, 0);
        break;
    }

    // Parent waits for child process with PID to be terminated
    default:
    {
        waitpid(child_pid, &status, WUNTRACED);
        break;
    }
    }
}

void exec_with_pipe(char *child01_argv[], char *child02_argv[])
{
    int pipefd[2];

    if (pipe(pipefd) == -1)
    {
        /* Create a pipe with 1 input and 1 output file descriptor
      Notation: Index = 0 ==> read pipe, Index = 1 ==> write pipe
      */
        perror("pipe() failed");
        exit(EXIT_FAILURE);
    }

    // Create 1st child
    if (fork() == 0)
    {

        // Redirect STDOUT to output part of pipe
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);

        execvp(child01_argv[0], child01_argv);
        perror("Fail to execute first command");
        exit(EXIT_FAILURE);
    }

    // Create 2nd child
    if (fork() == 0)
    {

        // Redirect STDIN to input part of pipe
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[1]);
        close(pipefd[0]);

        execvp(child02_argv[0], child02_argv);
        perror("Fail to execute second command");
        exit(EXIT_FAILURE);
    }

    close(pipefd[0]);
    close(pipefd[1]);
    // Wait for child 1
    wait(0);
    // Wait for child 2
    wait(0);
}

int run(char *cmd)
{
    cmd[strcspn(cmd, "\n")] = '\0';
    int wait;
    char *argv[50], *redir_argv[2];
    char *child01_argv[3], *child02_argv[3];

    parse_command(cmd, argv, &wait);
    parse_redir(argv, redir_argv);

    if (parse_pipe(argv, child01_argv, child02_argv))
    {
        exec_with_pipe(child01_argv, child02_argv);
    }

    // Fork child process
    pid_t pid = fork();

    // Fork return twice on success: 0 - child process, > 0 - parent process
    switch (pid)
    {
    case -1:
        perror("fork() failed!");
        exit(EXIT_FAILURE);

    case 0: // In child process
        child(argv, redir_argv);
        exit(EXIT_SUCCESS);

    default: // In parent process
        parent(pid, wait);
    }
    return 0;
}