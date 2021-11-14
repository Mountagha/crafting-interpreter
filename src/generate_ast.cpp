#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <utility>
#include <algorithm>


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

void defineVisitor(std::ostream& out, std::string basename, const std::map<std::string, std::string>& map) {
    out << "template <typename T>\n";
    out << "class Visitor {\n";
    out << "\tpublic:\n";
    std::string basename_lower = basename;
    std::for_each(basename_lower.begin(), basename_lower.end(), [](char &c ) { c = ::tolower(c); });
    //auto lower_basename = [&basename]() { std::for_each(basename.begin(), basename.end(), ::tolower); };
    for (const auto& e: map){
        std::string type_name = e.first;
        out << "\t\tvirtual T visit" + type_name + basename + "( ";  
        out <<  type_name + "& " + basename_lower + ") = 0;\n";  
    }
    out << "};\n\n";

    
}

void define_type(std::ofstream& out, std::string basename, std::string classname, std::string fieldList) {
    out << "template<typename T>\n";
    out << "class " + classname + ": public " + basename + "<T> {\n";
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
    // Visitor pattern
    out << "\t\tT accept(Visitor<T>& visitor) override {\n";
    out << "\t\t\treturn visitor.visit" + classname + basename + "(*this);\n";
    out << "\t\t}\n";
    // fields
    //out << "\tprivate:\n";
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

    std::string path = output_dir + "/" + basename + ".hpp";
    std::ofstream out{path};
    if (!out) {
        std::cerr << "Unable to open the file for writing";
        std::exit(65);
    }
    out << "#include \"token.hpp\"\n\n";
    out << "namespace lox { namespace AST { \n\n";
    // Define visitor class 
    defineVisitor(out, basename, map);
    // define base class
    out << "template<typename T>\n";
    out << "class " + basename + " {\n\n";
    out << "\tpublic:\n";
    out << "\t\tvirtual T accept(Visitor<T>& visitor) = 0;\n";
    out << "};\n\n";

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
        {"Binary", "Expr left, Token operator_, Expr right"},
        {"Grouping", "Expr expression"},
        {"Literal", "std::string value"},
        {"Unary", "Token operator_, Expr right"} 
    };

    defineAST(output_dir, "Expr", map);
    return 0;
}