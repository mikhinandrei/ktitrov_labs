#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int opt = 0;
    long option;
    char *file_path;

    int p, l, s, c;

    while ((opt = getopt(argc,argv,"plsc")) != -1) {
        switch( opt ) {
	    case 'p':
		p=1;
		break;
            case 'l':
		l=1;
		break;
	    case 's':
		s=1;
		break;
	    case 'c':
		c=1;
		break;
        }
    }

    file_path = argv[optind];

    if (p==1) {
	option = pathconf(file_path, _PC_PATH_MAX);
	printf("Max size of filename %ld bytes\n", option);
    }
    if (l==1) {
	option = pathconf(file_path, _PC_LINK_MAX);
	printf("Max number of file links %ld\n", option);
    }
    if (s==1) {
	option = pathconf(file_path, _PC_SYMLINK_MAX);
	printf("Number of bytes in a symbolic link %ld\n", option);
    }
    if (c==1) {
	option = pathconf(file_path, _PC_CHOWN_RESTRICTED);
        if (option == 0)
	    printf("chown not allowed\n");
        else
	    printf("chown allowed\n");
    }

    return 0;
}
