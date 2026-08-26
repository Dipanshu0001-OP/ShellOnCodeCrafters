#include <iostream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <unistd.h>

int main() {
    // Flush after every std::cout / std::cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    while (true) {
        std::cout << "$ ";

        std::string input;
        std::getline(std::cin, input);

        if (input == "exit") {
            break;
        }

        else if (input.substr(0, 5) == "echo ") {
            std::cout << input.substr(5) << std::endl;
        }

        else if (input.substr(0, 5) == "type ") {
            std::string command = input.substr(5);

            // Step 1: Check builtins
            if (command == "exit" ||
                command == "echo" ||
                command == "type") {

                std::cout << command
                          << " is a shell builtin"
                          << std::endl;
            }

            // Step 2: Search PATH
            else {
                const char* pathEnv = std::getenv("PATH");
                bool found = false;

                if (pathEnv != nullptr) {
                    std::stringstream pathStream(pathEnv);
                    std::string directory;

                    while (std::getline(pathStream, directory, ':')) {
                        std::string fullPath = directory + "/" + command;

                        // Check whether it exists AND is executable
                        if (access(fullPath.c_str(), X_OK) == 0) {
                            std::cout << command
                                      << " is "
                                      << fullPath
                                      << std::endl;

                            found = true;
                            break;
                        }
                    }
                }

                // Step 3: Nothing found
                if (!found) {
                    std::cout << command
                        << ": not found"
                        << std::endl;
                }
            }
        }

        else {
            std::cout << input
                      << ": command not found"
                      << std::endl;
        }
    }
}
