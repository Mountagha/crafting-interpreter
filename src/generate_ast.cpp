#include <iostream>
#include <map>
#include <vector>
#include <fstream>


void defineAST(
        std::string output_dir, 
        std::string basename
        /* std::map<std::string, std::vector<std::pair<std::string, std::string>>> */) {
    std::string path = output_dir + "/" + basename + ".cpp";
    std::ofstream out{path};
    if (!out) {
        std::cerr << "Unable to open the file for writing";
        std::exit(65);
    }
    out << "#include <iostream>\n\n";
    out << "class Express {\n";
    out << "\tprivate:\n";
    out << "\t\tint n;\n";
    out << "};\n";
}
int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: generate_ast <output_dir>";
        std::exit(64);
    }
    std::string output_dir = argv[1];
    defineAST("AST", "Expr");
    //auto my_map = {"binary": }
    return 0;
}