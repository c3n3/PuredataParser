nclude <iostream>
#include "PuredataFile.h"
#include <vector>

int main(int argc, char* argv[])
{

	    std::vector<PuredataFile> files;
	        files.reserve(argc - 1);
		    for (int i = 1; i < argc; i++) {
			            files.push_back(PuredataFile(argv[i]));
				        }
		        std::cout << files[0].objects[0] << "\n";


			    for (int i = 0; i < files.size(); i++) {
				            files[i].toString();
					        }
			        return 0;
}
