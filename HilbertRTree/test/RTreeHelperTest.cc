#include "gtest.h"
#include "../src/RTreeHelper.hh"
#include "../src/Constants.hh"

TEST(RTreeHelperTest, chooseLeaf)
{
    std::vector<boost::uint64_t> lower1(2, 1);
    std::vector<boost::uint64_t> upper1(2, 1);
    boost::shared_ptr<Rectangle> rect1(new Rectangle(lower1, upper1));
    boost::shared_ptr<HilbertValue> h1(new HilbertValue(rect1->getCenter()));

    std::vector<boost::uint64_t> lower2(2, 2);
    std::vector<boost::uint64_t> upper2(2, 2);
    boost::shared_ptr<Rectangle> rect2(new Rectangle(lower2, upper2));
    boost::shared_ptr<HilbertValue> h2(new HilbertValue(rect2->getCenter()));

    std::vector<boost::uint64_t> lower3(2, 3);
    std::vector<boost::uint64_t> upper3(2, 3);
    boost::shared_ptr<Rectangle> rect3(new Rectangle(lower3, upper3));
    boost::shared_ptr<HilbertValue> h3(new HilbertValue(rect3->getCenter()));

    std::vector<boost::uint64_t> lower4(2, 4);
    std::vector<boost::uint64_t> upper4(2, 4);
    boost::shared_ptr<Rectangle> rect4(new Rectangle(lower4, upper4));
    boost::shared_ptr<HilbertValue> h4(new HilbertValue(rect4->getCenter()));

    boost::shared_ptr<LeafEntry>  leaf1( new LeafEntry(rect1,h2));
    boost::shared_ptr<LeafEntry>  leaf2( new LeafEntry(rect2,h3));
    boost::shared_ptr<LeafEntry>  leaf3( new LeafEntry(rect3,h4));

    Node* leaf = new Node(MAX_NODE_ENTRIES);
    leaf->setLeaf(true);

    ASSERT_EQ(leaf, RTreeHelper::chooseLeaf(leaf, h1));

    Node* childNode1 = new Node(MAX_NODE_ENTRIES);
    childNode1->setLeaf(true);
    childNode1->insertLeafEntry(leaf1);
    childNode1->adjustLHV();
    childNode1->adjustMBR();
    boost::shared_ptr<NonLeafEntry>  entry1( new NonLeafEntry(childNode1));

    Node* childNode2 = new Node(MAX_NODE_ENTRIES);
    childNode2->setLeaf(true);
    childNode2->insertLeafEntry(leaf2);
    childNode2->adjustLHV();
    childNode2->adjustMBR();
    boost::shared_ptr<NonLeafEntry>  entry2(new NonLeafEntry(childNode2));

    Node* childNode3 = new Node(MAX_NODE_ENTRIES);
    childNode3->setLeaf(true);
    childNode3->insertLeafEntry(leaf3);
    childNode3->adjustLHV();
    childNode3->adjustMBR();
    boost::shared_ptr<NonLeafEntry>  entry3( new NonLeafEntry(childNode3));

    Node* nonLeaf = new Node(MAX_NODE_ENTRIES);

    nonLeaf->insertNonLeafEntry(entry3);
    nonLeaf->insertNonLeafEntry(entry2);
    nonLeaf->insertNonLeafEntry(entry1);

    // If this looks confusing, look at the h* in new NodeEntry
    ASSERT_EQ(childNode2,RTreeHelper::chooseLeaf(nonLeaf, h2));

    ASSERT_EQ(childNode3,RTreeHelper::chooseLeaf(nonLeaf, h3));

    ASSERT_EQ(childNode1,RTreeHelper::chooseLeaf(nonLeaf, h1));

    delete leaf;
    delete nonLeaf;
}

TEST(RTreeHelperTest, searchLeaf)
{
    std::vector<boost::uint64_t> lower1(2, 1);
    std::vector<boost::uint64_t> upper1(2, 1);
    boost::shared_ptr<Rectangle> rect1(new Rectangle(lower1, upper1));
    boost::shared_ptr<HilbertValue> h1(new HilbertValue(rect1->getCenter()));

    std::vector<boost::uint64_t> lower2(2, 2);
    std::vector<boost::uint64_t> upper2(2, 2);
    boost::shared_ptr<Rectangle> rect2(new Rectangle(lower2, upper2));
    boost::shared_ptr<HilbertValue> h2(new HilbertValue(rect2->getCenter()));

    std::vector<boost::uint64_t> lower3(2, 3);
    std::vector<boost::uint64_t> upper3(2, 3);
    boost::shared_ptr<Rectangle> rect3(new Rectangle(lower3, upper3));

    Node* leaf = new Node(MAX_NODE_ENTRIES);
    leaf->setLeaf(true);

    boost::shared_ptr<LeafEntry>  entry1( new LeafEntry(rect1, h1));
    boost::shared_ptr<LeafEntry>  entry2( new LeafEntry(rect2, h2));

    std::list<boost::shared_ptr<NodeEntry> > r1 = RTreeHelper::search(leaf, rect1);
    ASSERT_EQ(0, r1.size());

    leaf->insertLeafEntry(entry1);

    std::list<boost::shared_ptr<NodeEntry> > r2 = RTreeHelper::search(leaf, rect1);
    ASSERT_EQ(1, r2.size());
    ASSERT_EQ(entry1, r2.front());

    leaf->insertLeafEntry(entry2);

    //No intersection
    std::list<boost::shared_ptr<NodeEntry> > r3 = RTreeHelper::search(leaf, rect3);
    ASSERT_EQ(0, r3.size());

    std::vector<boost::uint64_t> lower4(2, 0);
    std::vector<boost::uint64_t> upper4(2, 4);
    boost::shared_ptr<Rectangle> rect4(new Rectangle(lower4, upper4));

    std::list<boost::shared_ptr<NodeEntry> > r4 = RTreeHelper::search(leaf, rect4);
    ASSERT_EQ(2, r4.size());

    delete leaf;
}

TEST(RTreeHelperTest, searchNonLeaf)
{
    std::vector<boost::uint64_t> lower1(2, 1);
    std::vector<boost::uint64_t> upper1(2, 1);
    boost::shared_ptr<Rectangle> rect1(new Rectangle(lower1, upper1));
    boost::shared_ptr<HilbertValue> h1(new HilbertValue(rect1->getCenter()));

    std::vector<boost::uint64_t> lower2(2, 2);
    std::vector<boost::uint64_t> upper2(2, 2);
    boost::shared_ptr<Rectangle> rect2(new Rectangle(lower2, upper2));
    boost::shared_ptr<HilbertValue> h2(new HilbertValue(rect2->getCenter()));

    std::vector<boost::uint64_t> lower3(2, 3);
    std::vector<boost::uint64_t> upper3(2, 3);
    boost::shared_ptr<Rectangle> rect3(new Rectangle(lower3, upper3));


    std::vector<boost::uint64_t> lower4(2, 0);
    std::vector<boost::uint64_t> upper4(2, 2);
    boost::shared_ptr<Rectangle> rect4(new Rectangle(lower4, upper4));

    Node* leaf = new Node(MAX_NODE_ENTRIES);
    leaf->setLeaf(true);

    boost::shared_ptr<LeafEntry>  entry1( new LeafEntry(rect1, h1));
    boost::shared_ptr<LeafEntry>  entry2( new LeafEntry(rect2, h2));

    leaf->insertLeafEntry(entry1);
    leaf->insertLeafEntry(entry2);

    leaf->adjustLHV();
    leaf->adjustMBR();

    Node* parent = new Node(MAX_NODE_ENTRIES);
    boost::shared_ptr<NonLeafEntry>  nonLeafEntry( new NonLeafEntry(leaf));
    parent->insertNonLeafEntry(nonLeafEntry);

    std::list<boost::shared_ptr<NodeEntry> > r1 = RTreeHelper::search(parent, rect3);
    ASSERT_EQ(0, r1.size());

    std::list<boost::shared_ptr<NodeEntry> > r2 = RTreeHelper::search(parent, rect1);
    ASSERT_EQ(1, r2.size());
    ASSERT_EQ(entry1, r2.front());

    std::list<boost::shared_ptr<NodeEntry> > r3 = RTreeHelper::search(parent, rect2);
    ASSERT_EQ(1, r3.size());
    ASSERT_EQ(entry2, r3.front());

    std::list<boost::shared_ptr<NodeEntry> > r4 = RTreeHelper::search(parent, rect4);
    ASSERT_EQ(2, r4.size());
    ASSERT_EQ(entry1, r4.front());
    ASSERT_EQ(entry2, r4.back());

    //delete leaf;
    delete parent;
}

TEST(RTreeHelperTest, redistributeEntries_1)
{
    EntryMultiSet entries;
    std::list<Node *> nodes;
    Node* node1 = new Node(MAX_NODE_ENTRIES);
    node1->setLeaf(true);
    nodes.push_back(node1);

    Node* node2 = new Node(MAX_NODE_ENTRIES);
    node2->setLeaf(true);
    nodes.push_back(node2);

    for(int i=0; i<MAX_NODE_ENTRIES*2-1; i++)
    {
        std::vector<boost::uint64_t> l(2, 1);
        std::vector<boost::uint64_t> u(2, 1);
        boost::shared_ptr<Rectangle> rectangle(new Rectangle(l, u));
        boost::shared_ptr<HilbertValue> hv(new HilbertValue(rectangle->getCenter()));

        boost::shared_ptr<NodeEntry>  entry( new LeafEntry(rectangle, hv));

        entries.insert(entry);
    }

    RTreeHelper::redistributeEntries(entries, nodes);

    ASSERT_EQ(MAX_NODE_ENTRIES, node1->getEntries().size());
    ASSERT_EQ(MAX_NODE_ENTRIES-1, node2->getEntries().size());

    delete node1;
    delete node2;
}


TEST(RTreeHelperTest, redistributeEntries_2)
{
    EntryMultiSet entries;
    std::list<Node *> nodes;
    Node* node1 = new Node(MAX_NODE_ENTRIES);
    nodes.push_back(node1);

    Node* node2 = new Node(MAX_NODE_ENTRIES);
    nodes.push_back(node2);

    for(int i=0; i<2*MAX_NODE_ENTRIES; i++)
    {
        std::vector<boost::uint64_t> l(2, i);
        std::vector<boost::uint64_t> u(2, i);
        boost::shared_ptr<Rectangle> rectangle(new Rectangle(l, u));
        boost::shared_ptr<HilbertValue> hv(new HilbertValue(rectangle->getCenter()));
        boost::shared_ptr<LeafEntry>  leafEntry( new LeafEntry(rectangle, hv));
        Node* leaf = new Node(MAX_NODE_ENTRIES);
        leaf->setLeaf(true);
        leaf->insertLeafEntry(leafEntry);
        leaf->adjustLHV();
        leaf->adjustMBR();

        boost::shared_ptr<NodeEntry>  entry( new NonLeafEntry(leaf));

        entries.insert(entry);
    }

    RTreeHelper::redistributeEntries(entries, nodes);

    ASSERT_EQ(MAX_NODE_ENTRIES, node1->getEntries().size());
    ASSERT_EQ(MAX_NODE_ENTRIES, node2->getEntries().size());

    delete node1;
    delete node2;
}

TEST(RTreeHelperTest, handleOverflow_1)
{
    //One node, no siblings
    Node* node1 = new Node(MAX_NODE_ENTRIES);
    node1->setLeaf(true);
    std::list<Node*> siblings;

    for(int i=0; i<MAX_NODE_ENTRIES; i++)
    {
        std::vector<boost::uint64_t> l(2, i);
        std::vector<boost::uint64_t> u(2, i);
        boost::shared_ptr<Rectangle> rectangle(new Rectangle(l, u));
        boost::shared_ptr<HilbertValue> hv(new HilbertValue(rectangle->getCenter()));

        boost::shared_ptr<LeafEntry>  entry( new LeafEntry(rectangle, hv));
        node1->insertLeafEntry(entry);
    }
    //The node is full

    std::vector<boost::uint64_t> l(2, 0);
    std::vector<boost::uint64_t> u(2, 0);
    boost::shared_ptr<Rectangle> rectangle(new Rectangle(l, u));
    boost::shared_ptr<HilbertValue> hv(new HilbertValue(rectangle->getCenter()));

    boost::shared_ptr<LeafEntry>  entry( new LeafEntry(rectangle, hv));

    Node* node2 = RTreeHelper::handleOverflow(node1, entry, siblings);

    ASSERT_EQ(MAX_NODE_ENTRIES/2,node1->getEntries().size());
    ASSERT_EQ(MAX_NODE_ENTRIES/2+1,node2->getEntries().size());

    ASSERT_EQ(node1,node2->getNextSibling());
    ASSERT_EQ(NULL,node2->getPrevSibling());

    ASSERT_EQ(NULL,node1->getNextSibling());
    ASSERT_EQ(node2,node1->getPrevSibling());

    delete node1;
    delete node2;
}

TEST(RTreeHelperTest, DISABLED_handleOverflow_2)
{
    std::list<Node*> siblings;

    //One node, one sibling
    Node* node1 = new Node(MAX_NODE_ENTRIES);
    node1->setLeaf(true);

    Node* node2 = new Node(MAX_NODE_ENTRIES);
    node2->setLeaf(true);

    node1->setNextSibling(node2);
    node2->setPrevSibling(node1);

    for(int i=0; i<MAX_NODE_ENTRIES; i++)
    {
        std::vector<boost::uint64_t> l(2, i);
        std::vector<boost::uint64_t> u(2, i);
        boost::shared_ptr<Rectangle> rectangle(new Rectangle(l, u));
        boost::shared_ptr<HilbertValue> hv(new HilbertValue(rectangle->getCenter()));

        boost::shared_ptr<LeafEntry>  entry( new LeafEntry(rectangle, hv));
        node1->insertLeafEntry(entry);
    }
    //The node is full

    std::vector<boost::uint64_t> l(2, 0);
    std::vector<boost::uint64_t> u(2, 0);
    boost::shared_ptr<Rectangle> rectangle(new Rectangle(l, u));
    boost::shared_ptr<HilbertValue> hv(new HilbertValue(rectangle->getCenter()));

    boost::shared_ptr<LeafEntry>  entry( new LeafEntry(rectangle , hv));

    Node* node3 = RTreeHelper::handleOverflow(node1, entry,siblings);

    ASSERT_EQ(NULL, node3);

    ASSERT_EQ(MAX_NODE_ENTRIES/2+1,node1->getEntries().size());
    ASSERT_EQ(MAX_NODE_ENTRIES/2,node2->getEntries().size());

    delete node1;
    delete node2;
}
