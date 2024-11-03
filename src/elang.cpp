#include <iostream>
#include <fstream>
#include <unordered_map>
#include <sstream>
#include <string>
#include <vector>
#include <stack>

std::unordered_map<std::string, int> variables;
std::unordered_map<std::string, std::vector<std::string>> functions;
bool in_function = false;
std::string current_function_name;

void interpret_line(const std::string &line);

void define_function(const std::string &func_name) {
    current_function_name = func_name;
    functions[func_name] = std::vector<std::string>();
    in_function = true;
}

void interpret_script(const std::string &filename);

void interpret_line(const std::string &line) {
    std::string trimmed_line = line;
    trimmed_line.erase(0, trimmed_line.find_first_not_of(" \t"));
    trimmed_line.erase(trimmed_line.find_last_not_of(" \t") + 1);
    if (trimmed_line.empty()) return;

    std::istringstream iss(trimmed_line);
    std::string command;
    iss >> command;

    if (in_function) {
        if (command == "end") {
            in_function = false;
        } else {
            functions[current_function_name].push_back(trimmed_line);
        }
        return;
    }

    if (command == "call") {
        std::string func_name;
        iss >> func_name;
        auto func_it = functions.find(func_name);
        if (func_it != functions.end()) {
            for (const auto &func_line : func_it->second) {
                interpret_line(func_line);
            }
        } else {
            std::cerr << "Error: Undefined function '" << func_name << "'" << std::endl;
        }
    }
    else if (command == "function") {
        std::string func_name;
        iss >> func_name;
        define_function(func_name);
    }
    else if (command == "input") {
        std::string var_name;
        iss >> var_name;
        int value;
        std::cout << "Enter value for " << var_name << ": ";
        std::cin >> value;
        variables[var_name] = value;
    }
    else if (command == "print") {
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
