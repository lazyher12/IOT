#include <stdio.h>
#include <unistd.h>

void sortAsc(int a[], int n) {
    for(int i=0;i<n;i++)
        for(int j=i+1;j<n;j++)
            if(a[i] > a[j]) { int t=a[i]; a[i]=a[j]; a[j]=t; }

    printf("\n[Child | PID %d] Sorted in Ascending Order: ", getpid());
    for(int i=0;i<n;i++) printf("%d ", a[i]);
    printf("\n");
}

void sortDesc(int a[], int n) {
    for(int i=0;i<n;i++)
        for(int j=i+1;j<n;j++)
            if(a[i] < a[j]) { int t=a[i]; a[i]=a[j]; a[j]=t; }

    printf("\n[Parent | PID %d] Sorted in Descending Order: ", getpid());
    for(int i=0;i<n;i++) printf("%d ", a[i]);
    printf("\n");
}

int main() {
    int n, a[50];
    printf("Enter size: ");
    scanf("%d", &n);

    printf("Enter elements: ");
    for(int i=0;i<n;i++) scanf("%d", &a[i]);

    pid_t pid = fork();

    if(pid == 0) {          // Child
        sortAsc(a, n);
    } else {                // Parent
        sortDesc(a, n);
    }
    return 0;
}


/* ==================================================================================
   ✅ LINE-BY-LINE EXPLANATION — PROCESS CREATION & SORTING PROGRAM (fork system call)
   ==================================================================================

1.  #include <stdio.h>
        - Required for printf(), scanf()

2.  #include <unistd.h>
        - Required for fork(), getpid()

--------------------------- ASCENDING SORT FUNCTION ---------------------------

3.  void sortAsc(int a[], int n)
        - Receives array and size, sorts in ascending order

4.  Nested loops compare a[i] and a[j]
        - Simple bubble-style sorting logic
        - If a[i] > a[j], swap the values

5.  printf("[Child | PID %d] ...", getpid());
        - Displays process ID of child process
        - getpid() returns PID of currently running process

--------------------------- DESCENDING SORT FUNCTION --------------------------

6.  Same logic but condition changes:
        - if(a[i] < a[j]) → swap → results in descending order

7.  Printed with "[Parent | PID %d]" to show parent process output

------------------------------- MAIN FUNCTION ---------------------------------

8.  int n, a[50];
        - n = number of elements
        - a[] = array of max size 50

9.  scanf() inputs array size and elements

10. pid_t pid = fork();
        - fork() creates a new process
        - Returns:
              0      → to Child process
              >0 PID → to Parent process

11. if(pid == 0)
        - Code inside this block runs in CHILD process
        - Calls sortAsc()

12. else
        - Code runs in PARENT process
        - Calls sortDesc()

13. return 0;
        - End program (both processes run independently)

--------------------------- PROGRAM EXECUTION FLOW ----------------------------

✔ Before fork(): one process (parent)
✔ After fork(): two processes run the SAME code
✔ Each process executes its own sorting function

Example I/O:

/* output
sagar@sagar-Lenovo-V15-IIL:~/Pictures/osl$ gcc fork2a.c
sagar@sagar-Lenovo-V15-IIL:~/Pictures/osl$ ./a.out

Enter size: 5
Enter elements: 3 9 1 6 2

Output may appear like:

[Child | PID 1234] Sorted in Ascending Order: 1 2 3 6 9
[Parent | PID 1233] Sorted in Descending Order: 9 6 3 2 1

(Child and parent order may change due to scheduling)

------------------------------ IMPORTANT NOTES --------------------------------

✔ fork() duplicates process memory → both parent & child have same array copy  
✔ No shared memory used → changes done by one do NOT affect the other  
✔ getpid() helps identify which process is printing  
✔ This program demonstrates **process creation + independent execution paths**

=================================================================================
   END OF EXPLANATION
================================================================================= */
