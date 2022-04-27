#pragma once

#include <stack>
#include <map>
#include "Expr.hpp"
#include "Stmt.hpp"
#include "interpreter.hpp"

namespace lox {

class Resolver : public ExprVisitor, public StmtVisitor {

    public:
        using SExpr = std::unique_ptr<Stmt>;
        using PExpr = std::unique_ptr<Expr>;
        Resolver(Interpreter* intp): interpreter{intp} {}

        void resolve(std::vector<SExpr>& statements) {
            for (auto& statement : statements ) {
                resolve(statement);
            }
        }

        void visitBlockStmt(Block& stmt) override {
            beginScope();
            resolve(stmt.statements);
            endScope();
        }

        void visitClassStmt(Class& stmt) {
            ClassType enclosingClass = currentClass;
            currentClass = ClassType::CLASS;

            declare(stmt.name);
            define(stmt.name);

            // Resolve possible superclass
            if (stmt.superclass) {
                
                auto superclassName = static_cast<Variable*>(stmt.superclass.get())->name;
                if (stmt.superclass && (stmt.name.lexeme == superclassName.lexeme)) {
                    Lox::error(superclassName, "A class can't inherit from itself.");
                }

                resolve(stmt.superclass);
            }

            beginScope();
            scopes.back()["this"] = true;

            for (auto& method: stmt.methods) {
                FunctionType declaration = FunctionType::METHOD;
                if (method->name.lexeme == "init") {
                    declaration = FunctionType::INITIALIZER;
                }
                resolveFunction(*method, declaration); // not sure if this is a good practice.
            }

            endScope();
            currentClass = enclosingClass;

        }

        void visitVarStmt(Var& stmt) override {
            declare(stmt.name);
            if (stmt.initializer) {
                resolve(stmt.initializer);
            }
            define(stmt.name);
        }

        LoxObject visitVariableExpr(Variable& expr) override {
            if (!scopes.empty() &&
                scopes.back().find(expr.name.lexeme) != scopes.back().end() && 
                scopes.back().at(expr.name.lexeme) == false) {
                    Lox::error(expr.name, 
                        "Can't read local variable in its own initializer");
            }

            resolveLocal(expr, expr.name);
            return LoxObject();
        }

        LoxObject visitAssignExpr(Assign& expr) override {
            resolve(expr.value);
            resolveLocal(expr, expr.name);
            return LoxObject();
        }

        void visitFunctionStmt(Function& stmt) override {
            declare(stmt.name);
            define(stmt.name);

            resolveFunction(stmt, FunctionType::FUNCTION);
        }

        void visitExpressionStmt(Expression& stmt) override {
            resolve(stmt.expression);
        }

        void visitIfStmt(If& stmt) override {
            resolve(stmt.condition);
            resolve(stmt.thenBranch);
            if (stmt.elseBranch) resolve(stmt.elseBranch);
        }

        void visitPrintStmt(Print& stmt) override {
            resolve(stmt.expression);
        }

        void visitReturnStmt(Return& stmt) override {
            if (currentFunction == FunctionType::NONE) {
                Lox::error(stmt.keyword, "Cant return from top-level code.");
            }
            if (stmt.value) {
                if (currentFunction == FunctionType::INITIALIZER) {
                    Lox::error(stmt.keyword, "Can't return a value from an initlializer.");
                }
                resolve(stmt.value);
            } 
        }

        void visitWhileStmt(While& stmt) override {
            resolve(stmt.condition);
            resolve(stmt.body);
        }

        LoxObject visitBinaryExpr(Binary& expr) override {
            resolve(expr.left);
            resolve(expr.right);
            return LoxObject();
        }

        LoxObject visitCallExpr(Call& expr) override {
            resolve(expr.callee);
            for (auto& arg : expr.arguments) {
                resolve(arg);
            }
            return LoxObject();
        }

        LoxObject visitGetExpr(Get& expr) override {
            resolve(expr.object);
            return LoxObject();
        }

        LoxObject visitGroupingExpr(Grouping& expr) override {
            resolve(expr.expression);
            return LoxObject();
        }

        LoxObject visitLiteralExpr(Literal& expr) override {
            return LoxObject();
        }

        LoxObject visitLogicalExpr(Logical& expr) override {
            resolve(expr.left);
            resolve(expr.right);
            return LoxObject();
        }

        LoxObject visitSetExpr(Set& expr) override {
            resolve(expr.value);
            resolve(expr.object);
            return LoxObject();
        }

        LoxObject visitThisExpr(This& expr) override {
            if (currentClass == ClassType::NONE) {
                Lox::error(expr.keyword, "Can't use 'this' outside of a class.");
            }
            resolveLocal(expr, expr.keyword);
            return LoxObject();
        }

        LoxObject visitUnaryExpr(Unary& expr) override {
            resolve(expr.right);
            return LoxObject();
        }

    private:
        enum class ClassType {
            NONE,
            CLASS
        };

        enum class FunctionType {
            NONE,
            FUNCTION,
            INITIALIZER,
            METHOD
        };
        
        ClassType currentClass {ClassType::NONE};
        FunctionType currentFunction {FunctionType::NONE};
        Interpreter* interpreter;
        std::vector<std::map<std::string, bool>> scopes {};

        void resolve(SExpr& stmt) {
            stmt->accept(*this);
        }

        LoxObject resolve(PExpr& expr){
            return expr->accept(*this);
        }

        void beginScope() {
            scopes.push_back({});
        }

        void endScope() {
            scopes.pop_back();
        }

        void declare(Token name) {
            if (scopes.empty()) return;

            if (scopes.back().find(name.lexeme) != scopes.back().end()) {
                Lox::error(name, "Already a variable with this name in this scope.");
            }
            scopes.back()[name.lexeme] = false;
        }

        void define(Token name) {
            if (scopes.empty()) return;
            scopes.back()[name.lexeme] = true;
        }

        void resolveLocal(Expr& expr, Token name) {
            unsigned int scope_depth = 0; 
            for (auto r_iter = scopes.rbegin(); r_iter != scopes.rend(); r_iter++) {
                if (r_iter->find(name.lexeme) != r_iter->end()) {
                    interpreter->resolve(&expr, scope_depth);
                    return;
                }  
                scope_depth++;
            }
        }

        void resolveFunction(Function& function, FunctionType type) {
            FunctionType enclosingFunction = currentFunction;
            currentFunction = type;
            beginScope();
            for(auto param : function.params) {
                declare(param);
                define(param);
            }
            resolve(function.body);
            endScope();
            currentFunction = enclosingFunction;
        }

};

} // lox namespace