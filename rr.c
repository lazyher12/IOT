#include <stdio.h>

int main() {
    int n, i, tq, time = 0, completed = 0;
    int at[20], bt[20], rt[20], wt[20], tat[20], flag[20] = {0};

    printf("Enter number of processes: ");
    scanf("%d", &n);

    for(i = 0; i < n; i++) {
        printf("Enter Arrival Time of P%d: ", i+1);
        scanf("%d", &at[i]);
        printf("Enter Burst Time of P%d: ", i+1);
        scanf("%d", &bt[i]);
        rt[i] = bt[i]; // Remaining time initialized
    }

    printf("Enter Time Quantum: ");
    scanf("%d", &tq);

    printf("\nGantt Chart:\n");
    printf("0");

    while (completed != n) {
        int done = 1;
        for(i = 0; i < n; i++) {
            if(at[i] <= time && rt[i] > 0) {
                done = 0;
                if(rt[i] > tq) {
                    time += tq;
                    rt[i] -= tq;
                } else {
                    time += rt[i];
                    wt[i] = time - at[i] - bt[i];
                    rt[i] = 0;
                    completed++;
                }
                printf(" -- P%d -- %d", i+1, time);
            }
        }
        if(done) time++; // If no process arrived yet, move time forward
    }

    printf("\n\nProcess\tAT\tBT\tWT\tTAT\n");
    printf("----------------------------------------\n");

    float avgWT = 0, avgTAT = 0;
    for(i = 0; i < n; i++) {
        tat[i] = bt[i] + wt[i];
        avgWT += wt[i];
        avgTAT += tat[i];
        printf("P%d\t%d\t%d\t%d\t%d\n", i+1, at[i], bt[i], wt[i], tat[i]);
    }

    printf("\nAverage Waiting Time: %.2f", avgWT / n);
    printf("\nAverage Turnaround Time: %.2f\n", avgTAT / n);

    return 0;
}


/* =====================================================================================
   ✅ LINE-BY-LINE EXPLANATION — ROUND ROBIN CPU SCHEDULING PROGRAM
   =====================================================================================

1.  n  → number of processes
    tq → time quantum
    time → global clock
    completed → number of finished processes

2.  at[]  → arrival time of each process
    bt[]  → burst time (original total execution time)
    rt[]  → remaining time (changes during execution)
    wt[]  → waiting time
    tat[] → turnaround time
    flag[] (unused here, initially 0)

----------------------------- INPUT SECTION ------------------------------------

3.  User enters number of processes and fills arrival & burst time
4.  Remaining time array rt[] is initialized to burst time

-------------------------- ENTER TIME QUANTUM ----------------------------------

5.  Time quantum defines how long each process can run at once (CPU slice)

----------------------------- GANTT CHART --------------------------------------

6.  printf("0") prints starting time
7.  while(completed != n):
        - Run scheduling until all processes finish

8.  done = 1 means assume no process is ready
9.  for each process:
        if(at[i] <= time && rt[i] > 0)
        - Process has arrived AND still needs CPU time

10. If(rt[i] > tq)
        - Process runs for time quantum only
        - Remaining time is reduced
        - Clock increases by tq

11. Else
        - Process will finish in this CPU burst
        - waiting time = completion time - arrival time - burst time
        - remaining time becomes 0
        - completed++

12. Each execution step prints:  -- Pn -- <time>

13. If no process arrived yet → time++

----------------------------- CALCULATION TABLE --------------------------------

14. After loop ends, calculate:
        TAT = BT + WT
        AvgWT = sumWaiting / n
        AvgTAT = sumTurnaround / n

15. Print formatted table:

    Process   AT   BT   WT   TAT

-------------------------- ROUND ROBIN SUMMARY ---------------------------------

✔ Works on **Time Sharing System**
✔ Each process gets equal CPU time in cyclic order
✔ Good for fairness (no starvation)
✔ Waiting time is calculated only when process completes


/* output
sagar@sagar-Lenovo-V15-IIL:~/Pictures/osl$ gcc RR.c
sagar@sagar-Lenovo-V15-IIL:~/Pictures/osl$ ./a.out
Enter number of processes: 3
Enter Arrival Time of P1: 0
Enter Burst Time of P1: 5
Enter Arrival Time of P2: 1
Enter Burst Time of P2: 3
Enter Arrival Time of P3: 2
Enter Burst Time of P3: 8
Enter Time Quantum: 2

Gantt Chart:
0 -- P1 -- 2 -- P2 -- 4 -- P3 -- 6 -- P1 -- 8 -- P2 -- 9 -- P3 -- 11 -- P1 -- 12 -- P3 -- 14 -- P3 -- 16

Process	AT	BT	WT	TAT
----------------------------------------
P1	0	5	7	12
P2	1	3	5	8
P3	2	8	6	14

Average Waiting Time: 6.00
Average Turnaround Time: 11.33
*/
