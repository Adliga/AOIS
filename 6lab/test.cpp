#include "pch.h"
#include "gtest/gtest.h"
#include "D:/Study/VisualStudio/AOIS_6/AOIS_6/AOIS_6.cpp"
#include <string>

//Тесты для AVLTree
TEST(AVLTreeTest, Init) {
    AVLTree tree;
    EXPECT_THROW(tree.search("test"), std::runtime_error);
}

TEST(AVLTreeTest, InsertAndSearch) {
    AVLTree tree;
    tree.insert("key1", "value1");
    EXPECT_EQ(tree.search("key1"), "value1");
    EXPECT_THROW(tree.search("key2"), std::runtime_error);
}

TEST(AVLTreeTest, Update) {
    AVLTree tree;
    tree.insert("key1", "value1");
    tree.insert("key1", "value2");
    EXPECT_EQ(tree.search("key1"), "value2");
}

TEST(AVLTreeTest, Delete) {
    AVLTree tree;
    tree.insert("key1", "value1");
    tree.remove("key1");
    EXPECT_THROW(tree.search("key1"), std::runtime_error);
}

TEST(AVLTreeTest, DeleteNonExistent) {
    AVLTree tree;
    EXPECT_THROW(tree.remove("key1"), std::runtime_error);
}

TEST(AVLTreeTest, LeftLeftBalance) {
    AVLTree tree;
    tree.insert("key3", "value3");
    tree.insert("key2", "value2");
    tree.insert("key1", "value1");
    EXPECT_EQ(tree.search("key1"), "value1");
    EXPECT_EQ(tree.search("key2"), "value2");
    EXPECT_EQ(tree.search("key3"), "value3");
}

TEST(AVLTreeTest, RightRightBalance) {
    AVLTree tree;
    tree.insert("key1", "value1");
    tree.insert("key2", "value2");
    tree.insert("key3", "value3");
    EXPECT_EQ(tree.search("key1"), "value1");
    EXPECT_EQ(tree.search("key2"), "value2");
    EXPECT_EQ(tree.search("key3"), "value3");
}

TEST(AVLTreeTest, LeftRightBalance) {
    AVLTree tree;
    tree.insert("key3", "value3");
    tree.insert("key1", "value1");
    tree.insert("key2", "value2");
    EXPECT_EQ(tree.search("key1"), "value1");
    EXPECT_EQ(tree.search("key2"), "value2");
    EXPECT_EQ(tree.search("key3"), "value3");
}

TEST(AVLTreeTest, RightLeftBalance) {
    AVLTree tree;
    tree.insert("key1", "value1");
    tree.insert("key3", "value3");
    tree.insert("key2", "value2");
    EXPECT_EQ(tree.search("key1"), "value1");
    EXPECT_EQ(tree.search("key2"), "value2");
    EXPECT_EQ(tree.search("key3"), "value3");
}

TEST(AVLTreeTest, MultipleOperations) {
    AVLTree tree;
    tree.insert("key1", "value1");
    tree.insert("key2", "value2");
    tree.insert("key3", "value3");
    EXPECT_EQ(tree.search("key2"), "value2");
    tree.insert("key2", "updated_value2");
    EXPECT_EQ(tree.search("key2"), "updated_value2");
    tree.remove("key1");
    EXPECT_THROW(tree.search("key1"), std::runtime_error);
    tree.insert("key4", "value4");
    EXPECT_EQ(tree.search("key4"), "value4");
    EXPECT_EQ(tree.search("key3"), "value3");
}

//Тесты для HashTable
TEST(HashTableTest, Init) {
    HashTable ht(20);
    EXPECT_THROW(ht.search("Dima"), std::runtime_error);
}

TEST(HashTableTest, InsertAndSearch) {
    HashTable ht(20);
    ht.insert("Dima", "Carpenter");
    EXPECT_EQ(ht.search("Dima"), "Carpenter");
    EXPECT_THROW(ht.search("Roma"), std::runtime_error);
}

TEST(HashTableTest, Update) {
    HashTable ht(20);
    ht.insert("Dima", "Carpenter");
    ht.update("Dima", "Programmer");
    EXPECT_EQ(ht.search("Dima"), "Programmer");
}

TEST(HashTableTest, Delete) {
    HashTable ht(20);
    ht.insert("Dima", "Carpenter");
    ht.remove("Dima");
    EXPECT_THROW(ht.search("Dima"), std::runtime_error);
}

TEST(HashTableTest, DeleteNonExistent) {
    HashTable ht(20);
    EXPECT_THROW(ht.remove("Dima"), std::runtime_error);
}

TEST(HashTableTest, InvalidKeyLength) {
    HashTable ht(20);
    EXPECT_THROW(ht.insert("A", "Value"), std::invalid_argument);
    EXPECT_THROW(ht.search(""), std::invalid_argument);
}

TEST(HashTableTest, InvalidKeyCharacters) {
    HashTable ht(20);
    EXPECT_THROW(ht.insert("12Key", "Value"), std::invalid_argument);
    EXPECT_THROW(ht.insert("@#Key", "Value"), std::invalid_argument);
}

TEST(HashTableTest, CollisionHandling) {
    HashTable ht(20);
    ht.insert("Dima", "Carpenter");
    ht.insert("Dino", "Artist");
    EXPECT_EQ(ht.search("Dima"), "Carpenter");
    EXPECT_EQ(ht.search("Dino"), "Artist");
    ht.remove("Dima");
    EXPECT_THROW(ht.search("Dima"), std::runtime_error);
    EXPECT_EQ(ht.search("Dino"), "Artist");
}

TEST(HashTableTest, MultipleOperations) {
    HashTable ht(20);
    ht.insert("Dima", "Carpenter");
    ht.insert("Roma", "Locksmith");
    ht.insert("Alex", "Painter");
    EXPECT_EQ(ht.search("Dima"), "Carpenter");
    EXPECT_EQ(ht.search("Roma"), "Locksmith");
    ht.update("Dima", "Programmer");
    EXPECT_EQ(ht.search("Dima"), "Programmer");
    ht.remove("Roma");
    EXPECT_THROW(ht.search("Roma"), std::runtime_error);
    ht.insert("Bob", "Engineer");
    EXPECT_EQ(ht.search("Bob"), "Engineer");
    EXPECT_THROW(ht.insert("12", "Invalid"), std::invalid_argument);
}
