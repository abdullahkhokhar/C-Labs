#include <stdio.h>
#include <stdlib.h>


struct ll_node {
    int value;
    struct ll_node *next;
};


// This overly complex code reads integers from stdin and places them in
// a linked list. Then, it sums the items in the list and prints the result.
int main() {
    int user_inp;
    int sum;

    // Using a dummy head node
    struct ll_node *front = malloc(sizeof(struct ll_node));
    struct ll_node *current = front;

    // inital set the fronts value to 0
    front->value = 0;

    while (scanf("%d", &user_inp) != EOF) {
        current->next = malloc(sizeof(struct ll_node));
        current = current->next;

        current->value = user_inp;
        current->next = NULL;
    }

    struct ll_node *dummy_node = front;
    current = front;
    for (sum = 0; front != NULL; front = front->next) {
        sum += front->value;
    }
    //now free the data
    while(dummy_node != NULL){
      current = dummy_node->next;
      free(dummy_node);
      dummy_node = current;
    }

    printf("The sum of the inputs is %d.\n", sum);

    return 0;
}
