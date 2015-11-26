#include "cst_regex.h"
#include "cst_cart.h"

CST_VAL_REGISTER_TYPE_NODEL(cart,cst_cart)

/* Make this 1 if you want to debug som cart calls */
#define CART_DEBUG 0

#define cst_cart_node_n(P,TREE) ((TREE)->rule_table[P])

void delete_cart(cst_cart *cart)
{
    cst_errmsg("delete_cart function missing\n");
    return;
}

#define cst_cart_node_val(n,tree) (cst_cart_node_n(n,tree).val)
#define cst_cart_node_op(n,tree) (cst_cart_node_n(n,tree).op)
#define cst_cart_node_feat(n,tree) (tree->feat_table[cst_cart_node_n(n,tree).feat])
#define cst_cart_node_yes(n,tree) (n+1)
#define cst_cart_node_no(n,tree) (cst_cart_node_n(n,tree).no_node)

#if CART_DEBUG
static void cart_print_node(int n, const cst_cart *tree)
{
    printf("%s ",cst_cart_node_feat(n,tree));
    if (cst_cart_node_op(n,tree) == CST_CART_OP_IS)
	printf("IS ");
    else if (cst_cart_node_op(n,tree) == CST_CART_OP_LESS)
	printf("< ");
    else if (cst_cart_node_op(n,tree) == CST_CART_OP_GREATER)
	printf("> ");
    else if (cst_cart_node_op(n,tree) == CST_CART_OP_IN)
	printf("IN ");
    else if (cst_cart_node_op(n,tree) == CST_CART_OP_MATCHES)
	printf("MATCHES ");
    else
	printf("*%d* ",cst_cart_node_op(n,tree));
    val_print(stdout,cst_cart_node_val(n,tree));
    printf("\n");
}
#endif

const cst_val *cart_interpret(cst_item *item, const cst_cart *tree)
{
    /* Tree interpretation */
    const cst_val *v=0;
    const cst_val *tree_val;
    const char *tree_feat = "";
    cst_features *fcache;
    int r=0;
    int node=0;

    fcache = new_features_local(item_utt(item)->ctx);

    while (cst_cart_node_op(node,tree) != CST_CART_OP_LEAF)
    {
#if CART_DEBUG
 	cart_print_node(node,tree);
#endif
	tree_feat = cst_cart_node_feat(node,tree);

	v = get_param_val(fcache,tree_feat,0);
	if (v == 0)
	{
	    v = ffeature(item,tree_feat);
	    feat_set(fcache,tree_feat,v);
	}

#if CART_DEBUG
	val_print(stdout,v); printf("\n");
#endif
	tree_val = cst_cart_node_val(node,tree);
	if (cst_cart_node_op(node,tree) == CST_CART_OP_IS)
	    r = val_equal(v,tree_val);
	else if (cst_cart_node_op(node,tree) == CST_CART_OP_LESS)
	    r = val_less(v,tree_val);
	else if (cst_cart_node_op(node,tree) == CST_CART_OP_GREATER)
	    r = val_greater(v,tree_val);
	else if (cst_cart_node_op(node,tree) == CST_CART_OP_IN)
	    r = val_member(v,tree_val);
	else if (cst_cart_node_op(node,tree) == CST_CART_OP_MATCHES)
	    r = cst_regex_match(cst_regex_table[val_int(tree_val)],
				val_string(v));
	else
	{
	    cst_errmsg("cart_interpret_question: unknown op type %d\n",
		       cst_cart_node_op(node,tree));
	    cst_error();
	}

	if (r)
	{   /* Oh yes it is */
#if CART_DEBUG
            printf("   YES\n");
#endif
	    node = cst_cart_node_yes(node,tree);
	}
	else
	{   /* Oh no it isn't */
#if CART_DEBUG
            printf("   NO\n");
#endif
	    node = cst_cart_node_no(node,tree);
	}
    }

    delete_features(fcache);

    return cst_cart_node_val(node,tree);	

}

