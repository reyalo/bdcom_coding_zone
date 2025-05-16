int os_demo_ip_cache_create_counter = 0;
int os_demo_ip_cache_delete_counter = 0;

void os_demo_cmd_impl_get_counter()
{
	printf("Create number is %d\n", os_demo_ip_cache_create_counter);
	printf("Delete number is %d\n", os_demo_ip_cache_delete_counter);
}