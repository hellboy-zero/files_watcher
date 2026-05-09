#include <stdio.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define EVENT_SIZE 		sizeof(struct inotify_event)
#define BUFFER_LENGTH 	1024 * (EVENT_SIZE + 16)
#define MAX_TRACK 		1024

typedef struct 
{
	char name[256];
	uint32_t mask;
	time_t last_time;
} EventCache;

EventCache cache[MAX_TRACK];


int is_duplicate(const char *name , uint32_t mask)
{
	time_t t_now = time(NULL);
	for (int i = 0; i < MAX_TRACK; i++)
	{
		if (cache[i].name[0] == '\0')
			continue;

		if (strcmp(cache[i].name , name) == 0 && 
												cache[i].mask == mask && 
												(t_now - cache[i].last_time) < 1)
		{
			return 1;
		}
	}
	return 0;
}

void update_cache(const char *name , uint32_t mask)
{
	time_t t_now = time(NULL);
	for (int i = 0; i < MAX_TRACK; i++)
	{
		if (cache[i].name[0] == '\0' || strcmp(cache[i].name , name)== 0)
		{
			strcpy(cache[i].name , name);
			cache[i].mask = mask;
			cache[i].last_time = t_now;
			return;
		}
	}
}

int main(int argc , char *argv[]) 
{

	printf("files watcher is running....\n");
	
	int fd = inotify_init();

	if (fd < 0)
	{
		perror("inotify_init");
		return 1;
	}
	
	char BUFFER[BUFFER_LENGTH];

	int wd = inotify_add_watch(fd , ".",  IN_ALL_EVENTS);


	while (1)
	{

	int length = read(fd, BUFFER , BUFFER_LENGTH);
	
	if (length < 0)
	{
		perror("length");
		return 1;
	}

	
	int i=0;
	while ( i <length)
	{
		
		struct inotify_event *event = (struct inotify_event *)&BUFFER[i];
		
		if (event -> len) 
		{
			if((event->mask & IN_CREATE) && !is_duplicate(event->name, IN_CREATE))
			{
				time_t raw_time;
				time(&raw_time);
				printf("The file named {%s} trigerred event {CREATE} at %s\n", event->name ,ctime(&raw_time)  );
				update_cache(event->name, IN_CREATE);
			}

			else if ((event->mask & IN_DELETE) && !is_duplicate(event->name, IN_DELETE))
			{
				time_t raw_time;
				time(&raw_time);
				printf("The file named {%s} trigerred event {DELETE} at %s\n", event->name ,ctime(&raw_time)  );
				update_cache(event->name, IN_DELETE);

			}

			else if ((event->mask  & IN_MODIFY) && !is_duplicate(event->name, IN_MODIFY))
			{
				time_t raw_time;
				time(&raw_time);
				printf("The file named {%s} trigerred event {MODIFY} at %s\n", event->name ,ctime(&raw_time)  );
				update_cache(event->name, IN_MODIFY);

			}
			
			else if ((event->mask  & IN_ACCESS) && !is_duplicate(event->name, IN_ACCESS))
			{
				time_t raw_time;
				time(&raw_time);
				printf("The file named {%s} trigerred event {ACCESS} at %s\n", event->name ,ctime(&raw_time)  );
				update_cache(event->name, IN_ACCESS);

			}

			else if ((event->mask  & IN_OPEN) && !is_duplicate(event->name, IN_OPEN))
			{
				time_t raw_time;
				time(&raw_time);
				printf("The file named {%s} trigerred event {OPEN} at %s\n", event->name ,ctime(&raw_time)  );
				update_cache(event->name, IN_OPEN);

			}

			else if ((event->mask  & IN_CLOSE) && !is_duplicate(event->name, IN_CLOSE))
			{
				time_t raw_time;
				time(&raw_time);
				printf("The file named {%s} trigerred event {CLOSE} at %s\n", event->name ,ctime(&raw_time)  );
				update_cache(event->name, IN_CLOSE);

			}

			
			else if ((event->mask  & IN_ATTRIB) && !is_duplicate(event->name, IN_ATTRIB))
			{
				time_t raw_time;
				time(&raw_time);
				printf("The file named {%s} trigerred event {ATTRIB} at %s\n", event->name ,ctime(&raw_time)  );
				update_cache(event->name, IN_ATTRIB);

			}
			
		}
		

		i += EVENT_SIZE + event-> len;
	}
}
	
	(void) inotify_rm_watch(fd , wd);
	(void) close(fd);
	
	

	return 0;
}	
