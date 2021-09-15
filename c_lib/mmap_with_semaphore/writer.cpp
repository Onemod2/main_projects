#include <iostream>

#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char** argv) {

	sem_t *sm = sem_open(argv[1], O_CREAT, 0, 0);

	sem_wait( sm );

	int fd = shm_open("/test_sm_file.txt", O_CREAT | O_RDONLY, 0);	
	
	int *m = (int*)(mmap(0, sizeof(int) * 10, PROT_READ,
		 MAP_SHARED, fd, 0));
	
	for (std::size_t i = 0; i < 10; ++i) {
		std::cout << m[i] << std::endl;	
	}	

	close(fd);
	
	sem_post( sm );
	sem_close( sm );
	munmap(m, sizeof(int) * 10);

	sem_unlink(argv[1]);

	return 0;
}
