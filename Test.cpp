#include <stdexcept>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

#include "OrgChart.hpp"

using namespace ariel;

#include "doctest.h"

TEST_CASE("OrgChart Constructing") {
    OrgChart org;

    // regular construction
    CHECK_NOTHROW(org.add_root("Root"));
    // adding subs
    CHECK_NOTHROW(org.add_sub("Root", "A")); // another sub
    CHECK_NOTHROW(org.add_sub("A", "AA")); // sub-sub
    // use concatenated additions
    CHECK_NOTHROW(org.add_sub("Root", "B").add_sub("B", "BB"));

    // change the root
    CHECK_NOTHROW(org.add_root("Z"));
    // try to add to old root
    CHECK_THROWS(org.add_sub("Root", "FAIL"));
    // add to new root
    CHECK_NOTHROW(org.add_sub("Z", "C"));

    // try to add to a node that does not exist
    CHECK_THROWS(org.add_sub("UNKNOWN", "THROWS"));

    // add to similar but not exist
    CHECK_THROWS(org.add_sub("z", "THROWS"));
}

TEST_CASE("Level order iterator") {
    OrgChart org;
    CHECK_NOTHROW(org.add_root("A"));
    CHECK_NOTHROW(org.add_sub("A", "B").add_sub("A", "C").add_sub("A", "D"));
    CHECK_NOTHROW(org.add_sub("C", "CC").add_sub("CC", "CCC"));
    CHECK_NOTHROW(org.add_sub("B", "BB"));
    /*
      A
      |--------|--------|
      B        C        D
      |        |
      BB       CC
               |
               CCC
   */
    // org chart is not empty so iterator must not be empty
    CHECK_NE(org.begin_level_order(), org.end_level_order());
    size_t i = 0;
    // right traversal order
    vector<string> names_level_order = {"A", "B", "C", "D", "BB", "CC", "CCC"};
    for (auto it = org.begin_level_order(); it != org.end_level_order(); ++it) {
        CHECK_EQ((*it), names_level_order[i++]);
    }

    // check that for-each works like level order iterator
    CHECK_EQ(org.begin_level_order(), org.begin());
    CHECK_EQ(org.end_level_order(), org.end());
    i = 0;
    for (string element : org) {
        CHECK_EQ(element, names_level_order[i++]);
    }
}

TEST_CASE("Reverse order iterator") {
    OrgChart org;
    CHECK_NOTHROW(org.add_root("A"));
    CHECK_NOTHROW(org.add_sub("A", "B").add_sub("A", "C").add_sub("A", "D"));
    CHECK_NOTHROW(org.add_sub("C", "CC").add_sub("CC", "CCC"));
    CHECK_NOTHROW(org.add_sub("B", "BB"));
    /*
      A
      |--------|--------|
      B        C        D
      |        |
      BB       CC
               |
               CCC
   */
    // org chart is not empty so iterator must not be empty
    CHECK_NE(org.begin_reverse_order(), org.reverse_order());
    size_t i = 0;
    // right traversal order
    vector<string> names_reverse_order = {"CCC", "BB", "CC", "B", "C", "D", "A"};
    for (auto it = org.begin_reverse_order(); it != org.reverse_order(); ++it) {
        CHECK_EQ((*it), names_reverse_order[i++]);
    }
}

TEST_CASE("Pre order iterator") {
    OrgChart org;
    CHECK_NOTHROW(org.add_root("A"));
    CHECK_NOTHROW(org.add_sub("A", "B").add_sub("A", "C").add_sub("A", "D"));
    CHECK_NOTHROW(org.add_sub("C", "CC").add_sub("CC", "CCC"));
    CHECK_NOTHROW(org.add_sub("B", "BB"));
    /*
      A
      |--------|--------|
      B        C        D
      |        |
      BB       CC
               |
               CCC
   */
    // org chart is not empty so iterator must not be empty
    CHECK_NE(org.begin_preorder(), org.end_preorder());
    size_t i = 0;
    // right traversal order
    vector<string> names_reverse_order = {"A", "B", "BB", "C", "CC", "CCC", "D"};
    for (auto it = org.begin_reverse_order(); it != org.reverse_order(); ++it) {
        CHECK_EQ((*it), names_reverse_order[i++]);
    }
}

TEST_CASE("Data changes affects iterator") {
    OrgChart org;

    CHECK_NOTHROW(org.add_root("A"));
    CHECK_NOTHROW(org.add_sub("A", "B").add_sub("A", "C").add_sub("A", "D"));
    CHECK_NOTHROW(org.add_sub("C", "CC").add_sub("CC", "CCC"));
    CHECK_NOTHROW(org.add_sub("B", "BB"));
    /*
      A
      |--------|--------|
      B        C        D
      |        |
      BB       CC
               |
               CCC
   */
    // before changing anything
    size_t i = 0;
    vector<string> names_level_order = {"A", "B", "C", "D", "BB", "CC", "CCC"};
    for (string element : org) {
        CHECK_EQ(element, names_level_order[i++]);
    }
    // change root before starting iterator
    string str = "NewRoot";
    names_level_order[0] = str;
    CHECK_NOTHROW(org.add_root(str));
    i = 0;
    for (string element : org) {
        CHECK_EQ(element, names_level_order[i++]);
    }
    // change root after iterator started
    auto it = org.begin();
    str = "Testing";
    CHECK_NOTHROW(org.add_root(str));
    names_level_order[0] = str;
    i = 0;
    while(it != org.end()) {
        CHECK_EQ(*it, names_level_order[i++]); // value should be the new root because the iterator should hold pointers
        ++it;
    }
}

string &convertStringToLower(string &s) {
    transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); });
    return s;
}

TEST_CASE("Using algorithms") {
    OrgChart org;
    CHECK_NOTHROW(org.add_root("A"));
    CHECK_NOTHROW(org.add_sub("A", "B").add_sub("A", "C").add_sub("A", "D"));
    CHECK_NOTHROW(org.add_sub("C", "CC").add_sub("CC", "CCC"));
    CHECK_NOTHROW(org.add_sub("B", "BB"));

    transform(
            org.begin(), org.end(),
            org.begin(),
            [](string &s) { return convertStringToLower(s); });

    // values should be lower
    size_t i = 0;
    vector<string> names_level_order = {"a", "b", "c", "d", "bb", "cc", "ccc"};
    for (string element : org) {
        CHECK_EQ(element, names_level_order[i++]);
    }
}