#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

#define BUF_SIZE 32

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
  cb->count    = 0;
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
  int N = 0;
  CircularBuffer cb;
  initBuffer(&cb, BUF_SIZE);
  
  if (argc > 1)
    N = atoi(argv[1]);
  
  double count = 0;
#pragma omp parallel sections
{
#pragma omp section
  {
    int i = 0;
    for (i = 0; i < N; i++) {
      double x = rand();
      double y = rand();
      x = x / RAND_MAX;
      y = y / RAND_MAX;
      Point p = {x, y};
      
#pragma omp flush(cb)
      while(isFull(&cb)) {
        // printf("Buffer is full ...\n");
#pragma omp flush
      }
      
#pragma omp critical
      write(&cb, p);
#pragma omp flush
    }
  } // end producer
  
#pragma omp section
  {
    int i = 0;
    for (i = 0; i < N; i++) {
      Point p;
      double dx, dy, delta, r;
      
#pragma omp flush(cb)
      while(isEmpty(&cb)) {
        // printf("Buffer is empty ...\n");
#pragma omp flush
      }
      
#pragma omp critical
      read(&cb, &p);
      
      dx = p.x - 0.5;
      dy = p.y - 0.5;
      delta = dx * dx + dy * dy;
      r = sqrt(delta);
      
// #pragma omp critical
//       {
        if (r <= 0.5)
          count = count + 1;
      // }
    }
  }
} // end parallel sections

  double PI = 4 * count / N;
  printf("%.10f\n", PI);
}