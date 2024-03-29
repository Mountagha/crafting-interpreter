#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <utility>
#include <algorithm>

void banner(std::ostream& out) {
    out << "/*  \n\t Be aware that this is a auto-generated source file hence \
                \n\t no modification should be done directly. \
                \n\t All changes MUST BE added via the src/AST/generate_ast.cpp file.\n*/\n";
}

bool isPointer(const std::string& s) {
    return s.size() && s.back() == '*';
}

std::string type_from_ptr(const std::string& s){
    return s.substr(0, s.size()-1); // return string without *
}

bool is_vector_type(const std::string& s) {
    auto sub = s.find(std::string("std::vector<"));
    if (sub != std::string::npos)
        return true;
    return false;
}

void declare_classes(std::ostream& out, std::map<std::string, std::string> map) {
    for (const auto& e: map){
        out << "class " + e.first + ";\n";
    }
    out << "\n";
} 

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

void defineVisitor(std::ostream& out, 
                std::string basename, 
                const std::map<std::string, 
                std::string>& map,
                std::string return_type) {

    out << "class " << basename << "Visitor {\n";
    out << "\tpublic:\n";
    std::string basename_lower = basename;
    std::for_each(basename_lower.begin(), basename_lower.end(), [](char &c ) { c = ::tolower(c); });
    //auto lower_basename = [&basename]() { std::for_each(basename.begin(), basename.end(), ::tolower); };
    for (const auto& e: map){
        std::string type_name = e.first;
        out << "\t\tvirtual " << return_type << " visit" + type_name + basename + "( ";  
        out <<  type_name + "& " + basename_lower + ") = 0;\n";  
    }
    out << "};\n\n";

}

void define_type(std::ofstream& out, 
                std::string basename, 
                std::string classname, 
                std::vector<std::string> fieldList,
                std::string return_type) {

    out << "class " + classname + ": public " + basename + " {\n";
    out << "\tpublic:\n";
    // Constructor
    out << "\t\t" + classname + "(";
    std::string line;
    for (const std::string& field: fieldList) {
        // separate name from type
        std::vector<std::string> name_type = split(field, " ");
        if (isPointer(name_type[0]))
            line += "std::unique_ptr<" + type_from_ptr(name_type[0]) + ">&& " + name_type[1] + "_, ";
        else if (is_vector_type(name_type[0]))
            line += name_type[0] + "&& " + name_type[1] + "_, ";
        else 
            line += field + "_, "; 
    }
    // remove last comma
    line = line.substr(0, line.size()-2);
    out << line + ") {\n"; 
     
    //store parameters in fields
    
    for (const auto& field: fieldList) {
        std::vector<std::string> name_type = split(field, " ");
        if (isPointer(name_type[0]) || is_vector_type(name_type[0]))
            out << "\t\t\t" + name_type[1] + " = std::move (" + name_type[1] + "_);\n";
        else
            out << "\t\t\t" + name_type[1] + " = " + name_type[1] + "_;\n";

    }
    
    
    out << "\t\t}\n";
    // Visitor pattern
    out << "\t\t" << return_type << " accept(" << basename << "Visitor& visitor) override {\n";
    out << "\t\t\t" << (return_type == "void" ? "" : "return ") << "visitor.visit" + classname + basename + "(*this);\n";
    out << "\t\t}\n";
    // fields
    //out << "\tprivate:\n";
    for (const auto& field: fieldList) {
        std::vector<std::string> name_type = split(field, " ");
            if (isPointer(name_type[0]))
                out << "\t\tstd::unique_ptr<" + type_from_ptr(name_type[0]) + "> " + name_type[1] + ";\n";
            else 
                out << "\t\t" + field + ";\n";
    }
    out << "};\n\n";
    
}

void defineAST(
        std::string output_dir, 
        std::string basename,
        std::map<std::string,std::string> map,
        std::vector<std::string>& includes,
        std::string return_type
    ) { 

    std::string path = output_dir + "/" + basename + ".hpp";
    std::ofstream out{path};
    if (!out) {
        std::cerr << "Unable to open the file for writing";
        std::exit(65);
    }

    banner(out);

    out << "#pragma once\n\n";
    for (const auto& include_ : includes) 
        out << "#include " << include_ << '\n'; 
    out << "\nnamespace lox { \n\n";
    // declare classes
    declare_classes(out, map);
    // Define visitor class 
    defineVisitor(out, basename, map, return_type);
    // define base class
    out << "class " + basename + " {\n\n";
    out << "\tpublic:\n";
    out << "\t\t" + basename + "() = default;\n";
    out << "\t\t~" + basename + "() = default;\n";
    out << "\t\tvirtual " << return_type << " accept(" << basename << "Visitor& visitor) = 0;\n";
    out << "};\n\n";

    for (auto& e: map) {
        std::string classname = e.first;
        std::vector<std::string> fields = split(e.second, ", ");
        define_type(out, basename, classname, fields, return_type);
    }
    
    //out << "} // AST namespace\n";
    out << "} // lox namespace";

}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: generate_ast <output_dir>";
        std::exit(64);
    }
    std::string output_dir = argv[1];
    std::map<std::string, std::string> expr_map {
        {"Binary", "Expr* left, Token operator_, Expr* right"},
        {"Call", "Expr* callee, Token paren, std::vector<std::unique_ptr<Expr>> arguments"},
        {"CommaExpr", "std::vector<std::unique_ptr<Expr>> expressions"},
        {"Get", "Expr* object, Token name"},
        {"Assign", "Token name, Expr* value"},
        {"Grouping", "Expr* expression"},
        {"Literal", "LoxObject value"},
        {"Logical", "Expr* left, Token operator_, Expr* right"},
        {"Set", "Expr* object, Token name, Expr* value"},
        {"Super", "Token keyword, Token method"},
        {"This", "Token keyword"},
        {"Ternary", "Expr* condition, Expr* thenBranch, Expr* elseBranch"},
        {"Unary", "Token operator_, Expr* right"},
        {"Variable", "Token name"}
    };

    std::vector<std::string> includes {"\"token.hpp\"", "\"loxObject.hpp\"", "<memory>", "<vector>"};
    defineAST(output_dir, "Expr", expr_map, includes, "LoxObject");

    std::map<std::string, std::string> stmt_map {
        {"Block", "std::vector<std::unique_ptr<Stmt>> statements"},
        {"Class", "Token name, Expr* superclass, std::vector<std::unique_ptr<Function>> methods"},
        {"Expression", "Expr* expression"},
        {"Function", "Token name, std::string kind, std::vector<Token> params, std::vector<std::unique_ptr<Stmt>> body"},
        {"If", "Expr* condition, Stmt* thenBranch, Stmt* elseBranch"},
        {"Print", "Expr* expression"},
        {"Return", "Token keyword, Expr* value"},
        {"Var", "Token name, Expr* initializer"},
        {"While", "Expr* condition, Stmt* body"}
    };

    includes.push_back("\"Expr.hpp\"");

    defineAST (output_dir, "Stmt", stmt_map, includes, "void");
    return 0;
}