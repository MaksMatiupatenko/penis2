#ifndef __SHADERHPP__
#define __SHADERHPP__

#include <string>
#include <fstream>
#include "BASE.h"
#include "GLLOADER.hpp"
#include "MATH.h"
#include "TEXTURE.hpp"
#include <map>
#include <stack>

struct __GL_SHADERS_ORDER_HANDLER {
private:
    std::stack<UINT> ACTIVE_SHADERS_STACK;

public:
    __GL_SHADERS_ORDER_HANDLER() {
        ACTIVE_SHADERS_STACK.push(0);
    }

    void setActive(int id) {
        ACTIVE_SHADERS_STACK.push(id);
        glUseProgram(id);
    }

    const int& curActive() const {
        return ACTIVE_SHADERS_STACK.top();
    }

    void setBack(int id, BOOL unsafe) {
        auto cur = curActive();
        if (!unsafe && cur != id) {
            return;
        }
        ACTIVE_SHADERS_STACK.pop();
        glUseProgram(curActive());
    }

    void clear() {
        while (!ACTIVE_SHADERS_STACK.empty()) {
            ACTIVE_SHADERS_STACK.pop();
        }
        ACTIVE_SHADERS_STACK.push(0);
    }

    ~__GL_SHADERS_ORDER_HANDLER() {
        glUseProgram(0);
    }
} GL_SHADERS_ORDER_HANDLER;

class SPRG {
private:
    bool loadFromFile(const std::string& path, GLenum type, UINT& out) {
        std::ifstream inp(path);
        std::string s, code;
        while (std::getline(inp, s)) {
            code += s;
            code += '\n';
        }
        inp.close();

        //debug << code << std::endl;

        const char* cdata = code.c_str();
        out = glCreateShader(type);
        glShaderSource(out, 1, &cdata, 0);
        glCompileShader(out);

        int success;
        glGetShaderiv(out, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(out, 512, 0, infoLog);
            debug << "|ERROR| Failed to compile shader in " + path + '\n';
            debug << infoLog << std::endl;
            return false;
        }

        return true;
    }

    UINT id;

    std::map <std::string, int> texnum;

public:
    ~SPRG() {
        glDeleteProgram(id);
    }

    bool loadFromFile(
        std::string vertex, 
        std::string tessControl, 
        std::string tessEvaluation, 
        std::string geometry, 
        std::string fragment) {

        UINT vert = 0, tessC = 0, tessE = 0, geom = 0, frag = 0;
        if (vertex != "") loadFromFile(vertex, GL_VERTEX_SHADER, vert);
        if (tessControl != "") loadFromFile(tessControl, GL_TESS_CONTROL_SHADER, tessC);
        if (tessEvaluation != "") loadFromFile(tessEvaluation, GL_TESS_EVALUATION_SHADER, tessE);
        if (geometry != "") loadFromFile(geometry, GL_GEOMETRY_SHADER, geom);
        if (fragment != "") loadFromFile(fragment, GL_FRAGMENT_SHADER, frag);

        UINT program = glCreateProgram();
        if (vertex != "") {
            glAttachShader(program, vert);
            glDeleteShader(vert);
        }
        if (tessControl != "") {
            glAttachShader(program, tessC);
            glDeleteShader(tessC);
        }
        if (tessEvaluation != "") {
            glAttachShader(program, tessE);
            glDeleteShader(tessE);
        }
        if (geometry != "") {
            glAttachShader(program, geom);
            glDeleteShader(geom);
        }
        if (fragment != "") {
            glAttachShader(program, frag);
            glDeleteShader(frag);
        }

        glLinkProgram(program);

        int success;
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetProgramInfoLog(program, 512, 0, infoLog);
            debug << "|ERROR| Failed to link shader program\n";
            debug << infoLog << std::endl;
            return false;
        }

        id = program;
        return true;
    }

    SPRG() = default;
    SPRG(
        std::string vertex, 
        std::string tessControl, 
        std::string tessEvaluation, 
        std::string geometry, 
        std::string fragment) {

        loadFromFile(vertex, tessControl, tessEvaluation, geometry, fragment);
    }

    void setActiveShader() {
        GL_SHADERS_ORDER_HANDLER.setActive(id);
    }

    void setUniform(std::string name, FLOAT value) {
        setActiveShader();
        UINT loc = glGetUniformLocation(id, name.c_str());
        glUniform1f(loc, value);
        setInactive();
    }
    void setUniform(std::string name, CONST GLTXTR& tex) {
        setActiveShader();
        if (!texnum.count(name)) {
            texnum[name] = texnum.size();
            UINT loc = glGetUniformLocation(id, name.c_str());
            glUniform1i(loc, texnum[name]);
        }
        
        glActiveTexture(GL_TEXTURE0 + texnum[name]);
        tex.use();
        setInactive();
    }
    void setUniform(std::string name, mat3f mat) {
        setActiveShader();
        UINT loc = glGetUniformLocation(id, name.c_str());
        float mt[9] = { mat[0][0], mat[1][0], mat[2][0], mat[0][1], mat[1][1], mat[2][1], mat[0][2], mat[1][2], mat[2][2] };
        glUniformMatrix3fv(loc, 1, GL_FALSE, mt);
        setInactive();
    }

    /// <summary>
    /// я тут ебнул эррор хендлинг типа. если верхушка стека это не мы, 
    /// то ниче не происходит так что можно по поводу сейвовости этой хуеты не париться.
    /// но если хочется для дебага сделать ее ансейвовой, то туда передаешь true
    /// </summary>
    void setInactive(BOOL unsafeVersion = FALSE) {
        GL_SHADERS_ORDER_HANDLER.setBack(id, unsafeVersion);
    }
};

#endif