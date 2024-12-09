#ifndef XSH_ENV_H
#define XSH_ENV_H

void set_env_var(const char *key, const char *value);
void unset_env_var(const char *key);
char *get_env_var(const char *key);
char *replace_env_vars(const char *input);

#endif
