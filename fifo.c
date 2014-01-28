#include <stdlib.h>
#include <stdio.h>

#define BUF_SIZE 4

typedef struct Point {
  double x;
  double y;
} Point;

typedef struct CircularBuffer {
  int capacity;
  int head;
  int count;
  Point *points;
} CircularBuffer;

// Create a circular buffer with an array of points
void initBuffer(CircularBuffer *cb, int cap) {
  cb->capacity = cap;
  cb->head     = 0;
  cb->count     = 0;
  cb->points   = malloc(sizeof(Point) * cb->capacity);
}

// Free allocated memory of a circular buffer
void freeBuffer(CircularBuffer *cb) {
  free(cb->points);
}

// Buffer is full count meets capacity
int isFull(CircularBuffer *cb) {
  return cb->count == cb->capacity;
}

// Buffer is empty if count is empty
int isEmpty(CircularBuffer *cb) {
  return cb->count == 0;
}

// Insert a point at tail position and increment tail
// Caller needs to check if buffer is full beforehand
//   Otherwise, the head may be overwritten
void write(CircularBuffer *cb, Point p) {
  int tail = (cb->head + cb->count) % cb->capacity;
  cb->points[tail] = p;
  cb->count = (cb->count % cb->capacity) + 1;
}

// Read point in head position and increment
// Caller needs to check if buffer is empty beforehand
//   Otherwise, an undefined value may be read
void read(CircularBuffer *cb, Point *p) {
  Point tmp = cb->points[cb->head];
  *p = tmp;
  cb->head = (cb->head + 1) % cb->capacity; 
  cb->count = (cb->count + cb->capacity - 1) % cb->capacity;
}

int main(int argc, char **argv) {
  Point p = {1,3};
  
  CircularBuffer cb;
  initBuffer(&cb, BUF_SIZE);
  
  printf("Buffer Size:   \t%d\n", cb.capacity);
  printf("Index of Head: \t%d\n", cb.head);
  printf("Count:         \t%d\n", cb.count);
  printf("Full Buffer:   \t%d\n", isFull(&cb));
  printf("Empty Buffer:  \t%d\n\n", isEmpty(&cb));
  
  int i = 0;
  for (i=0; i < 2*BUF_SIZE; i++) {
    p.x = i;
    p.y = i;  
    if (!isFull(&cb)) {
       printf("Writing ...\tP:\t{%.1f,%.1f}\n", p.x, p.y);
       write(&cb, p);
       printf("Capacity:  \t%d\n", cb.count);
    }
    else printf("Buffer is full ...\n");
    
    if (!isEmpty(&cb)) {
      read(&cb, &p);
      printf("Reading ...\tP:\t{%.1f,%.1f}\n", p.x, p.y);
      printf("Capacity:  \t%d\n", cb.count);
    }
    else printf("Buffer is empty ...\n");
  }
  
  // printf("\n...Reading...\n");
  // for (i=0; i < 2*BUF_SIZE; i++) {
  //   if (!isEmpty(&cb)) {
  //     read(&cb, &p);
  //     printf("P:\t{%.1f,%.1f}\n", p.x, p.y);
  //   }
  //   else printf("Buffer is empty ...\n");
  // }
}