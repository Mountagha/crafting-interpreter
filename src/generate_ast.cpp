#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <utility>

auto split(std::string s, std::string sep) {
    auto delimiter = s.find(sep);
    if (delimiter != std::string::npos)
        return std::make_pair(s.substr(0, delimiter), s.substr(delimiter));
    return std::make_pair(s, "");
}
void define_type(std::ofstream& out, std::string basename, std::string classname, std::string fields) {

} 
void defineAST(
        std::string output_dir, 
        std::string basename,
        std::map<std::string,std::string> map) { 
    std::string path = output_dir + "/" + basename + ".cpp";
    std::ofstream out{path};
    if (!out) {
        std::cerr << "Unable to open the file for writing";
        std::exit(65);
    }
    out << "#include <iostream>\n\n";
    out << "namespace lox { namespace AST { \n";
    out << "class Express {\n";
    out << "\tprivate:\n";
    out << "\t\tint n;\n";
    out << "};\n";

    for (auto& e: map) {
        std::string classname = e.first;
        std::string fiels = e.second;
        define_type(out, basename, classname, fiels);
    }
}
int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: generate_ast <output_dir>";
        std::exit(64);
    }
    std::string output_dir = argv[1];
    std::map<std::string, std::string> map {
        {"Binary", "Expr left, Token operator, Expr right"},
        {"Grouping", "Expr expression"},
        {"Literal", "Object value"},
        {"Unary", "Token operator, Expr right"} 
    };

    defineAST("AST", "Expr", map);
    //auto my_map = {"binary": }
    return 0;
}