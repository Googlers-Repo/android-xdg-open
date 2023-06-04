#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <iostream>

#include <string>
#include <fstream>
#include <getopt.h>
#include <fcntl.h>
#include <pwd.h>
#include <sched.h>

#include <MimeType.hpp>


int main(int argc, char *argv[]) {
    bool help = false;
	int c;
    while ((c = getopt(argc, argv, "h")) != -1) {
		switch (c) {
			case 'h':
			case '?': help = true; break;
		}
	}
  
    if (help || optind == argc) {
        fprintf(stdout,
            "xdg-mime - command line tool for querying information about file type handling\n"
            "and adding descriptions for new file types\n\n"
            "Synopsis\n\n"
            "xdg-mime [...]\n");
		return 1;
	}
  

    char* fileUrl = argv[1];
    const char* type = NULL;

    if (FILE *file = fopen(fileUrl, "r")) {
        fclose(file);
        type = MimeTypes::getType(fileUrl);
    } else { 
        printf("\x1B[31mERR!\033[0m Can't find \"%s\"\n", fileUrl);
        return EXIT_FAILURE;
    }

	if (type != NULL) {
        printf("%s\n", type);
    } else {
        printf("Mime type not found!");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}; 