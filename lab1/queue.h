#include <stdlib.h>
#include <stdio.h>

struct queue_node {
  short value;
  struct queue_node *next;
};

typedef queue_node queue_node;

struct queue_t {
  queue_node *start;
  queue_node *end;
};

typedef queue_t queue_t;

void queue_push(queue_t queue, short value) {
  queue_node *ptr;
  ptr = malloc(sizeof(queue_node));
  ptr->next = NULL;
  if (ptr != NULL) {
    if (queue.start == NULL) {
      queue.start = ptr;
      queue.end = ptr;
    } else {
      queue.end->next = ptr;
      queue.end = ptr;
    }
  }
}

int queue_is_empty(queue_t queue) {
  return (queue.start == NULL);
}

short queue_top(queue_t queue) {
  if (queue_is_empty(queue)) return 0;
  return queue.start->value;
}

short queue_pop(queue_t queue) {
  if (queue_is_empty(queue)) return 0;
  short pop_value = queue_top(queue);
  queue_node *temp = queue.start;
  queue.start = queue.start->next;
  free(temp);
  return pop_value;
}
