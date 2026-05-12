#ifndef VERIFY_H
#define VERIFY_H

enum error_codes {
    EC_MIN_CRITICAL_ERROR = 1 << 10, // All errors more than this constant are critical
    EC_NULL_POINTER       = 1 << 10,
    EC_PARENT_ERR         = 2 << 10
};

#define PRINT_ERROR(code, text)                                                                 \
                                                                                                \
    fprintf(Logfile, "<h3 style = \"color: red;\"> Node_Verify: " text " </h3>\n");             \
    fprintf(Logfile, "<h3 style = \"color: red;\"> Error code: %d </h3>\n\n", code);            \
                                                                                                \
    *Error_code = *Error_code | code;

#define IF_CRITICAL_ERR(error_code)                     \
    TreeErr_t temp_##error_code = error_code;           \
    if (temp_##error_code >= EC_MIN_CRITICAL_ERROR)     \
        return temp_##error_code;

#ifdef DEBUG
    #define ONDEBUG(code) code
#else
    #define ONDEBUG(code)
#endif

TreeErr_t Node_Verify (TreeNode_t* node);
TreeErr_t if_null_pointer(void* pointer, TreeErr_t* Error_code);
TreeErr_t check_ancestor(TreeNode_t* node, TreeErr_t* Error_code);

#endif