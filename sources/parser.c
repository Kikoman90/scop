
#include "scop.h"

static int  readFile(const char *file, int *fd)
{
    if ((*fd = open(file, O_RDWR)) == -1)
	{
        char *errorMsg = ft_strjoin("open error: ", strerror(errno));
		ft_putendl(errorMsg);
        free(errorMsg);
		return (-1);
	}
	
	return (*fd);
}

static void parseWavefrontObj(char *data, size_t size, unsigned int seed)
{
    ft_strword(data, &seed);
    /*while (seed < size && data[seed])
    {

        if (data[seed] == '#')
        {
            //comment
        } else if (data[seed] == 'o')
        {
            //create object, get indices
        }

        while (data[seed] != '\n' || data[seed] != '\0') {
            seed++;
        }
        seed++;
    }*/
    // groups
    // get vertices
    // get triangles -> build index list
    // get materials and link them to objects

}

void        parseFile(const char *path)
{
    int     fd;
    size_t  fsize;
    char    *data;

    if (readFile(path, &fd) != -1 && (fsize = fileSize(fd)) != -1)
    {
        if ((data = (char *)mmap(NULL, fsize, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
        {
            char *errorMsg = ft_strjoin("mmap error: ", strerror(errno));
		    ft_putendl(errorMsg);
            free(errorMsg);
        }
        else
        {
            data[fsize - 1] = '\0';
            ft_putendl(data);
            parseWavefrontObj(data, fsize, 0);
            if (munmap(data, fsize) == -1)
            {
                char *errorMsg = ft_strjoin("munmap error: ", strerror(errno));
		        ft_putendl(errorMsg);
                free(errorMsg);
            }
        }
        close(fd);
    }
}


// flat and smooth shading. -> no need for 1 normal per vertex, need for 1 normal per triangle