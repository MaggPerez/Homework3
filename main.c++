#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>


using namespace std;

int main() {

    //Creating an array with 20 elements
    int elements[20];

    int size = sizeof(elements) / sizeof(elements[0]);


    //Filing array using random number generator
    srand(time(0));
    for(int i = 0; i < size; i++){
        elements[i] = rand() % 100;
    }


    //Printing array
    cout << "\nArray with random numbers: ";
    for (int i = 0; i < size; i++){
        cout << elements[i] << " ";
    }
    cout << endl;
    cout << endl;



    //Creating pipe
    int fd[2];
    pipe(fd);

    if(pipe(fd) == -1){
        cout << "Error in creating pipe" << endl;
        return(1);
    }



    //forking a child process
    pid_t c_pid = fork();

    //Error message if fork fails
    if(c_pid < -1){
        cout << "Error in creating child process" << endl;
        return(2);
    }

    //Creating child process that will find the minimum number in the second half of the array
    else if(c_pid == 0){
        close(fd[0]);
        int min = elements[size / 2];

        for(int i = size / 2; i < size; i++){
            if(elements[i] < min){
                min = elements[i];
            }
        }

        //Displaying child process PID and minimum number for the second half of the array
        cout << "Child Process (PID " << getpid() << "): Minimum in second half = " << min << endl;


        //Using pipe to pass minimum number of the child to parent process
        write(fd[1], &min, sizeof(min));
        close(fd[1]);
        exit(0);
    }

    //creating parent process that will find the minimum number in the first half of the array
    else{
        close(fd[1]);
        int min = elements[0];

        for(int i = 0; i < size / 2; i++){
            if(elements[i] < min){
                min = elements[i];
            }
        }

        //Displaying parent process PID and minimum number for the first half of the array
        cout << "Parent Process (PID " << getpid() << "): Minimum in first half = " << min << endl;


        //Reading minimum number from child process using pipe
        int child_min;
        read(fd[0], &child_min, sizeof(child_min));
        close(fd[0]);


        //Finding and displaying the overall minimum number
        int overall_min = min < child_min ? min : child_min;
        cout << "Overall minimum of the array = " << overall_min << endl;


        //Waiting for child process to finish
        wait(NULL);
    }

    
    return 0;
}