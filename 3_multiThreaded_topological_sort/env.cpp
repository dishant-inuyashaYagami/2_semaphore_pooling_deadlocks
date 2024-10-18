int
get_int_env(const char* var_name, int def_val)
{
    const char* var_val = getenv(var_name);
    if (!var_val) {
        return def_val;
    }
    int val = atoi(var_val);
    return val;
}
