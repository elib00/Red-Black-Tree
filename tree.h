#include "node.h"
#include <iostream>
#include <cstdlib>
using namespace std;
class BSTree {
    node* root;
    int size;

    node *create_node(int num, node *parent){
        node *n = (node*) calloc(1, sizeof(node));
        n->element = num;
        n->parent = parent;
        n->is_red = true;
        return n;
    }

    void zigleft(node *curr){
        node *par = curr->parent;
        node *gp = par->parent;

        node *currLeft = curr->left;

        par->right = currLeft;
        if(currLeft) currLeft->parent = par;

        curr->left = par;
        par->parent = curr;

        if(gp){
            if(gp->left == par){
                gp->left = curr;
            }else{
                gp->right = curr;
            }

            curr->parent = gp;
        }else{
            root = curr;
            curr->parent = nullptr;
        }
    }

    void zigright(node *curr){
        node *par = curr->parent;
        node *gp = par->parent;

        node *currRight = curr->right;

        par->left = currRight;
        if(currRight) currRight->parent = par;

        curr->right = par;
        par->parent = curr;

        if(gp){
            if(gp->left == par){
                gp->left = curr;
            }else{
                gp->right = curr;
            }

            curr->parent = gp;
        }else{
            root = curr;
            curr->parent = nullptr;
        }
    }

    node *restructure(node *child_x){
        node *parent_y = child_x->parent;
        node *z_gp = parent_y->parent;

        bool gtop_right = false;
        bool ptoc_right = false;

        if(z_gp->right == parent_y){
            gtop_right = true;
        }

        if(parent_y->right == child_x){
            ptoc_right = true;
        }

        //ops
        if(gtop_right && ptoc_right){
             cout << "ZIGLEFT" << endl;
             zigleft(parent_y);
             return parent_y;
        }else if(!gtop_right && ptoc_right){
            cout << "ZIGZAGRIGHT" << endl;
            zigleft(child_x);
            zigright(child_x);
            return child_x;
        }else if(!gtop_right && !ptoc_right){
            cout << "ZIGRIGHT" << endl;
            zigright(parent_y);
            return parent_y;
        }else{
            cout << "ZIGZAGLEFT" << endl;
            zigright(child_x);
            zigleft(child_x);
            return child_x;
        }
    }

    node *insert_node(int num, node *n){
        if(!n) return nullptr;
        if(n->element == num) return nullptr;
        
        if(num > n->element){
            if(!n->right){
               node *new_node = create_node(num, n);
               n->right = new_node;
               return new_node;
            }else{
                return insert_node(num, n->right);
            }
        }else{
            if(!n->left){
                node *new_node = create_node(num, n);
                n->left = new_node;
                return new_node;
            }else{
                return insert_node(num, n->left);
            }
        }
    }

    
    void preserve_properties(node *curr){
        if(!curr) return;
        node *par = curr->parent;

        if(!par) return; // meaning naa tas root

        if(!par->is_red) return;

        node *gp = par->parent;
        node *uncle;

        //pangitaon si uncle
        if(gp->right == par){
            uncle = gp->left;
        }else{
            uncle = gp->right;
        }

        bool black_uncle = false;
        if(uncle == nullptr || uncle->is_red == false){
            black_uncle = true;
        }

        if(black_uncle){
            cout << "INSERTION Violation: Case 1" << endl;
            node *new_parent = restructure(curr);
            new_parent->is_red = false;

            node *right_child = new_parent->right;
            node *left_child = new_parent->left;
            right_child->is_red = true;
            left_child->is_red = true;
        }else{ //red si uncle
            cout << "INSERTION Violation: Case 2" << endl;
            par->is_red = false;
            uncle->is_red = false;

            if(gp != root){
                gp->is_red = true;
            }

            //cout << "kasulod pa" << endl;
            preserve_properties(gp);
        }
    }

    public:
    BSTree() {
        root = NULL;
        size = 0;
    }

    bool insert(int num) {
        node *n = root;
        if(n == nullptr){ // no root;
            node *child = create_node(num, nullptr);
            child->is_red = false;
            root = child;
            size++;
            return true;
        }

        node *child = insert_node(num, n);
        size++;
        preserve_properties(child);
        
        return true;
    }

    // WARNING. Do not modify these methods below.
    // Doing so will nullify your score for this activity.
    void print() {
        if (isEmpty()) {
            cout << "EMPTY" << endl;
            return;
        }
        cout << "PRE-ORDER: ";
        print_preorder(root);
        cout << endl << "IN-ORDER: ";
        print_inorder(root);
        cout << endl << "POST-ORDER: ";
        print_postorder(root);
        cout << endl << "STATUS: " << check_health(root, NULL) << endl;
    }

    bool isEmpty() {
        return size == 0;
    }

    void print_preorder(node* curr) {
        cout << curr->element;
        if (curr->is_red) {
            cout << "(R) ";
        } else {
            cout << "(B) ";
        }
        if (curr->left != NULL) {
            print_preorder(curr->left);
        }
        if (curr->right != NULL) {
            print_preorder(curr->right);
        }
    }

    void print_inorder(node* curr) {
        if (curr->left != NULL) {
            print_inorder(curr->left);
        }
        cout << curr->element;
        if (curr->is_red) {
            cout << "(R) ";
        } else {
            cout << "(B) ";
        }
        if (curr->right != NULL) {
            print_inorder(curr->right);
        }
    }

    void print_postorder(node* curr) {
        if (curr->left != NULL) {
            print_postorder(curr->left);
        }
        if (curr->right != NULL) {
            print_postorder(curr->right);
        }
        cout << curr->element;
        if (curr->is_red) {
            cout << "(R) ";
        } else {
            cout << "(B) ";
        }
    }

    

    // WARNING. Do not modify this method.
    // Doing so will nullify your score for this activity.
    bool check_health(node* curr, node* parent) {
        bool health = curr->parent == parent;
        if (curr->left != NULL) {
            health &= check_health(curr->left, curr);
        }
        if (curr->right != NULL) {
            health &= check_health(curr->right, curr);
        }
        return health;
    }
};