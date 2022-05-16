#pragma once

#include <iostream>
#include <string>
#include <queue>
#include <stack>
#include <stdexcept>
#include <vector>

using namespace std;
namespace ariel {
    class OrgChart {
    private:

        class node {
        public:
            string data;
            vector<node *> subs;

            node(const string &data) : data(data) {}
        };

        node *root;

    public:
        OrgChart() : root(nullptr) {}

        ~OrgChart() {}

        OrgChart &add_root(const string &data) { return *this; }

        OrgChart &add_sub(const string &parent, const string &son) { return *this; }

        friend ostream &operator<<(ostream &ost, const OrgChart &chart) { return ost; }

        class iterator {
        protected:
            node *current_node;
            queue<node *> traversal_order;
        public:
            iterator() {}

            string &operator*() const { return current_node->data; }

            string *operator->() const { return &current_node->data; }

            bool operator==(const iterator &rhs) const { return true; }

            bool operator!=(const iterator &rhs) const { return false; }

            iterator &operator++() { return *this; }
        };

        class level_order_iterator : public iterator {
        public:
            level_order_iterator(node *ptr) {}
        };

        level_order_iterator begin_level_order() { return level_order_iterator(nullptr); }

        level_order_iterator end_level_order() { return level_order_iterator(nullptr); }

        class reverse_order_iterator : public iterator {
        public:
            reverse_order_iterator(node *ptr) {}
        };

        reverse_order_iterator begin_reverse_order() { return reverse_order_iterator(nullptr); }

        reverse_order_iterator reverse_order() { return reverse_order_iterator(nullptr); }

        class preorder_iterator : public iterator {
        public:
            preorder_iterator(node *ptr) {}
        };

        preorder_iterator begin_preorder() { return preorder_iterator(nullptr); }

        preorder_iterator end_preorder() { return preorder_iterator(nullptr); }

        level_order_iterator begin() { return level_order_iterator(nullptr); }

        level_order_iterator end() { return level_order_iterator(nullptr); }
    };

    ostream &operator<<(ostream &ost, const OrgChart &chart);
}

