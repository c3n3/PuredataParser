 once
#include <vector>
#include <string>
#include <regex>	
#include <iostream>
 class PuredataFile
{
	public:
			std::vector<std::string> objects;
				std::vector<std::string> connects;
					
					int inputObject;
						int outputObject;
							void toString();
								PuredataFile(const char * name);
};


PuredataFile::PuredataFile(const char* name)
{
		FILE* f;
			fopen_s(&f, name, "r"); // read mode
				std::cout << "File: " << f << " name: " << name << "\n";
					if (f == NULL) {
								throw __ExceptionPtrAssign;
									}
						char c;
							char buf[100];
								outputObject = 0;
									inputObject = 0;
										std::regex obj("#X obj");
											std::regex connect("#X connect");
												std::regex out("1.0001");
													std::regex in("1.0002");

														for (int i = 0; (c = fgetc(f)) != EOF; i++) {
																	std::cout << "char c: " << c << " index: " << i << "\n";
																			buf[i] = c;
																					if (c == ';') {
																									i = 0;
																												buf[i + 1] = 0;
																															if (std::regex_search(buf, obj)) {
																																				objects.push_back(buf);
																																							}
																																		else if (std::regex_search(buf, connect)) {
																																							connects.push_back(buf);
																																										}
																																					if (std::regex_search(buf, in)) {
																																										inputObject = objects.size() - 1;
																																													}
																																								else if (std::regex_search(buf, out)) {
																																													outputObject = objects.size() - 1;
																																																}
																																										}
																						}
}

void PuredataFile::toString() {
		for (int i = 0; i < objects.size(); i++) {
					std::cout << objects[i] << (i == outputObject ? " <--- OUTPUT" : "") << (i == outputObject ? " <--- INPUT" : "");
						}
}
