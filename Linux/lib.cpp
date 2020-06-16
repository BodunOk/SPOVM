#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <aio.h>

extern "C" bool ReadTestFile(const char* filePath, const char* message)
{
	puts("Read file - start");

	aiocb aioStructure;
	aioStructure.aio_offset = 0;
	aioStructure.aio_fildes = open(filePath, O_RDONLY);;
	aioStructure.aio_buf = (void*)message;
	aioStructure.aio_nbytes = 80;
	aioStructure.aio_sigevent.sigev_notify = SIGEV_NONE;
	aio_read(&aioStructure);
	while(aio_error(&aioStructure)) { usleep(100); }
	close(aioStructure.aio_fildes);
	puts("ReadT file - finish");
	return true;
}

extern "C" bool WriteTestFile(const char* filePath, const char* message, unsigned int size, unsigned int offset)
{
	puts("Write file - start");

	aiocb aioStructure;
	aioStructure.aio_offset = offset;
	aioStructure.aio_fildes = open(filePath, O_WRONLY);;
	aioStructure.aio_buf = (void*)message;
	aioStructure.aio_nbytes = size;
	aioStructure.aio_sigevent.sigev_notify = SIGEV_NONE;
	aio_write(&aioStructure);
	while(aio_error(&aioStructure)) { usleep(100); }
	close(aioStructure.aio_fildes);

	puts("Write file - finish");
	return true;
}
