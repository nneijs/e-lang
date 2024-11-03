#include <iostream>
#include <fstream>
#include <unordered_map>
#include <sstream>
#include <string>

std::unordered_map<std::string, int> variables;

void interpret_line(const std::string &line) {
    std::istringstream iss(line);
    std::string command;
    iss >> command;

    if (command == "print") {
        std::string value;
        iss >> std::ws;
        char ch = iss.peek();

        if (ch == '"') {
            iss.get();
            std::getline(iss, value, '"');
            std::cout << value << std::endl;
        } else {
            iss >> value;
            auto it = variables.find(value);
            if (it != variables.end()) {
                std::cout << it->second << std::endl;
            } else {
                std::cerr << "Error: Undefined variable '" << value << "'" << std::endl;
            }
        }
    }
    else if (command == "set") {
        std::string var_name;
        int value;
        iss >> var_name >> value;
        variables[var_name] = value;
    }
    else if (command == "add") {
        std::string target_var, op1, op2;
        iss >> target_var >> op1 >> op2;
        
        int operand1 = variables.count(op1) ? variables[op1] : std::stoi(op1);
        int operand2 = variables.count(op2) ? variables[op2] : std::stoi(op2);
        
        variables[target_var] = operand1 + operand2;
    }
    else {
        std::cerr << "Error: Unknown command '" << command << "'" << std::endl;
    }
}

void interpret_script(const std::string &filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error: Could not open file '" << filename << "'" << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        interpret_line(line);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: interpreter <filename>.e" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    if (filename.size() < 2 || filename.substr(filename.size() - 2) != ".e") {
        std::cerr << "Error: File extension should be .e" << std::endl;
        return 1;
    }

    interpret_script(filename);
    return 0;
}
