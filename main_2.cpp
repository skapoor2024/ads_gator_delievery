#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "gatordel.h" // Make sure this includes the declaration of the GatorDel class and its methods

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    std::ifstream input_file(argv[1]);
    if (!input_file) {
        std::cerr << "Error: Unable to open " << argv[1] << " for reading." << std::endl;
        return 1;
    }

    GatorDel gator_del;

    std::string input_file_name = argv[1];
    size_t dot_position = input_file_name.find_last_of('.');
    std::string output_file_name = input_file_name.substr(0, dot_position) + "_output.txt";

    std::ofstream output_file(output_file_name);
    if (!output_file) {
        std::cerr << "Error: Unable to open " << output_file_name << " for writing." << std::endl;
        return 1;
    }

    auto coutbuf = std::cout.rdbuf(output_file.rdbuf());

    std::string line;
    while (std::getline(input_file, line)) {
        // Remove spaces to simplify parsing
        line.erase(remove_if(line.begin(), line.end(), isspace), line.end());

        size_t open_paren = line.find('(');
        size_t close_paren = line.find(')');
        if (open_paren == std::string::npos || close_paren == std::string::npos || close_paren <= open_paren) {
            std::cerr << "Error: Malformed command - " << line << std::endl;
            continue;
        }

        std::string function_name = line.substr(0, open_paren);
        std::string args_str = line.substr(open_paren + 1, close_paren - open_paren - 1);

        std::vector<std::string> args;
        std::istringstream args_stream(args_str);
        std::string arg;
        while (std::getline(args_stream, arg, ',')) {
            args.push_back(arg);
        }

        if (function_name == "createOrder" && args.size() == 4) {
            gator_del.create_order(std::stoi(args[0]), std::stoi(args[1]), std::stoi(args[2]), std::stoi(args[3]));
        } else if (function_name == "cancelOrder" && args.size() == 2) {
            gator_del.cancel_order(std::stoi(args[0]), std::stoi(args[1]));
        } else if (function_name == "updateTime" && args.size() == 3) {
            gator_del.update_time(std::stoi(args[0]), std::stoi(args[1]), std::stoi(args[2]));
        } else if (function_name == "print") {
            if (args.size() == 1) {
                gator_del.print(std::stoi(args[0]));
            } else if (args.size() == 2) {
                gator_del.print(std::stoi(args[0]), std::stoi(args[1]));
            } else {
                std::cerr << "Error: Invalid arguments for print - " << line << std::endl;
            }
        } else if (function_name == "getRankOfOrder" && args.size() == 1) {
            gator_del.get_rank_of_order(std::stoi(args[0]));
        } else if (function_name == "Quit" && args.empty()) {
            gator_del.quit();
            break;
        } else {
            std::cerr << "Error: Unknown function name encountered or incorrect arguments - " << line << std::endl;
        }
    }

    std::cout.rdbuf(coutbuf);

    return 0;
}
