#include <stdlib.h>
#include <stdio.h>
#include <regex> 
#include <string.h>
#include <MimeType.h>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <getopt.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
  const char* type;
  
  if (std::regex_match (url, std::regex("^(https?:\\/\\/)?((([a-z\\d]([a-z\\d-]*[a-z\\d])*)\\.)+[a-z]{2,}|((\\d{1,3}\\.){3}\\d{1,3}))(\\:\\d+)?(\\/[-a-z\\d%_.~+]*)*(\\?[;&a-z\\d%_.~+=-]*)?(\\#[-a-z\\d_]*)?$") )) {
    char bufferFile[128];
    sprintf(bufferFile, "am start -a android.intent.action.VIEW -d \"%s\"", url);
    exec(bufferFile);
  } else {
    type = MimeTypes::getType(url);
    char bufferUrl[128];
    sprintf(bufferUrl, "am start -a android.intent.action.VIEW -d \"%s\" -t \"%s\"", url, type);
    exec(bufferUrl); 
  }
  
  return EXIT_SUCCESS;
}

