#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

#include "OrgChart.hpp"

using namespace ariel;

string &convertStringToLower(string &s) {
    transform(s.begin(), s.end(), s.begin(),
              [](unsigned char c) { return std::tolower(c); });
    return s;
}

void printLevelOrder(OrgChart &org) {
    for_each(org.begin(),
             org.end(),
             [](string &s) { cout << s << " "; });
    cout << endl;
}

void printHeadline(const string &s) {
    cout << endl;
    cout << "************************************" << endl;
    cout << s << endl;
    cout << "************************************" << endl;
    cout << endl;
}

int main() {
    // 0. Constructing an organization and iterating with all iterators
    printHeadline("Constructing an organization and iterating with all iterators");
    OrgChart org;
    org.add_root("CEO")
            .add_sub("CEO", "CTO")
            .add_sub("CEO", "CFO")
            .add_sub("CEO", "COO")
            .add_sub("CTO", "VP_SW")
            .add_sub("COO", "VP_BI");

    /* Prints the org chart in a reasonable format. For example:
    CEO
    |--------|--------|
    CTO      CFO      COO
    |                 |
    VP_SW             VP_BI
    */
    cout << "Print level order : " << endl;
    printLevelOrder(org); // prints: CEO CTO CFO COO VP_SW VP_BI

    cout << "Print pre-order : " << endl;
    for (auto it = org.begin_preorder(); it != org.end_preorder(); ++it) {
        cout << (*it) << " ";
    } // prints: CEO CTO VP_SW CFO COO VP_BI
    cout << endl;

    cout << "Print reverse order : " << endl;
    for (auto it = org.begin_reverse_order(); it != org.reverse_order(); ++it) {
        cout << (*it) << " ";
    } // prints: VP_SW VP_BI CTO CFO COO CEO
    cout << endl;

    // 1. For Each - print the length of each string
    printHeadline("Example 1 - print the length of each string using for_each");
    cout << "Print the length of each string using for_each: " << endl;
    for_each(org.begin(),
             org.end(),
             [](const string &s) { cout << s.size() << " "; });
    cout << endl;

    // 2. Transform - print level order as lower strings
    printHeadline("Example 2 - print level order as lower strings using transform");
    cout << "Print level order as lower strings using transform : " << endl;
    transform(
            org.begin(), org.end(),
            org.begin(),
            [](string &s) { return convertStringToLower(s); });
    // print
    for (auto it = org.begin_level_order(); it != org.end_level_order(); ++it) {
        cout << (*it) << " ";
    } // prints: ceo cto cfo coo vp_sw vp_bi
    cout << endl;

    // 3. Replace - replace CTO with full name
    printHeadline("Example 3 - replace CTO with full name using replace");
    string current_job_title = "cto";
    string new_job_title = "Chief Technology Officer";
    cout << "Replace '" << current_job_title << "' with '" << new_job_title << "' using replace : " << endl;

    cout << "Before:" << endl;
    cout << org << endl;
    replace(org.begin(), org.end(), current_job_title, new_job_title);
    cout << "After:" << endl;
    cout << org << endl;

    // 4. Iterators don't collide, we can traverse with 2 iterators on same OrgChart,
    // iterator does not change the object's content
    printHeadline("Example 4 - See that the iterators don't collide when traversing simultaneously");
    auto it_preorder = org.begin_preorder();
    auto it_level_order = org.begin_level_order();
    int loc = 1;
    while (it_preorder != org.end_preorder() && it_level_order != org.end_level_order()) {
        cout << "Item in loc " << loc++ << ": " << endl;
        cout << "Preorder: " << *it_preorder << endl;
        cout << "Level Order: " << *it_level_order << endl;
        ++it_preorder;
        ++it_level_order;
    }

    // 5. Copying moving the OrgChart
    printHeadline("Example 5 - Copying and Moving");
    OrgChart copy(org);
    cout << "After copying 'org' to 'copy'" << endl;
    cout << "org:\n" << org << endl;
    cout << "copy:\n" << copy << endl;
    cout << "Add new_sub to copy" << endl;
    copy.add_sub("ceo", "new sub");
    cout << "After adding new_sub to copy" << endl;
    cout << "org:\n" << org << endl;
    cout << "copy:\n" << copy << endl;
    cout << "After moving 'org' to 'move'" << endl;
    OrgChart move = std::move(org);
    cout << "org:\n" << org << endl;
    cout << "copy:\n" << copy << endl;
    cout << "move:\n" << move << endl;

    return 0;
}