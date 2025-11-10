#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t writeLock = PTHREAD_MUTEX_INITIALIZER;
int readCount = 0;

void* reader(void* arg) {
    pthread_mutex_lock(&mutex);
    readCount++;
    if (readCount == 1)
        pthread_mutex_lock(&writeLock);   // First reader blocks writer
    pthread_mutex_unlock(&mutex);

    printf("Reader %ld is reading\n", (long)arg);
    sleep(1);

    pthread_mutex_lock(&mutex);
    readCount--;
    if (readCount == 0)
        pthread_mutex_unlock(&writeLock); // Last reader allows writer
    pthread_mutex_unlock(&mutex);

    printf("Reader %ld left\n", (long)arg);
    return NULL;
}

void* writer(void* arg) {
    pthread_mutex_lock(&writeLock);
    printf("Writer %ld is writing\n", (long)arg);
    sleep(2);
    pthread_mutex_unlock(&writeLock);
    printf("Writer %ld left\n", (long)arg);
    return NULL;
}

int main() {
    int r, w;
    printf("Enter number of readers: ");
    scanf("%d", &r);
    printf("Enter number of writers: ");
    scanf("%d", &w);

    pthread_t rt[r], wt[w];

    for (long i = 0; i < r; i++)
        pthread_create(&rt[i], NULL, reader, (void*)i);

    for (long i = 0; i < w; i++)
        pthread_create(&wt[i], NULL, writer, (void*)i);

    for (int i = 0; i < r; i++)
        pthread_join(rt[i], NULL);
    for (int i = 0; i < w; i++)
        pthread_join(wt[i], NULL);

    return 0;
}


/* =====================================================================================
   ✅ LINE-BY-LINE EXPLANATION — READERS–WRITERS PROBLEM (READER PRIORITY)
   =====================================================================================

1.  #include <pthread.h> → needed for threads and mutex
    #include <unistd.h>  → for sleep()

2.  mutex       → protects readCount variable (so readers don't conflict)
    writeLock   → ensures only ONE writer OR multiple readers at a time
    readCount   → total active readers

--------------------------- READER THREAD FUNCTION -----------------------------

3. pthread_mutex_lock(&mutex);
       - Lock before updating readCount (critical section)

4. readCount++;
   if(readCount == 1) → first reader arrives
       pthread_mutex_lock(&writeLock);
       - First reader blocks writers (writer cannot enter while readers exist)

5. pthread_mutex_unlock(&mutex); → Release mutex

6. printf("Reader %ld is reading"); → Simulate read
   sleep(1); → Reading takes 1 second

7. pthread_mutex_lock(&mutex); → Reader exits critical section
   readCount--;
   if(readCount == 0)
       pthread_mutex_unlock(&writeLock);
       - Last reader leaves → allow writers

8. pthread_mutex_unlock(&mutex);
   printf("Reader %ld left\n");

--------------------------- WRITER THREAD FUNCTION -----------------------------

9. pthread_mutex_lock(&writeLock);
       - Writer waits until no reader/writer is inside
10. printf("Writer %ld is writing");
    sleep(2);  → Writing takes longer than reading
11. pthread_mutex_unlock(&writeLock);
    printf("Writer %ld left");

------------------------------ MAIN FUNCTION ----------------------------------

12. Input number of readers and writers

13. pthread_t rt[r], wt[w];
       - Arrays of thread IDs

14. Create reader threads
       pthread_create(&rt[i], NULL, reader, (void*)i);

15. Create writer threads
       pthread_create(&wt[i], NULL, writer, (void*)i);

16. Join all threads so program waits for them to finish

------------------------- ALGORITHM BEHAVIOR SUMMARY ---------------------------

✔ Multiple readers can read **simultaneously**  
✔ Writer gets **exclusive access** (no other reader or writer allowed)  
✔ First reader locks writer; last reader unlocks writer  
✔ This is **Reader Priority** solution → writers may starve if many readers keep coming

------------------------------ SAMPLE OUTPUT ----------------------------------

Enter number of readers: 3
Enter number of writers: 2

Reader 0 is reading
Reader 1 is reading
Reader 2 is reading
Reader 0 left
Reader 1 left
Reader 2 left
Writer 0 is writing
Writer 0 left
Writer 1 is writing
Writer 1 left

=====================================================================================
   END OF EXPLANATION
=====================================================================================
*/
