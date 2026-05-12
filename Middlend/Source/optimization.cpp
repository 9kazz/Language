#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#include "types.h"
#include "Logfile.h"
#include "utils.h"
#include "tree.h"
#include "verify.h"
#include "dump.h"
#include "optimization.h"

#define CTOR_CONST(number)  Node_Ctor(TYPE_NUM, (NodeData_t) {.num = number}, NULL, NULL)

#define CTOR_FOLDED_NODE(value)      \
        Node_Dtor( LEFT(node)  );    \
        Node_Dtor( RIGHT(node) );    \
        TYPE(node)  = TYPE_NUM;      \
        D_NUM(node) = value;         \
        LEFT(node)  = NULL;          \
        RIGHT(node) = NULL;          \
        *Tree_was_Changed = TRUE;

#define DEL_USELESS_OPER(side)                                                    \
        result_node = (side == SON_L) ? RIGHT(node) : LEFT(node);                 \
        free( ( side == SON_L ) ? LEFT(node) : RIGHT(node) );                     \
        ( ( side == SON_L ) ? RIGHT(node) : LEFT(node) )->parent = PARENT(node);  \
        free(node);                                                               \
        *Tree_was_Changed = TRUE;

#define D_OPER(node)  node->data.oper
#define D_NUM(node)   node->data.num
#define D_VAR(node)   node->data.var

//---------------------------------------------------------------------------------------------------------------------------------

TreeNode_t* Optimization_Func(TreeNode_t* node) {
    assert(node);

    TreeNode_t* result_node = node;
    Bools Tree_was_Changed = FALSE;

    do {
        Tree_was_Changed = FALSE;

        result_node = Const_Folding_OPT    (result_node, &Tree_was_Changed); 
        result_node = Del_Neutral_Elem_OPT (result_node, &Tree_was_Changed); 
        
    } while (Tree_was_Changed);

    return result_node;
}

TreeNode_t* Const_Folding_OPT(TreeNode_t* node, Bools* Tree_was_Changed) {
    assert(node);

    double new_num = 0;

    if ( LEFT(node) )
        Const_Folding_OPT( LEFT(node), Tree_was_Changed);
    
    if ( RIGHT(node) )
        Const_Folding_OPT( RIGHT(node), Tree_was_Changed);

    if ( Is_Leaf_Node(node) )       return node;
    if ( TYPE(node) != TYPE_OPER )  return node;

    if ( Is_unary_oper(D_OPER(node)) && Is_type_num(node, SON_L) )
    {
        new_num = Calculate_Tree(node, NULL, NAN);
        
        CTOR_FOLDED_NODE(new_num)
    }

    else if ( ! Is_unary_oper(D_OPER(node)) && Is_type_num(node, SON_L) 
                                            && Is_type_num(node, SON_R) )
    {
        new_num = Calculate_Tree(node, NULL, NAN);

        CTOR_FOLDED_NODE(new_num)
    }

    return node;
}

TreeNode_t* Del_Neutral_Elem_OPT(TreeNode_t* node, Bools* Tree_was_Changed) {
    assert(node);

    double new_num = 0;
    TreeNode_t* returned_node = node;
    TreeNode_t* result_node   = node;

    if ( Is_Leaf_Node(node) )       return node;
    if ( TYPE(node) != TYPE_OPER )  return node;

    if ( LEFT(node) ) {
        returned_node         = Del_Neutral_Elem_OPT( LEFT(node), Tree_was_Changed);
        LEFT(node)            = returned_node;
        PARENT(returned_node) = node;
    }
    
    if ( RIGHT(node) ) {
        returned_node         = Del_Neutral_Elem_OPT( RIGHT(node), Tree_was_Changed);
        RIGHT(node)           = returned_node;
        PARENT(returned_node) = node;
    }

    switch ( D_OPER(node) )
    {
    case _MATH_ADD_:
        if ( Compare_Son_with_Num(node, SON_L, 0, EQUAL) )  { DEL_USELESS_OPER(SON_L);  return result_node; }
        if ( Compare_Son_with_Num(node, SON_R, 0, EQUAL) )  { DEL_USELESS_OPER(SON_R);  return result_node; }
        break;

    case _MATH_SUB_:
        if ( Compare_Son_with_Num(node, SON_R, 0, EQUAL) )  { DEL_USELESS_OPER(SON_R);  return result_node; }
        break;

    case _MATH_MUL_:
        if ( Compare_Son_with_Num(node, SON_R, 1, EQUAL) )  { DEL_USELESS_OPER(SON_R);  return result_node; }
        if ( Compare_Son_with_Num(node, SON_L, 1, EQUAL) )  { DEL_USELESS_OPER(SON_L);  return result_node; }

        if ( Compare_Son_with_Num(node, SON_R, 0, EQUAL) )  { CTOR_FOLDED_NODE(0);      return result_node; }
        if ( Compare_Son_with_Num(node, SON_L, 0, EQUAL) )  { CTOR_FOLDED_NODE(0);      return result_node; }

        break;

    case _MATH_DIV_:
        if ( Compare_Son_with_Num(node, SON_R, 1, EQUAL) )  { DEL_USELESS_OPER(SON_R);  return result_node; }
        if ( Compare_Son_with_Num(node, SON_L, 0, EQUAL) )  { CTOR_FOLDED_NODE(0);      return result_node; }

        break;

    case _MATH_POW_:
        if ( Compare_Son_with_Num(node, SON_R, 1, EQUAL) )  { DEL_USELESS_OPER(SON_R);  return result_node; }
        if ( Compare_Son_with_Num(node, SON_R, 0, EQUAL) )  { CTOR_FOLDED_NODE(1);      return result_node; }

        if ( Compare_Son_with_Num(node, SON_L, 1, EQUAL) )  { CTOR_FOLDED_NODE(1);      return result_node; }
        if ( Compare_Son_with_Num(node, SON_L, 0, EQUAL) )  { CTOR_FOLDED_NODE(0);      return result_node; }

        break;

    case _MATH_LN_:
        if ( Compare_Son_with_Num(node, SON_L, 1, EQUAL) )  { CTOR_FOLDED_NODE(0);      return result_node; }
        break;

    default:
        break;
    }

    return result_node;
}

double Calculate_Tree(TreeNode_t* node, const char* var, double point) {
    assert(node);

    if ( XOR(var == NULL, isnan(point)) ) {
        fprintf(stderr, "Calculate_tree: NULL pointer to var or point\n");
        return NAN;
    }

    double num_1 = 0;
    double num_2 = 0;

    if ( TYPE(node) == TYPE_NUM) {
        return D_NUM(node);

    } else if ( TYPE(node) == TYPE_OPER) {
        num_1 = Calculate_Tree(LEFT(node), var, point);

        if ( RIGHT(node) )
            num_2 = Calculate_Tree(RIGHT(node), var, point);

        return Calculate_Oper( D_OPER(node), num_1, num_2);
    
    } else {
        fprintf(stderr, "Error in %s at %s:%d: tree cannot be calculated\n");
    }

    fprintf(stderr, "Calculate_tree: unknown type\n");
    return NAN;
}

double Calculate_Oper(Token_t oper, double num_1, double num_2) {

    switch (oper)
    {
    case _MATH_ADD_ :  return num_1 + num_2;
    case _MATH_SUB_ :  return num_1 - num_2;
    case _MATH_MUL_ :  return num_1 * num_2;
    case _MATH_DIV_ :  return num_1 / num_2;
    case _MATH_POW_ :  return pow(num_1, num_2);
    case _MATH_SQRT_:  return sqrt(num_1);
    case _MATH_SIN_ :  return sin(num_1);
    case _MATH_COS_ :  return cos(num_1);
    case _MATH_TAN_ :  return tan(num_1);
    case _MATH_COT_ :  return 1 / tan(num_1);
    case _MATH_ASIN_:  return asin(num_1);
    case _MATH_ACOS_:  return acos(num_1);
    case _MATH_ATAN_:  return atan(num_1);
    case _MATH_ACOT_:  return M_PI / 2 - atan(num_1);
    case _MATH_SINH_:  return sinh(num_1);
    case _MATH_COSH_:  return cosh(num_1);
    case _MATH_TANH_:  return tanh(num_1);
    case _MATH_COTH_:  return -1 / tanh(num_1);
    case _MATH_EXP_ :  return exp(num_1);
    case _MATH_LN_  :  return log(num_1);

    default:
        fprintf(stderr, "Calculate_Oper: unknown operator\n");
        break;
    }

    return NAN;
}

Bools Compare_Son_with_Num(TreeNode_t* node, NodeSon son, double value, Compare relation) {
    assert(node);

    TreeNode_t* son_node = NULL;
    Bools       result   = FALSE;

    if (son == SON_L) 
        son_node = LEFT(node);

    else if (son == SON_R)
        son_node = RIGHT(node);

    else {
        fprintf(stderr, "Is_Value_Eq: incorrect son parametr\n");
        return FALSE;    
    }

    if ( ! son_node)                   
        return FALSE;

    if ( TYPE(son_node) != TYPE_NUM )
        return FALSE;


    if (relation == EQUAL) {
        result = ( Double_compare( D_NUM(son_node), value ) == EQUAL ) ? TRUE : FALSE;

    } else if (relation == MORE) {
        result = ( Double_compare( D_NUM(son_node), value ) == MORE ) ? TRUE : FALSE;
    
    } else {
        result = ( Double_compare( D_NUM(son_node), value ) == LESS ) ? TRUE : FALSE;
    }

    return result;
}

Bools Is_type_num(TreeNode_t* node, NodeSon son) {
    assert(node);

    TreeNode_t* son_node = NULL;

    if (son == SON_L) 
        son_node = LEFT(node);

    else if (son == SON_R)
        son_node = RIGHT(node);

    else {
        fprintf(stderr, "Is_Type_Num: incorrect son parametr\n");
        return FALSE;
    }

    return ( TYPE(son_node) == TYPE_NUM ) ? TRUE : FALSE;
}

Compare Double_compare(double num_1, double num_2) {

    const double EPSILON = 1e-6;
    double delta = num_1 - num_2;

    if (delta > EPSILON)
        return MORE;

    else if (delta < -EPSILON)
        return LESS;

    return EQUAL;
} 

Bools Is_unary_oper(Token_t oper_code) {

    if (_MATH_UNARY_START_ <= oper_code && oper_code <= _MATH_UNARY_END_)
        return TRUE;;

    return FALSE;
}

//---------------------------------------------------------------------------------------------------------------------------------

#undef CTOR_CONST
#undef CTOR_FOLDED_NODE
#undef DEL_USELESS_OPER

#undef D_NUM
#undef D_OPER
#undef D_VAR