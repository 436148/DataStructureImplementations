// Program to show simple data structure implementations.

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

typedef enum STATUS
{
	STATUS_OK,
	STATUS_FAIL,
} STATUS;

// List implementation:

// Node for a doubly linked list.
struct list_node
{
	// Pointer to the previous node in the list.
	struct list_node* prev;

	// Pointer to the next node in the list.
	struct list_node* next;

	// The value of the node.
	// Typically implementations use a pointer to the data.
	// This implementation embeds the data into the node instead of using a pointer.
	// This type of list implementation is typically referred to as an intrusive linked list.
	int value;
};

// The linked list data structure contains a pointer to the head and tail list nodes.
struct list
{
	struct list_node* head;
	struct list_node* tail;
};

// Returns an empty linked list.
struct list list_create()
{
	struct list result;
	// Initialise the head and tail nodes to NULL.
	result.head = NULL;
	result.tail = NULL;
	return result;
}

// Returns 1 if the list is empty, 0 otherwise.
int list_is_empty(struct list const* List)
{
	// If the head node pointer is NULL, the list is empty.
	return List->head == NULL;
}

// Inserts the given value to the front of the list. Returns a pointer to the new node.
struct list_node* list_push_front(struct list* List, int Value)
{
	// Allocate enough heap memory for the new list node.
	struct list_node* newNode = malloc(sizeof(struct list_node));
	if (!newNode) return NULL;

	// Replace the list head with the new node.
	struct list_node* oldHead = List->head;
	List->head = newNode;

	// Update the tail node if needed.
	if (!List->tail) List->tail = newNode;

	// The old head needs to point back to the new head.
	if (oldHead) oldHead->prev = newNode;

	// The new head needs to point forward to the old head.
	newNode->next = oldHead;

	// Initialise the previous-node-pointer for the new node to NULL.
	newNode->prev = NULL;

	// Initialise the node value.
	newNode->value = Value;

	// Return the new node pointer.
	return newNode;
}

// Adds the given value to the end of the list. Returns a pointer to the new node.
struct list_node* list_push_back(struct list* List, int Value)
{
	// Allocate enough heap memory for the new list node.
	struct list_node* newNode = malloc(sizeof(struct list_node));
	if (!newNode) return NULL;

	// Replace the list tail with the new node.
	struct list_node* oldTail = List->tail;
	List->tail = newNode;

	// Update the head node if needed.
	if (!List->head) List->head = newNode;

	// The old tail needs to point forward to the new tail.
	if (oldTail) oldTail->next = newNode;

	// The new head needs to point back to the old tail.
	newNode->prev = oldTail;

	// Initialise the next-node-pointer for the new node to NULL.
	newNode->next = NULL;

	// Initialise the node value.
	newNode->value = Value;

	// Return the new node pointer.
	return newNode;
}

// Removes the node at the front of the list. Returns the value of the removed node.
int list_pop_front(struct list* List)
{
	// It is invalid to call list_pop_front if the list is empty.
	assert(!list_is_empty(List));

	// Retrieve a pointer to the node after the head.
	struct list_node* next = List->head->next;

	// And if that node exists, set its previous node pointer to NULL.
	if (next) next->prev = NULL;

	// Set the list head to the node after the list head.
	struct list_node* oldHead = List->head;
	List->head = next;

	// Update the tail node if needed.
	if (oldHead == List->tail) List->tail = NULL;

	// Save the value of the removed node so it can be returned.
	int value = oldHead->value;

	// Free the heap memory previously allocated for the removed node.
	free(oldHead);

	// Return the value of the removed node.
	return value;
}

// Clears the list, freeing memory allocated for its nodes.
void clear_list(struct list* List)
{
	// Iterate through each node of the list, freeing its memory.
	while (List->head)
	{
		struct list_node* next = List->head->next;
		free(List->head);
		List->head = next;
	}
	// Be sure to NULL the tail node pointer too.
	List->tail = NULL;
}

// Inserts the given value in the list before the node specified by Where.
// If Where is NULL, the value is appended to the list.
struct list_node* insert_list_item(struct list* List, struct list_node* Where, int Value)
{
	// If Where is NULL, append the new value.
	if (!Where) return list_push_back(List, Value);
	
	// Allocate enough heap memory for the new list node.
	struct list_node* newNode = malloc(sizeof(struct list_node));
	if (!newNode) return NULL;

	// Initialise the new node (except newNode->prev, which will be set later).
	newNode->value = Value;
	newNode->next = Where;

	// Set the node specified by Where to point back to the new node.
	struct list_node* prev = Where->prev;
	Where->prev = newNode;

	// Set the previous-node-pointer for the new node.
	newNode->prev = prev;

	if (prev)
	{
		// Set the previous node to point forward to the new node.
		prev->next = newNode;
	}
	else
	{
		// If the previous node does not exist, Where must be the head.
		// Therefore, the head pointer needs to be set to the new node.
		List->head = newNode;
	}

	return newNode;
}

// Erases the node specified by Where.
void erase_list_item(struct list* List, struct list_node* Where)
{
	assert(Where != NULL);

	// Get the previous and next node pointers for Where.
	struct list_node* prev = Where->prev;
	struct list_node* next = Where->next;

	if (prev)
	{
		// Set the previous node to point forward to the next node.
		prev->next = next;
	}
	else
	{
		// If the previous node does not exist, Where must be the head.
		// Therefore we need to NULL the head pointer.
		List->head = NULL;
	}

	if (next)
	{
		// Set the next node to point back to the previous node.
		next->prev = prev;
	}
	else
	{
		// If the next node does not exist, Where must be the tail.
		// Therefore we need to NULL the tail pointer.
		List->tail = NULL;
	}
}

// Sample using the doubly linked list.
STATUS test_list()
{
	printf("\nTesting list.\nAdding values from 0 to 9...\n");

	// Create a list.
	struct list list = list_create();

	// Add values from 0 to 9 to the list.
	for (int i = 0; i < 10; ++i)
	{
		if (!list_push_front(&list, i))
		{ // Memory error.
			clear_list(&list);
			return STATUS_FAIL;
		}
	}

	printf("Removing values from 0 to 9...\n");

	// Remove nodes from the list, outputting their valeus.
	while (!list_is_empty(&list))
	{
		printf("Removing value %d\n", list_pop_front(&list));
	}

	return STATUS_OK;
}

// Test more operations on the doubly linked list.
STATUS test_lists_advanced()
{
	printf("\nTesting list (advanced).\n");

	STATUS status = STATUS_OK;

	// Create a list.
	struct list list = list_create();

	for (int i = 0; i < 5; ++i)
	{
		int value = rand();
		if (!list_push_back(&list, value))
		{
			goto labelCleanup;
		}
		printf("Appended value %d to the list.\n", value);
	}

	struct list_node* p = list.head;
	for (int i = 0; i < 3; ++i)
	{
		// Move p to the next element in the list.
		p = p->next;
	}

	printf("Removing element %d from the list.\n", p->value);

	erase_list_item(&list, p);

	printf("The list now looks like this:\n");
	int i;
	for (i = 0, p = list.head; p; p = p->next, ++i)
	{
		printf("Element %d is %d.\n", i, p->value);
	}

	p = list.tail->prev;

	int value = rand();
	printf("Inserting value %d before value %d.\n", value, p->value);

	if (!(p = insert_list_item(&list, p, value)))
	{
		status = STATUS_FAIL;
		goto labelCleanup;
	}
	
	printf("The list now looks like this:\n");
	for (i = 0, p = list.head; p; p = p->next, ++i)
	{
		printf("Element %d is %d.\n", i, p->value);
	}

labelCleanup:
	clear_list(&list);
	return status;
}

// Sample using an array allocated to the heap:

STATUS test_dynamic_array()
{
	printf("\nTesting dynamic array\n");

	// Allocate enough heap memory for 10 integers.
	int* arr = malloc(sizeof(int) * 10);
	if (!arr) return STATUS_FAIL; // memory error

	// Set each element in the array to random values.
	for (int i = 0; i < 10; ++i)
	{
		int value = rand();
		arr[i] = value;
		printf("Set array element %d to %d.\n", i, value);
	}

	// Let's say we want 15 integers instead of 10.
	// We need to expand the array.

	printf("Expanding the array by 5 elements...\n");

	int* newArr = realloc(arr, sizeof(int) * 15);
	if (!newArr)
	{ // memory error
		free(arr);
		return STATUS_FAIL;
	}

	arr = newArr;

	printf("Setting the 5 new elements...\n");
	for (int i = 10; i < 15; ++i)
	{
		int value = rand();
		arr[i] = value;
		printf("Set array element %d to %d.\n", i, value);
	}

	printf("Printing the array...\n");
	for (int i = 0; i < 15; ++i)
	{
		printf("Element %d is %d.\n", i, arr[i]);
	}

	free(arr);

	return STATUS_OK;
}

// Bubble sort and binary search algorithms on an array:

void test_sorting_and_searching_array()
{
	int arr[16]; // array of 16 integers allocated on the stack.
	const int size = sizeof(arr) / sizeof(int);

	// Populate array with random numbers.
	for (int i = 0; i < size; ++i)
	{
		arr[i] = rand();
	}

	printf("\nPopulated array with random integers.\nSorting the array...\n");

	// Sort the array using a bubble sort algorithm.
	for (int i = 0; i < size - 1; ++i)
	{
		int swapped = 0; // indicates whether elements were swapped.
		for (int j = 0; j < size - 1; ++j)
		{
			if (arr[j] > arr[j + 1])
			{
				printf("Swapping %d with %d. ", arr[j], arr[j + 1]);

				// swap arr[j] with arr[j + 1]
				int tmp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = tmp;
				swapped = 1; // set swapped to 1 (true)
			}
		}
		if (!swapped)
		{
			// Break out of the loop if swapping did not occur.
			break;
		}
	}
	printf("\n");

	// Print the contents of the sorted array.
	printf("Sorted array:\n");
	for (int i = 0; i < size; ++i)
	{
		printf("Array element %d is %d.\n", i, arr[i]);
	}

	// Search for number in the array using a binary search.
	int target = 11942; // The number to search for in the sorted array.

	int l = 0;
	int r = size - 1;
	while (l != r)
	{
		int mid = (int)ceilf((l + r) / 2.0f);
		if (arr[mid] > target)
		{
			r = mid - 1;
		}
		else
		{
			l = mid;
		}
	}
	if (arr[l] == target)
	{
		printf("Number %d found at element %d.\n", target, l);
	}
	else
	{
		printf("Number %d not found.\n", target);
	}
}

// Queue implementation (using linked list):

struct queue_node
{
	struct queue_node* next; // Pointer to the next node in the queue.
	int value; // The value of the node.
};

// Queue data structure implemented using a linked list.
struct queue
{
	struct queue_node* head; // Pointer to the first element in the queue.
	struct queue_node* tail; // Pointer to the last element in the queue.
};

// Returns an empty queue.
struct queue create_queue()
{
	struct queue result;
	result.head = NULL;
	result.tail = NULL;
	return result;
}

// Returns 1 if the queue is empty, 0 otherwise.
int queue_is_empty(struct queue const* Queue)
{
	// If the head node pointer is NULL, the queue is empty.
	return Queue->head == NULL;
}

// Pushes the given value to the back of the queue.
// Returns STATUS_FAIL if an error occured allocating memory.
STATUS enqueue(struct queue* Queue, int Value)
{
	// Allocate storage for the new tail.
	struct queue_node* newTail = malloc(sizeof(struct queue_node));
	if (!newTail) return STATUS_FAIL;

	// Initialise the new tail.
	newTail->next = NULL;
	newTail->value = Value;

	// The tail is replaced with the new tail.
	struct queue_node* oldTail = Queue->tail;
	Queue->tail = newTail;

	// The old tail points forward to the new tail.
	if (oldTail) oldTail->next = newTail;

	// Update the front of the queue if needed.
	if (!Queue->head) Queue->head = newTail;

	return STATUS_OK;
}

// Removes the value at the front of the queue.
// Returns the removed value.
int dequeue(struct queue* Queue)
{
	// It is invalid to call dequeue if the queue is empty.
	assert(!queue_is_empty(Queue));

	// Replace the the head with the next node.
	struct queue_node* oldHead = Queue->head;
	struct queue_node* newHead = oldHead->next;
	Queue->head = newHead;

	// NULL the tail node pointer if needed.
	if (!newHead) Queue->tail = NULL;

	int value = oldHead->value; // Store the value of the removed node.
	free(oldHead); // Free previously allocated memory for the removed node.
	return value; // Return the value of the removed node.
}

// Returns the value at the front of the queue.
int peek_queue(struct queue const* Queue)
{
	// It is invalid to call peek_queue if the queue is empty.
	assert(!queue_is_empty(Queue));

	// Return the value of the head node.
	return Queue->head->value;
}

// Clears the queue, freeing memory allocated for its nodes.
void clear_queue(struct queue* Queue)
{ // This is the same behaviour as the clear_list function.
	while (Queue->head)
	{
		struct queue_node* next = Queue->head->next;
		free(Queue->head);
		Queue->head = next;
	}
	Queue->tail = NULL;
}

STATUS test_queue()
{
	printf("\nTesting queue\n");

	STATUS status = STATUS_OK;

	// Create the queue.
	struct queue queue = create_queue();

	// Queue up 5 random values:
	for (int i = 0; i < 5; ++i)
	{
		int value = rand();
		if ((status = enqueue(&queue, value)) != STATUS_OK)
		{
			// If an error occured, jump to the end.
			goto labelCleanup;
		}
		printf("Queued up %d.\n", value);
	}

	// Use the peek_queue function.
	printf("The front of the queue is %d.\n", peek_queue(&queue));

	// Dequeue 3 values.
	for (int i = 0; i < 3; ++i)
	{
		printf("Dequeued %d.\n", dequeue(&queue));
	}

	// Queue up 2 more random values:
	for (int i = 0; i < 2; ++i)
	{
		int value = rand();
		if ((status = enqueue(&queue, value)) != STATUS_OK)
		{
			// If an error occured, jump to the end.
			goto labelCleanup;
		}
		printf("Queued up %d.\n", value);
	}

labelCleanup:
	// Free storage for the queue and return the success/error code.
	clear_queue(&queue);
	return status;
}

// Stack implementation (using linked list):

struct stack_node
{
	struct stack_node* next;
	int value;
};

// Stack data structure implemented using a linked list.
// Observe that, for a stack, we needn't a tail node pointer.
struct stack
{
	struct stack_node* head;
};

// Returns an empty stack.
struct stack create_stack()
{
	struct stack result;
	result.head = NULL;
	return result;
}

// Returns 1 if the stack is empty, 0 otherwise.
int stack_is_empty(struct stack const* Stack)
{
	// If the head node pointer is NULL, the stack is empty.
	return Stack->head == NULL;
}

// Pushes the given value to the top of the stack.
STATUS push(struct stack* Stack, int Value)
{
	// Allocate memory for the new head.
	struct stack_node* newHead = malloc(sizeof(struct stack_node));
	if (!newHead) return STATUS_FAIL;
	
	// Set the value for the new head.
	newHead->value = Value;

	// Set the new head.
	struct stack_node* oldHead = Stack->head;
	Stack->head = newHead;

	// Make the new head point forward to the old head.
	newHead->next = oldHead;

	return STATUS_OK;
}

// Removes the value at the top of the stack.
// Returns the removed value.
int pop(struct stack* Stack)
{
	// It is invalid to call pop on an empty stack.
	assert(!stack_is_empty(Stack));

	// Set the head to the next node.
	struct stack_node* oldHead = Stack->head;
	Stack->head = Stack->head->next;

	// Store the value of the old head so we can later return it.
	int value = oldHead->value;

	// Free memory previously allocated for the old head.
	free(oldHead);

	// Return the value of the removed node.
	return value;
}

// Returns the value at the top of the stack.
int peek_stack(struct stack const* Stack)
{
	// It is invalid to call peek_stack if the stack is empty.
	assert(!stack_is_empty(Stack));

	// Return the value of the head node.
	return Stack->head->value;
}

// Clears the stack, freeing memory allocated for its nodes.
void clear_stack(struct stack* Stack)
{
	while (Stack->head)
	{
		struct stack_node* next = Stack->head->next;
		free(Stack->head);
		Stack->head = next;
	}
}

// Example using the push and pop functions.
STATUS test_stack()
{
	printf("\nTesting stack.\n");

	// Create the stack.
	struct stack stack = create_stack();

	STATUS status = STATUS_OK;

	// Push 3 values to the top of the stack.
	for (int i = 0; i < 3; ++i)
	{
		int value = rand();
		if ((status = push(&stack, value)) != STATUS_OK)
		{
			goto labelCleanup;
		}
		printf("Pushed %d.\n", value);
	}

	// Pop 3 values from the top of the stack.
	for (int i = 0; i < 3; ++i)
	{
		printf("Popped %d.\n", pop(&stack));
	}

labelCleanup:
	clear_stack(&stack);
	return status;
}

int main()
{
	// Run all tests.
	if (test_list() != STATUS_OK) return 1;
	if (test_lists_advanced() != STATUS_OK) return 1;
	if (test_dynamic_array() != STATUS_OK) return 1;
	test_sorting_and_searching_array();
	if (test_queue() != STATUS_OK) return 1;
	if (test_stack() != STATUS_OK) return 1;
	return 0;
}
