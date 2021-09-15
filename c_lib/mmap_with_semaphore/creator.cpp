#include <iostream>

#include <unistd.h>
#include <semaphore.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/types.h>

int main(int argc, char **argv) {


	/*int fd = shm_open("/test", O_CREAT, O_RDWR);
	ftruncate(fd, sizeof(sem_t) + 1u);	

	if (errno == -1) {
		std::cout << "ERRNO\n";
	}	

	mmap(0, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);	
*/
	sem_t *sm = sem_open(argv[1], O_CREAT, 0640, 0);

	int n = 10;

	int fd = shm_open("/test_sm_file.txt", O_CREAT | O_RDWR, 0);
	ftruncate(fd, sizeof(int) * n);
	
	int *m = (int*)(mmap(0, sizeof(int) * n, PROT_READ | PROT_WRITE,
		 MAP_SHARED, fd, 0));

	for (std::size_t i = 0; i < 10; ++i) {
		m[i] = i * i;
	}

	std::cout << "smth" << std::endl;

	close(fd);
	munmap(m, sizeof(int) * n);

	sem_post( sm );
	sleep(3);
	sem_wait( sm );

	sem_close(sm);
	sem_unlink(argv[1]);	

	return 0;
}
