#ifndef ENV_H_
#define ENV_H_

#include "pphat_runtime.h"
#include "str.h"

typedef struct Env Env;
typedef struct Env {
   Env* parent;
   PpHatRunTime* vars; 
} Env;




void env_declare_variable(Env* env,char* name,RunTimeValue value) {
    pphat_insert_runtime(&env->vars,name,value);
}

void env_assing_variable(Env* env,char* name,RunTimeValue value) {
    if(env == NULL) {
        assert(0 && "variable dont exist `env_get_variable`");
    }
    if(!pphat_contains_runtime(env->vars, name)) {
        env_assing_variable(env->parent, name,value);
    }
    pphat_insert_runtime(&env->vars,name,value);
}

RunTimeValue env_get_variable(Env* env,char* name) {
    if(env == NULL) {
        assert(0 && "variable dont exist `env_get_variable`");
        return (RunTimeValue) {.as = 0, .type = Noth};
    }
    if(!pphat_contains_runtime(env->vars, name)) {
        return env_get_variable(env->parent, name);
    }
    return pphat_get_runtime(env->vars,name);
}


#endif // ENV_H_