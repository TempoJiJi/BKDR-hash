/***********************************
    A Simple BKDRhash sample
************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct _list_t{
    char *str;
    struct _list_t *next;
}list_t;

typedef struct _hash_table_t{
    int size;		/* the size of the table */
    list_t **table;	/* the table elements */
}hash_table_t;

hash_table_t *create_hash_table(int size)
{
    int i;
    hash_table_t *new_table;
    if(size<1) return NULL; // invalid size of table

    new_table = malloc(sizeof(hash_table_t));
    new_table->table = malloc(sizeof(list_t *) * size);
    /* Attempt to allocate memory for the table*/
    if( new_table == NULL )
	return NULL; 

    /* Attempt to allocate memory for the table itself */
    if( new_table->table == NULL )
	return NULL;

    /* initialize the elements of the table */
    for(i=0;i<size;i++)
	new_table->table[i] = NULL;
    
    new_table->size = size;
    return new_table;   
}

unsigned long hash(hash_table_t *hashtable , char *str)
{
    unsigned int hashval = 0;	//we start our hash value at 0 */
    unsigned int seed = 31;
    while(*str)
	hashval = hashval * seed + (*str++);
    return hashval % hashtable->size;
}

list_t *lookup_string(hash_table_t *hashtable, char *str)
{
    list_t *list;
    unsigned int hashval = hash(hashtable , str);   //count the hashvalue

    /* Go to the correct list based on the hashvalue and see 
	the string is in the list or not */
    for(list = hashtable->table[hashval] ; list!=NULL ; list = list->next){
	if(strcmp(str,list->str) == 0)
	    return list;
    }
    return NULL;
}

int add_string(hash_table_t *hashtable, char *str)
{
    list_t *new_list;
    list_t *current_list;
    unsigned int hashval = hash(hashtable, str);
    
    new_list = (list_t*) malloc(sizeof(list_t));
    /* Attempt to allocate memory for list */
    if( new_list == NULL)
	return 1;
    
    /* Check does the item already exist? */
    current_list = lookup_string(hashtable , str);
    if(current_list != NULL) return 2;

    /* Insert into list */
    new_list->str = strdup(str);
    new_list->next = hashtable->table[hashval];
    hashtable->table[hashval] = new_list;
    return 0;

}

void free_table(hash_table_t *hashtable)
{
    int i;
    list_t *list, *temp;

    if (hashtable==NULL) return;

    for(i=0; i<hashtable->size; i++) {
        list = hashtable->table[i];
        while(list!=NULL) {
            temp = list;
            list = list->next;
            free(temp->str);
            free(temp);
        }
    }

    /* Free the table itself */
    free(hashtable->table);
    free(hashtable);
}

int main()
{
    FILE *fp = fopen("name.txt","r");	
    char *str;
    hash_table_t *my_hash_table;
    int size_of_table = 12;	
    list_t *item;
    int count=0;    //check how many item does not match
    
    /* Create a new hash table */
    my_hash_table = create_hash_table(size_of_table);
    
    /* Add the string to hash table */
    while( (fscanf(fp,"%s",str )) !=EOF)
	add_string(my_hash_table,str);

    fclose(fp);

    fp = fopen("input.txt","r");    
    while( (fscanf(fp,"%s",str)) != EOF ){
	    item = lookup_string (my_hash_table , str);
	    if(item == NULL){
		    printf("Can't found %s \n",str);
		    count++;
	    }
	    else
		    printf("Found %s - %s\n",str , item->str);
    }

    printf("%d\n",count);
    fclose(fp);
    
    /* Delete the table */
    free_table(my_hash_table);
}
