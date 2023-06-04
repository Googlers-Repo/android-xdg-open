// #include <stdlib.h>
// #include <stdio.h>
// #include <regex> 
// #include <string.h>
// #include <MimeType.h>
// #include <cstdio>
// #include <iostream>
// #include <memory>
// #include <stdexcept>
// #include <string>
// #include <pthread.h>
// #include <stdbool.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>

#include <array>
#include <getopt.h>
#include <regex>
#include <MimeType.hpp>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <fstream>


std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}


//     fprintf(stream,

// xdg-open - opens a file or URL in the user's preferred application

// Synopsis

// xdg-open { file | URL }

// xdg-open { --help | --manual | --version }

// Use 'man xdg-open' or 'xdg-open --manual' for additional info.


//     "MagiskSU\n\n"
//     "Usage: su [options] [-] [user [argument...]]\n\n"
//     "Options:\n"
//     "  -c, --command COMMAND         Pass COMMAND to the invoked shell\n"
//     "  -g, --group GROUP             Specify the primary group\n"
//     "  -G, --supp-group GROUP        Specify a supplementary group.\n"
//     "                                The first specified supplementary group is also used\n"
//     "                                as a primary group if the option -g is not specified.\n"
//     "  -Z, --context CONTEXT         Change SELinux context\n"
//     "  -t, --target PID              PID to take mount namespace from\n"
//     "  -h, --help                    Display this help message and exit\n"
//     "  -, -l, --login                Pretend the shell to be a login shell\n"
//     "  -m, -p,\n"
//     "  --preserve-environment        Preserve the entire environment\n"
//     "  -s, --shell SHELL             Use SHELL instead of the default " DEFAULT_SHELL "\n"
//     "  -v, --version                 Display version number and exit\n"
//     "  -V                            Display version code and exit\n"
//     "  -mm, -M,\n"
//     "  --mount-master                Force run in the global mount namespace\n\n");
//     exit(status);
// }

int main (int argc, char* argv[]) {
  bool help = false;
	int c;
  while ((c = getopt(argc, argv, "h")) != -1) {
		switch (c) {
			case 'h':
			case '?': help = true; break;
		}
	}
  
  if (help || optind == argc) {
		printf("usage: %s [url|file]\n", argv[0]);
		return 1;
	}
  
  const char* url = argv[1];
  const char* type = NULL;
  
  // Open Android apps
  if (std::regex_match(url, std::regex("^[a-z][a-z0-9_]*(\\.[a-z0-9_]+)*[a-z0-9_]*$"))) {
    // Check if app is installed
    char bufferInstalled[128];
    sprintf(bufferInstalled, "pm list packages | grep \"package:%s\" | cut -f 2 -d \":\"", url);
    std::string isInstalled = exec(bufferInstalled);

    if (!isInstalled.empty()) {
      char bufferApk[228];
      sprintf(bufferApk, "am start %s/$(cmd package resolve-activity -c android.intent.category.LAUNCHER %s | sed -n '/name=/s/^.*name=//p')", url, url);
      exec(bufferApk);
    } else {
      printf("\x1B[31mERR!\033[0m \"%s\" is not installed!\n", url);
      return EXIT_FAILURE;
    }

  // Open link in the default browser
  } else if (std::regex_match(url, std::regex("^(https?:\\/\\/)?((([a-z\\d]([a-z\\d-]*[a-z\\d])*)\\.)+[a-z]{2,}|((\\d{1,3}\\.){3}\\d{1,3}))(\\:\\d+)?(\\/[-a-z\\d%_.~+]*)*(\\?[;&a-z\\d%_.~+=-]*)?(\\#[-a-z\\d_]*)?$") )) {
      
      char bufferUrl[128];
      sprintf(bufferUrl, "am start -a android.intent.action.VIEW -d \"%s\"", url);
      exec(bufferUrl);

  // Open files
  } else {
    if (FILE *file = fopen(url, "r")) {
        fclose(file);
        type = MimeTypes::getType(url);
    } else { 
        printf("\x1B[31mERR!\033[0m Can't find \"%s\"\n", url);
        return EXIT_FAILURE;
    }

    char bufferFile[128];
    sprintf(bufferFile, "am start -a android.intent.action.VIEW -d \"%s\" -t \"%s\"", url, type);

	  if (type != NULL) {
      exec(bufferFile); 
    } else {
      printf("Mime type not found!");
      return EXIT_FAILURE;
    }
  }
  
  return EXIT_SUCCESS;
}
