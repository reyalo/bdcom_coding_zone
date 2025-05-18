#include <msgQLib.h>
#include <eventLib.h>
#include <taskLib.h>
#include <ip/socket.h>
#include <ip/inet.h>
#include <ip/msg.h>
#include <libfile/file_sys.h>

extern unsigned long Print(char *format, ...);
void os_demo_register_cmds(void);

#define OS_DEMO_DEFAULT_SIZE 14

typedef struct student {
    char name[OS_DEMO_DEFAULT_SIZE];
    char age;
    char gender;
} student_t;

extern uint32 sys_partmem_create(uint32 count, size_t size, char* name);
extern void* sys_partmem_alloc(uint32 ptid);
extern void sys_partmem_free(uint32 ptid, void* fptr);

static uint32 student_partition_id = 0;
static void* last_alloc_ptr = NULL;

void os_demo_mblk_handle(mblk_t *mb){
	return;
}

void os_demo_init(void)
{
	Print(")__________________________ReyaL(0)______________________(");
    os_demo_register_cmds();
    //os_demo_main_task_init();
	if (student_partition_id == 0) {
        student_partition_id = sys_partmem_create(10, sizeof(student_t), "student_partition");
        if (student_partition_id == 0) {
            Print("[os-demo] Failed to create student partition\n", 0, 0, 0, 0, 0, 0, 0);
        } else {
            Print("[os-demo] Created student partition with ID: 0x%X\n", student_partition_id, 0, 0, 0, 0, 0, 0);
        }
    }
    return;
}

// static void os_demo_main_task_init(void)
// {
//     if (student_partition_id == 0) {
//         student_partition_id = sys_partmem_create(10, sizeof(student_t), "student_partition");
//         if (student_partition_id == 0) {
//             Print("[os-demo] Failed to create student partition\n", 0, 0, 0, 0, 0, 0, 0);
//         } else {
//             Print("[os-demo] Created student partition with ID: 0x%X\n", student_partition_id, 0, 0, 0, 0, 0, 0);
//         }
//     }
//     return;
// }

uint32 os_demo_get_partition_id(void)
{
    return student_partition_id;
}

void* os_demo_get_last_alloc_ptr(void)
{
    return last_alloc_ptr;
}

void os_demo_set_last_alloc_ptr(void* ptr)
{
    last_alloc_ptr = ptr;
}

void os_demo_partition_alloc_handler(void)
{
    if (student_partition_id == 0) {
        Print("[os-demo] Partition not created.\n", 0, 0, 0, 0, 0, 0, 0);
        return;
    }

    void* ptr = sys_partmem_alloc(student_partition_id);
    if (ptr == NULL) {
        Print("[os-demo] Allocation failed.\n", 0, 0, 0, 0, 0, 0, 0);
        return;
    }

    last_alloc_ptr = ptr;
    Print("[os-demo] Allocated partition memory at address: %p\n", ptr, 0, 0, 0, 0, 0, 0);
}

void os_demo_partition_free_handler(void)
{
    if (student_partition_id == 0 || last_alloc_ptr == NULL) {
        Print("[os-demo] Nothing to free.\n", 0, 0, 0, 0, 0, 0, 0);
        return;
    }

    sys_partmem_free(student_partition_id, last_alloc_ptr);
    Print("[os-demo] Freed partition memory at address: %p\n", last_alloc_ptr, 0, 0, 0, 0, 0, 0);
    last_alloc_ptr = NULL;
}
