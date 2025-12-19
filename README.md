# VECTOR

---

## PRESENTATION

Dynamic array implementation in C99.

---

## PREREQUISITES

- C99 compiler (Clang, GCC, MSVC...)

---

## EXAMPLE

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./vector.h"

#ifndef strdup
char *strdup(const char *s) {
    size_t len = strlen(s) + 1;
    char *p = malloc(len);
    if (p) {
        memcpy(p, s, len); // or strcpy(p, s);
    }
    return p;
}
#endif /* !strdup */

typedef struct {
    char *name;
    int age;
} Person;

/* Destructor called for each element during clear/cleanup */
static void person_destructor(void *elem) {
    Person *p = elem;
    free(p->name);
}

int main(void) {
    Vector *people = vector_init(4, sizeof(Person), person_destructor);
    if (!people) {
        return 1;
    }

    Person p;

    p.name = strdup("Ada");
    p.age = 36;
    vector_push_back(people, &p);

    p.name = strdup("Dennis");
    p.age = 42;
    vector_push_back(people, &p);

    p.name = strdup("Ken");
    p.age = 50;
    vector_push_back(people, &p);

    /* Iterate */
    printf("People:\n");
    vector_foreach(Person, it, people) {
        printf("Name: %s, Age: %d\n", it->name, it->age);
    }

    /* Remove last element */
    Person removed;
    vector_pop_back(people, (void **)&removed);
    person_destructor(&removed);

    printf("\nAfter pop:\n");
    vector_foreach(Person, it, people) {
        printf("Name: %s, Age: %d\n", it->name, it->age);
    }

    vector_cleanup(people);
    return 0;
}
```

---
