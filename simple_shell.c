#include  <stdio.h>
#include  <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

/* Splits the string by space check returns the array of tokens
*
*/
pid_t fpid;
bool end=0;

int triple_pid[100];
int triple_idx=0;

void exec_command(int check, char **tokens) {
    if (tokens[0] != NULL && tokens[0][0] == 'c' && tokens[0][1] == 'd') {

        int ret = chdir(tokens[1]);
        if (ret == -1) {
            printf("invalid directory\n");
        }
        //tokens[0] = NULL;
        return;
    }
    pid_t child = fork();
    if(check==2)
    {
        triple_pid[triple_idx++]=child;
    }
    if (child == 0) {
        if (check==1)
            signal(SIGINT, SIG_IGN);
        int ret = execvp(tokens[0], tokens);
        if (ret == -1) {
            printf("invalid command\n");
        }
    } else if (child < 0) {
        printf("fork failed");
    }
    else if (!check)
        fpid = child, waitpid(child, NULL, 0), fpid = 0;
}

void exec_double_and(char **tokens) {
    char *temp[100][100];
    int com = 0;
    int idx = 0;
    for (int i = 0;; i++) {
        if (tokens[i] == NULL || strcmp(tokens[i], "&&") == 0) {
            temp[com][idx] = NULL;
            idx = 0; // Reset index for the next command
            com++;
            if (tokens[i] == NULL) break;
            else
                continue;
        }

            temp[com][idx++] = tokens[i];
        }

         // Break the loop at the end
    for (int i = 0;i<com; i++) {
        if(end)
        {
            end=0;
            return;
        }
        exec_command(0, temp[i]);
    }

}
void exec_triple_and(char **tokens) {
    char *temp[100][100];
    int com = 0;
    int idx = 0;
    for (int i = 0;; i++) {
        if (tokens[i] == NULL || strcmp(tokens[i], "&&&") == 0) {
            temp[com][idx] = NULL;
            idx = 0; // Reset index for the next command
            com++;
            if (tokens[i] == NULL) break;
            else
                continue;
        }

        temp[com][idx++] = tokens[i];
    }

    // Break the loop at the end
    for (int i = 0;i<com; i++) {
        if(end)
        {
            end=0;
            return;
        }
        exec_command(2, temp[i]);

    }
    pid_t pid;
    while ((pid = waitpid(-1, NULL, 0)) > 0) {
        continue;
    }

}

char **tokenize(char *line)
{
    char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
    char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
    int i, tokenIndex = 0, tokenNo = 0;

    for(i =0; i < strlen(line); i++){

        char readChar = line[i];

        if (readChar == ' ' || readChar == '\n' || readChar == '\t'){
            token[tokenIndex] = '\0';
            if (tokenIndex != 0){
                tokens[tokenNo] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
                strcpy(tokens[tokenNo++], token);
                tokenIndex = 0;
            }
        } else {
            token[tokenIndex++] = readChar;
        }
    }

    free(token);
    tokens[tokenNo] = NULL ;
    return tokens;
}

long long PIDS[100000];
int cnt = 0;

void reap_background_processes() {
    pid_t pid;
    while ((pid = waitpid(-1, NULL, WNOHANG)) > 0) {
        printf("Shell: Background process PID %d finished\n", pid);
    }
}

void sigint_handler(int sig) {

    if (fpid > 0) {
        end=1;
        //printf("%d\n",fpid);
        kill(fpid, SIGKILL);

    }
    for(int i=0;i<triple_idx;i++) {
        kill(triple_pid[i], SIGKILL);
    }
    triple_idx=0;
}


int main(int argc, char *argv[]) {
    char line[MAX_INPUT_SIZE];
    char **tokens;
    int i;
    while (1) {
        //if(fpid)
        //waitpid(-1,0,0);
        /* BEGIN: TAKING INPUT */
        bzero(line, sizeof(line));
        printf("$ ");
        scanf("%[^\n]", line);
        getchar();

        //printf("Commcheck entered: %s (remove this debug output later)\n", line);
        /* END: TAKING INPUT */

        line[strlen(line)] = '\n'; //terminate with new line
        tokens = tokenize(line);
        signal(SIGINT, sigint_handler);
        if (tokens[0] == NULL) {
            for (i = 0; tokens[i] != NULL; i++) {
                free(tokens[i]);
            }
            free(tokens);
            continue;
        }
        bool double_and = false, triple_and = false;

        PIDS[cnt++] = getpid();

        if (tokens[0][0] == 'e' && tokens[0][1] == 'x' && tokens[0][2] == 'i' && tokens[0][3] == 't') {
            for (i = cnt - 1; i >= 0; i--) {
                kill(PIDS[i], SIGKILL);
            }
            tokens[0] = NULL;
            break;
        }

        reap_background_processes();

        bool check = 0;
        for (i = 0; tokens[i] != NULL; i++) {
            if (tokens[i][0] == '&' && tokens[i][1] == NULL)
                check = true, tokens[i] = NULL;
        }
        for (i = 0; tokens[i] != NULL; i++) {

            if (tokens[i][0] == '&' && tokens[i][1] != NULL && tokens[i][1] == '&' && tokens[i][2] != NULL &&
                tokens[i][2] == '&')
                triple_and = true;
            else if (tokens[i][0] == '&' && tokens[i][1] != NULL && tokens[i][1] == '&')
                double_and = true;
        }
        if (!double_and && !triple_and) {
            exec_command(check, tokens);
        }
       else if (double_and) {
            exec_double_and(tokens);

        }
       else
        {
            exec_triple_and(tokens);
        }
        // Freeing the allocated memory
        for (i = 0; tokens[i] != NULL; i++) {
            free(tokens[i]);
        }
        free(tokens);


    }
    return 0;
}