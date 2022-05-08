#include "interpreter.hpp"
#include "return.hpp"

namespace lox {

Interpreter::Interpreter() {
    globals = std::make_shared<Environment>();
    environment = globals;
    //std::unique_ptr<LoxCallable> clock {static_cast<LoxCallable*>(new TimeFunction())}; 
    LoxCallable* clock {static_cast<LoxCallable*>(new TimeFunction())}; 
    globals->define("clock", LoxObject(clock, this)); // possible leak here
    // need to investigate whyy smart pointers failing. thought is wasn't released as long as interpreter running. 
}

Interpreter::~Interpreter() {
    // Manually delete objects since functions assume their 
    // interpreter is valid when they are destroyed.
    
    // Note: this is a hack to avoid double-destruction
    m_destroying = true;
    m_instances.clear();
    m_classes.clear();
    m_callables.clear();
}

void Interpreter::addUser(LoxCallable* func) {
    ++m_callables[func].second;
}

void Interpreter::addUser(LoxClass* klass) {
    ++m_classes[klass].second;
}

void Interpreter::addUser(LoxInstance* inst) {
    ++m_instances[inst].second;
}

void Interpreter::removeUser(LoxCallable* func) {
    if (m_destroying) return;
    --m_callables[func].second;
    if(!m_callables[func].second){
        m_callables.clear();
    }
}

void Interpreter::removeUser(LoxClass* klass) {
    if (m_destroying) return;
    --m_classes[klass].second;
    if (!m_classes[klass].second) {
        m_classes.clear();
    }
}

void Interpreter::removeUser(LoxInstance* inst) {
    if (m_destroying) return;
    --m_instances[inst].second;
    if (!m_instances[inst].second) {
        m_instances.clear();
    }
}

LoxCallable* Interpreter::createFunction(Function& stmt, PEnvironment env) {
    std::unique_ptr<LoxCallable> func { (static_cast<LoxCallable*>(new LoxFunction(&stmt, env))) };
}

void Interpreter::resolve(Expr* expr, unsigned int depth) {
    locals.insert({expr, depth});
}

LoxObject Interpreter::visitLiteralExpr(Literal& expr) {
    return expr.value;
}

LoxObject Interpreter::visitLogicalExpr(Logical& expr) {
    LoxObject left = evaluate(expr.left);

    if (expr.operator_.token_type == OR) {
        if(left) return left;
    } else {
        if (!left) return left;
    }

    return evaluate(expr.right);
}

LoxObject Interpreter::visitSetExpr(Set& expr) {
    LoxObject object = evaluate(expr.object);
    LoxObject value = evaluate(expr.value);
    return object.set(expr.name, value);
}

LoxObject Interpreter::visitSuperExpr(Super& expr) {
    auto distance = locals[&expr];
    auto superclass = environment->getAt(distance, "super");
    auto object = environment->getAt(distance - 1, "this");
    return superclass.getLoxClass()->function(expr.method, object.getInstance());
}

LoxObject Interpreter::visitThisExpr(This& expr) {
    return lookUpVariable(expr.keyword, &expr);
}

LoxObject Interpreter::visitGroupingExpr(Grouping& expr) {
    return evaluate(expr.expression);
}

LoxObject Interpreter::visitUnaryExpr(Unary& expr) {
    LoxObject right = evaluate(expr.right);

    switch(expr.operator_.token_type) {
        case TokenType::BANG:
            return !right;
        case TokenType::MINUS:
            return -right;
        default:
            std::runtime_error("Invalid unary expression.");
    }

}

LoxObject Interpreter::visitBinaryExpr(Binary& expr) {
    LoxObject left = evaluate(expr.left);
    LoxObject right = evaluate(expr.right);

    switch(expr.operator_.token_type) {
        case TokenType::GREATER:
            return LoxObject(left > right);
        case TokenType::GREATER_EQUAL:
            return LoxObject(left >= right); 
        case TokenType::LESS:
            return LoxObject(left < right); 
        case TokenType::LESS_EQUAL:
            return LoxObject(left <= right); 
        case TokenType::MINUS:
            return left - right;
        case TokenType::PLUS:
            return left + right;
        case TokenType::SLASH:
            return left / right; 
        case TokenType::STAR:
            return left * right; 
        case TokenType::BANG_EQUAL: return LoxObject(left != right);
        case TokenType::EQUAL_EQUAL: return LoxObject(left == right);
        default:
            std::runtime_error("unknown binary expression");
    }

}

LoxObject Interpreter::visitCallExpr(Call& expr) {
    LoxObject callee = evaluate(expr.callee);

    std::vector<LoxObject> arguments {};
    for (auto& argument : expr.arguments){
        arguments.push_back(evaluate(argument));
    }

    return callee(*this, arguments);
}

LoxObject Interpreter::visitGetExpr(Get& expr) {
    LoxObject object = evaluate(expr.object);
    return object.get(expr.name);
}

LoxObject Interpreter::visitVariableExpr(Variable& expr) {
    return lookUpVariable(expr.name, &expr); 
}

LoxObject Interpreter::visitAssignExpr(Assign& expr) {
    LoxObject value = evaluate(expr.value);
    if (locals.find(&expr) != locals.end()) {
        unsigned int distance = locals[&expr];
        environment->assignAt(distance, expr.name, value);
    } else {
        globals->assign(expr.name, value);
    }
    return value;
}

// Stmt
void Interpreter::visitExpressionStmt(Expression& stmt) {
    evaluate(stmt.expression);
}

void Interpreter::visitFunctionStmt(Function& stmt) {
    LoxCallable* function {static_cast<LoxCallable*>(new LoxFunction(&stmt, environment))}; // possible leak (never freed)
    environment->define(stmt.name.lexeme, LoxObject(function, this));
}

void Interpreter::visitIfStmt(If& stmt) {
    if(evaluate(stmt.condition)) {
        execute(stmt.thenBranch);
    } else if (stmt.elseBranch) {
        execute(stmt.elseBranch);
    }
}

void Interpreter::visitPrintStmt(Print& stmt) {
    LoxObject value = evaluate(stmt.expression);
    std::cout << value << '\n';
}

void Interpreter::visitReturnStmt(Return& stmt) {
    LoxObject value;
    if (stmt.value) {
        value = evaluate(stmt.value);
    }

    throw ReturnExcept(value); // ugly but author gives reasonable excuses.
}

void Interpreter::visitVarStmt(Var& stmt) {
    LoxObject value;
    if (stmt.initializer) {
        value = evaluate(stmt.initializer);
    }
    environment->define(stmt.name.lexeme, value); 
}

void Interpreter::visitWhileStmt(While& stmt) {
    while (evaluate(stmt.condition)) {
        execute(stmt.body); 
    }
    
}

void Interpreter::executeBlock(std::vector<std::unique_ptr<Stmt>>& statements, PEnvironment newEnv) {
    ScopeEnvironment newScope(environment, newEnv);
    for (auto& statement: statements) {
        execute(statement);
    }
}

void Interpreter::visitBlockStmt(Block& stmt) {
    auto newEnv = std::make_shared<Environment>(environment); 
    executeBlock(stmt.statements, newEnv);
}

void Interpreter::visitClassStmt(Class& stmt) {
    
    LoxObject superclass;
    if (stmt.superclass) {
        superclass = evaluate(stmt.superclass);
        if (superclass.getLoxObjectType() != LoxType::Class) {
            throw std::runtime_error("Superclass must be a class.");
        }
    }
    environment->define(stmt.name.lexeme, LoxObject());

    if(stmt.superclass) {
        environment = std::make_shared<Environment>(environment);
        environment->define("super", superclass);
        // will allow methods closure to capture environment containing super.  
    }
    LoxClass* classy = new LoxClass(&stmt, superclass.getLoxClass(), this, environment); // possible leak here. not freed.

    if (superclass.getLoxObjectType() != LoxType::Nil) {
        environment = environment->enclosing;
    }

    environment->assign(stmt.name, LoxObject(classy, this));
}

void Interpreter::interpret(std::vector<std::unique_ptr<Stmt>>& statements) {
    try
    {
        for (auto& stmt : statements) {
            execute(stmt);
        }
    }
    catch(const std::runtime_error& e)
    {
        std::cerr << e.what() << '\n';
        Lox::runtimeError();
    }
}

}