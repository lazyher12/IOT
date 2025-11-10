#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define SIZE 5

int buffer[SIZE], in = 0, out = 0;
sem_t emptySlots, fullSlots;
pthread_mutex_t lock;

void *producer(void *arg) {
    for (int i = 0; i < 10; i++) {
        int item = rand() % 100;          // Create item
        
        sem_wait(&emptySlots);            // Wait if buffer full
        pthread_mutex_lock(&lock);        // Lock buffer

        buffer[in] = item;                // Put item in buffer
        printf("Producer produced: %d\n", item);
        in = (in + 1) % SIZE;             // Circular index

        pthread_mutex_unlock(&lock);      // Unlock buffer
        sem_post(&fullSlots);             // One more full slot
        sleep(1);
    }
    return NULL;
}

void *consumer(void *arg) {
    for (int i = 0; i < 10; i++) {

        sem_wait(&fullSlots);             // Wait if buffer empty
        pthread_mutex_lock(&lock);        // Lock buffer

        int item = buffer[out];           // Remove item
        printf("Consumer consumed : %d\n", item);
        out = (out + 1) % SIZE;           // Circular index

        pthread_mutex_unlock(&lock);      // Unlock buffer
        sem_post(&emptySlots);            // One more empty slot
        sleep(2);
    }
    return NULL;
}

int main() {
    pthread_t prod, cons;

    sem_init(&emptySlots, 0, SIZE);   // Buffer empty (5 slots)
    sem_init(&fullSlots, 0, 0);       // No filled slots
    pthread_mutex_init(&lock, NULL);

    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    sem_destroy(&emptySlots);
    sem_destroy(&fullSlots);
    pthread_mutex_destroy(&lock);
    return 0;
}


/* -------------------------------------------------------------
   EXPLANATION OF PROGRAM (PRODUCER - CONSUMER PROBLEM)
   -------------------------------------------------------------

   ✅ This program demonstrates Producer–Consumer problem using:
      - Threads (producer & consumer)
      - Semaphores
      - Mutex Lock
      - Circular Buffer

   ✅ BUFFER
      buffer[SIZE] → shared memory of 5 items
      'in'  → index where producer adds next item
      'out' → index where consumer removes next item
      (Both go in circular form: index = (index + 1) % SIZE)

   ✅ SEMAPHORES USED
      emptySlots → counts free spaces in buffer (starts = SIZE)
      fullSlots  → counts filled spaces in buffer (starts = 0)

      Producer waits on emptySlots before adding item
      Consumer waits on fullSlots before removing item

   ✅ MUTEX LOCK
      pthread_mutex_t lock → ensures only 1 thread accesses buffer at a time
      (prevents race condition)

   ✅ PRODUCER WORKFLOW
      1. Generate item
      2. sem_wait(emptySlots)   → wait if buffer full
      3. Lock mutex
      4. Add item to buffer
      5. Unlock mutex
      6. sem_post(fullSlots)    → signal consumer

   ✅ CONSUMER WORKFLOW
      1. sem_wait(fullSlots)    → wait if empty
      2. Lock mutex
      3. Remove item from buffer
      4. Unlock mutex
      5. sem_post(emptySlots)   → signal producer

   ✅ WHY CIRCULAR BUFFER?
      Because indexes keep rotating:
         0 → 1 → 2 → 3 → 4 → back to 0
      This avoids shifting elements and reusing space efficiently.

   ✅ FINAL OUTPUT LOOKS LIKE:
      Producer produced: 45
      Consumer consumed: 45
      Producer produced: 88
      Producer produced: 12
      Consumer consumed: 88
      ...

   ✅ REAL LIFE EXAMPLE:
      Producer = Chef making burgers
      Consumer = Customer eating burgers
      Buffer   = Tray with capacity 5 burgers
      emptySlots = empty plates
      fullSlots  = ready burgers

   ------------------------------------------------------------- */
