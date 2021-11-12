#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <utility>

std::vector<std::string> split(std::string s, std::string sep) {
    std::vector<std::string> splits;
    size_t pos = 0, delimiter = 0;
    do { 
        delimiter = s.find(sep, pos); 
        splits.push_back(s.substr(pos, delimiter-pos));
        pos = delimiter + sep.length();
    } while(delimiter != std::string::npos);

    return splits;
}

void define_type(std::ofstream& out, std::string basename, std::string classname, std::string fieldList) {
    out << "class " + classname + ": public " + basename + " {\n";
    out << "\tpublic:\n";
    // Constructor
    out << "\t\t" + classname + "(" + fieldList + ") {\n";
    //store parameters in fields
    auto fields = split(fieldList, ", ");
    for (const auto& field: fields) {
        std::string name = split(field, " ")[1];
        out << "\t\t\t" + name + " = " + name + ";\n";
    }
    out << "\t\t}\n";
    // fields
    out << "\tprivate:\n";
    for (const auto& field: fields) {
        out << "\t\t" + field + ";\n";
    }
    out << "};\n\n";
    
}

void defineAST(
        std::string output_dir, 
        std::string basename,
        std::map<std::string,std::string> map
    ) { 

    std::string path = output_dir + "/" + basename + ".cpp";
    std::ofstream out{path};
    if (!out) {
        std::cerr << "Unable to open the file for writing";
        std::exit(65);
    }
    out << "#include \"token.hpp\"\n\n";
    out << "namespace lox { namespace AST { \n\n";
    // define base class
    out << "class " + basename + " {};\n\n";
    for (auto& e: map) {
        std::string classname = e.first;
        std::string fiels = e.second;
        define_type(out, basename, classname, fiels);
    }
    out << "} // AST namespace\n";
    out << "} // lox namespace";

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

    defineAST(output_dir, "Expr", map);
    return 0;
}