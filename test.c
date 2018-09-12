#include <string.h>
#include <stdio.h>
#include "map.h"

int cmp(const void *first, const void *second) 
{
    return strcmp((char *)first, (char *)second);
}

int main(int argc, char *argv[])
{
    map *test_map = construct_map(cmp);
    put_map(test_map, "name", "Vadmir Putin");
    put_map(test_map, "job", "President of Russia");
    put_map(test_map, "wealth", "Unknown");
    put_map(test_map, "nationality", "Russian");
    put_map(test_map, "about", 
            "The President of Russia for 3 consequtive terms even though the "
            "Russian consitution stipulates that the President can only run for 2" );

    printf("%s\n", get_map(test_map, "name"));
    printf("%s\n", get_map(test_map, "job"));
    printf("%s\n", get_map(test_map, "wealth"));
    printf("%s\n", get_map(test_map, "nationality"));
    printf("%s\n", get_map(test_map, "about"));
    
    
    delete_map(test_map, "name");
    printf("%s\n", get_map(test_map, "job"));
    printf("%s\n", get_map(test_map, "wealth"));
    printf("%s\n", get_map(test_map, "nationality"));
    printf("%s\n", get_map(test_map, "about"));

    return 0;
}