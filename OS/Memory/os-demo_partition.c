#include <msgQLib.h>
#include <eventLib.h>
#include <taskLib.h>
#include <ip/socket.h>
#include <ip/inet.h>
#include <ip/msg.h>
#include <libfile/file_sys.h>

#define OS_DEMO_DEFAULT_SIZE 14
#define MAX_ALLOCATIONS 10  // Maximum number of allocations to track

typedef struct student {
    char name[OS_DEMO_DEFAULT_SIZE];
    char age;
    char gender;
} student_t;

extern uint32 sys_partmem_create(uint32 count, size_t size, char* name);
extern void* sys_partmem_alloc(uint32 ptid);
extern void sys_partmem_free(uint32 ptid, void* fptr);
extern unsigned long Print(char *format, ...);
void os_demo_register_cmds(void);
void os_demo_main_task_init();

static uint32 student_partition_id = 0;
static void* allocated_ptrs[MAX_ALLOCATIONS];  // Array to store allocated pointers
static int alloc_count = 0;                    // Counter for number of allocations

void os_demo_mblk_handle(mblk_t *mb){
	return;
}

void os_demo_init(void)
{
	Print(")__________________________ReyaL(0)______________________(");
    os_demo_register_cmds();
    os_demo_main_task_init();
	
    return;
}

void os_demo_main_task_init(){

	if (student_partition_id == 0) {
        student_partition_id = sys_partmem_create(10, sizeof(student_t), "student_partition");
        if (student_partition_id == 0) {
            Print("[os-demo] Failed to create student partition\n", 0, 0, 0, 0, 0, 0, 0);
        } else {
            Print("[os-demo] Created student partition with ID: 0x%X\n", student_partition_id, 0, 0, 0, 0, 0, 0);
        }
    }
}

void os_demo_partition_alloc_handler(void)
{
    if (student_partition_id == 0) {
        Print("[os-demo] Partition not created.\n", 0, 0, 0, 0, 0, 0, 0);
        return;
    }

    if (alloc_count >= MAX_ALLOCATIONS) {
        Print("[os-demo] Maximum allocations reached.\n", 0, 0, 0, 0, 0, 0, 0);
        return;
    }

    void* ptr = sys_partmem_alloc(student_partition_id);
    if (ptr == NULL) {
        Print("[os-demo] Allocation failed.\n", 0, 0, 0, 0, 0, 0, 0);
        return;
    }

    allocated_ptrs[alloc_count++] = ptr;
    Print("[os-demo] Allocated partition memory at address: %p (Total: %d)\n", 
          ptr, alloc_count, 0, 0, 0, 0, 0);
}

void os_demo_partition_free_handler(void)
{
    if (student_partition_id == 0 || alloc_count == 0) {
        Print("[os-demo] Nothing to free.\n", 0, 0, 0, 0, 0, 0, 0);
        return;
    }

    // Free the last allocated block
    alloc_count--;
    void* ptr = allocated_ptrs[alloc_count];
    sys_partmem_free(student_partition_id, ptr);
    allocated_ptrs[alloc_count] = NULL;
    
    Print("[os-demo] Freed partition memory at address: %p (Remaining: %d)\n", 
          ptr, alloc_count, 0, 0, 0, 0, 0);
}

