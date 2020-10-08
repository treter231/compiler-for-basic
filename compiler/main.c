#include "lexer.h"
#include "parser.h"
#include "hashtable.h"
#include "semantics.h"
#include "CodeGen.h"

int main(int argc, char const *argv[]) {

    ListTokens* Tokens = IdentifyTokens(argc, argv[1]);
    ListPrint(Tokens);
    
    struct AST* root = initParser(Tokens);
    createTree(root);
    
    init_semantic(root);
    print_table(root);
    
    code_gen(root);

    // struct listnode *node;

    // hashtab_init(hashtab);
    // hashtab_add(hashtab, "Tigr", 190);
    // hashtab_add(hashtab, "Slon", 2300);
    // hashtab_add(hashtab, "Volk", 60);

    // node = hashtab_lookup(hashtab, "Slon");
    // printf("Node: %s, %d\n", node->key, node->value);

    // hashtab_delete(hashtab, "Slon");
    // node = hashtab_lookup(hashtab, "Slon");
    // if (node != NULL) {
    //     printf("Node: %s, %d\n", node->key, node->value);
    // } else {
    //     printf("Key 'Slon' not found\n");
    // }
    return 0;
}
