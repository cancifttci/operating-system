//OPERATING SYSTEMS ASSIGNMENT III Izmir Istitute of Technology
//Can ÇİFTÇİ
//270201080 - 30/04/2022

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

#define SEC_TO_USEC_FACTOR 1000000

pthread_mutex_t mutex_dequeue;      //mutex variable for dequeue
pthread_cond_t cond;                //condition variable

int no_more_task = 0;    // global flag for no more task

typedef struct list_node_s
{
    int data;
    struct list_node_s *next;
}
list_node_s;

typedef struct linked_list_s    
{
    int length;
    struct list_node_s *head;
    struct list_node_s *tail;
}
linked_list_s;


typedef struct task_node_s
{
    int task_num;
    int task_type;
    int value;
    struct task_node_s *next;
}
task_node_s;

typedef struct task_queue_s
{
    task_node_s *head;
    task_node_s *tail;
    int length;
}
task_queue_s;

task_queue_s *create_task_queue()       // creates task queue returns queue struct pointer in type of task_queue_s
{                                       // initializes 0 by default
    task_queue_s *queue;
    queue = malloc(sizeof(task_queue_s));
    queue->head = 0;
    queue->tail = 0;
    queue->length = 0;
    return queue;
}

linked_list_s *create_linked_list()      // creates linked list returns struct pointer in type of lisnked_list_s
{                                        // initializes 0 by default
    linked_list_s *list;
    list = malloc(sizeof(linked_list_s));
    list->head = 0;
    list->length = 0;
    list->tail = 0;
    return list;
}

task_queue_s *queue;                       //global variable queue
linked_list_s *list;                       //global variable list

void task_queue(task_queue_s *task_queue, int num_of_task)
{   
    for (int i = 0; i < num_of_task; i++)       //generates task amount of number of task 
    {
        int task_num = i + 1;
        int task_type = (rand() % 3) + 1;
        int value = (rand() % 500) + 1;         // initialize with random numbers
        task_enqueue(queue, task_num, task_type, value);
        pthread_cond_signal(&cond);             // condition signal for every single task
    }
    no_more_task = 1;                           //flag indicates that there were no more task
    pthread_cond_broadcast(&cond);              // since generating task is done broadcasting all worker threads

}

void task_enqueue(task_queue_s *queue, int task_num, int task_type, int value)
{
    task_node_s *node;
    node = malloc(sizeof(task_node_s));             // creating node for enqueue
    node->next = 0;
    node->task_num = task_num;
    node->task_type = task_type;
    node->value = value;
    if (queue->head == 0)           // if queue has no element
    {
        queue->head = node;
    }
    else                    // adding to the rear
    {
        queue->tail->next = node;
    }
    queue->tail = node;
    queue->length = queue->length + 1;    // adding to counter length
}

void task_dequeue(task_queue_s *queue, long my_rank, int* task_num_p, int* task_type_p, int* value_p)
{
    if (queue->length == 0)     // if no element for dequeue
    {
        return;             
    }
    task_node_s *node;
    node = queue->head;             // pops from front
    queue->head = node->next;
    *task_num_p = node->task_num;
    *task_type_p = node->task_type;
    *value_p = node->value;
    queue->length = queue->length - 1;      //decrement length
}

int insert(linked_list_s *list, int value)          // insert operation for linked list
{
    int temp = search_list(list,value);             // first it searchs for the sake of no duplication
    list_node_s *node;
    node = malloc(sizeof(node));                    // creating node for insert
    node->data = value;
    node->next = 0;
    list->length = list->length + 1;
    if (temp == value)                              //if exist
    {
        printf("Item already exist in the list\n");
        return 2;
    }
    else if (list->head == 0)                       // if list is empty
    {   
        list->head = node;
        list->tail = node;
        return 0;
    }
    else
    {
        if (value<=list->head->data)                // if value < front node data
        {
            node->next = list->head;
            list->head = node;
        }
        else if (value>=list->tail->data)           // if value > tail node data
        {
            list->tail->next = node;
            list->tail = node;
        }
        else
        {   
            list_node_s *current_node;                  //if value is in between tail and head data 
            current_node = list->head->next;
            list_node_s *last_node_before_current;
            last_node_before_current = list->head;
            while (current_node->next != 0)             // traversing
            {
                if (current_node->data>value)
                {
                    list_node_s *replace_node;                  // basic operations for adding node
                    node->next = current_node;
                    last_node_before_current->next = node;
                    break;
                }
        
                last_node_before_current = current_node;
                current_node = current_node->next;
            }
        }
        return 0;
        
    }
    
}
// task enqueue da mutex
int delete(linked_list_s *list, int value)      
{                                           // delete for list operaiton
    if (list->head == 0)            // id no elemnt exist
    {
        printf("No element found in the list\n");
        return 1;
    }
    else
    {   
        list_node_s *current, *prev;
        current = list->head;
        if (current->data == value)    // current data that equal value param
        {   
            list->head = list->head->next;
            free(current);
            list->length = list->length - 1;
            return 0;
        }
        while (current->next != 0 && current->data != value)    // traversing until value occurs in the list 
        {
            prev = current;
            current = current->next;
        }
        if (current == 0)           // if current node is empty
        {
            printf("No such element in the list \n");
            return 2;
        }
        if (current->data != value)             //checks that current data matches with value
        {
            return 2;
        }
        
        prev->next = current->next;              // removing node
        free(current);
        list->length = list->length - 1;            // decrementing length
        
        if (list->length == 0)
        {
            list->head = 0;
        }
        return 0;
         
    }
    
}

int search_list(linked_list_s *list, int value)
{
    list_node_s *temp = list->head;             // basic search operaiton on linked list
    int val = value;
    if (temp == 0)
    {
        printf("No elements in the list\n");
    }
    else
    {   
        while (temp != 0)
        {
            if (temp->data == value)
            {
                return val;
            }
            temp = temp->next;
        }
    }
    
}

void* routine(void* arg)        // takes arg for thread number
{   
    while (1)           // while true for threads works forever until the tasks are done.
    {
        pthread_mutex_lock(&mutex_dequeue);             // mutexing task_dequeue. because we worked on single queue and
        while (queue->length == 0 && !no_more_task)     // since all ooperaitons on the queue and the list can cause 
        {                                               // race condition.
            pthread_cond_wait(&cond, &mutex_dequeue);       //This while loops ensures that all the task are done in the 
        }                                               //queue and there will be no more task.

        int task_num, task_type, value;                 //
        task_dequeue(queue, 0, &task_num, &task_type, &value);  //dequeue task
        if (task_type == 1) // INSERT               // this if else block determining the task type
        {
            if (insert(list, value) == 0)                   //checking return value
            {
                printf("Thread %d       Task number : %d        Value:     %d is inserted\n",
                (int*)arg, task_num, value);
            }
            else
            {
                printf("Thread %d       Task number : %d        Value:     %d is not inserted\n",
                (int*)arg, task_num, value);
            }

        }
        else if (task_type == 2) // DELETE
        {
            if (delete(list, value) == 0)           //checking return value
            {
                printf("Thread %d       Task number : %d        Value:     %d is deleted\n",
                (int*)arg, task_num, value);
            }
            else
            {
                printf("Thread %d       Task number : %d        Value:     %d is not deleted\n",
                (int*)arg, task_num, value);
            }
        }
        else if (task_type == 3) //SEARCH
        {
            
            if (search_list(list, value) == 0)          //checking return value
            {
                printf("Thread %d       Task number : %d        Value:     %d is found\n",
                (int*)arg, task_num, value);
            }
            else
            {
                printf("Thread %d       Task number : %d        Value:     %d is not found\n",
                (int*)arg, task_num, value);
            }
        }
        pthread_mutex_unlock(&mutex_dequeue);       // unlocking mutex for other threads

        if (no_more_task && queue->length == 0)     // this if for breaking while true loop. If no more task and 
        {                                           // queue is empty it will break the loop
            break;
        }
        
    }
    
}

int main(int argc, char const *argv[])
{   
    int user_thread_number = atoi(argv[1]);         // user args for number of threads and tasks
    int user_task_number = atoi(argv[2]);

    srand(time(NULL));              // for random numbers
    pthread_t threads[user_thread_number];           // thread array
    pthread_mutex_init(&mutex_dequeue,NULL);        // initialize the mutex variable
    pthread_cond_init(&cond,NULL);              // initialize the condition variable
    queue = create_task_queue();         // creating task_queue and linked_list
    list = create_linked_list();

    struct timeval check_point;     // for calculating time. copied from lab
    gettimeofday(&check_point, NULL);
    long beginning_time_in_usec = (long) check_point.tv_sec * SEC_TO_USEC_FACTOR +  (long) check_point.tv_usec;

    for (int i = 0; i < user_thread_number; i++)
    {
        pthread_create(&threads[i],NULL,routine,(void*) i);         // creation of threads. with the number of
    }                                                               // user input
    task_queue(queue,user_task_number);                           // calling task queue function in main
    for (int i = 0; i < user_thread_number; i++) 
    {                                               // joining the threads. but the important point is that we should 
        pthread_join(threads[i],NULL);      // avoid joining immediately after creation. because main thread will wait
    }                                       // the workers execution and workers wait the task from main for execution
                                            // at the same time
    
    pthread_mutex_destroy(&mutex_dequeue);      // destroying cond and mutex variables.
    pthread_cond_destroy(&cond);


    gettimeofday(&check_point, NULL);           // for calculating time. copied from lab
    long end_time_in_usec = (long) check_point.tv_sec * SEC_TO_USEC_FACTOR +  (long) check_point.tv_usec;

    list_node_s *traverse_node = list->head;
    printf("\n ------------------------------FINAL LIST-------------------------------- \n");
    while (traverse_node !=0)                                       // printing final list
    {
        printf("%d, ",traverse_node->data);
        traverse_node = traverse_node->next;
    }
    
    double elapsed_time_in_sec = (end_time_in_usec - beginning_time_in_usec) / (double) SEC_TO_USEC_FACTOR;
    printf("\n\n[INFO] Elapsed time in seconds: %f\n", elapsed_time_in_sec);  

    return 0;
}
