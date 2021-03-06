#include <gtest/gtest.h>
#include <soft.h>
#include <tripletstore.h>
#include <algorithm>

class TripletStoreTest : public ::testing::Test {
protected:
  TripletStoreTest() {}
  virtual ~TripletStoreTest(){}
};

TEST_F(TripletStoreTest, Constructor)
{
  soft::TripletStore *triplets = new soft::TripletStore();
  ASSERT_TRUE(NULL != triplets);

  delete triplets;
}

TEST_F(TripletStoreTest, addTriplet)
{
  soft::TripletStore triplets;
  triplets.addTriplet("Porto", "is", "SOFT5");
  auto ts = triplets.findTriplets("Porto", "is");
  ASSERT_TRUE(ts.size() == 1);
  ASSERT_STREQ(ts.front().c_str(), "SOFT5");
}

TEST_F(TripletStoreTest, revFind)
{
  soft::TripletStore triplets;
  triplets.addTriplet("Porto", "is", "SOFT5");
  auto ts = triplets.findTriplets("SOFT5", "^is");
  ASSERT_TRUE(ts.size() == 1);
  ASSERT_STREQ(ts.front().c_str(), "Porto");
}

TEST_F(TripletStoreTest, toFromToCSV) {
  soft::TripletStore triplets;
  triplets.addTriplet("Dune", "full-of", "sand");
  triplets.addTriplet("spice", "controls", "universe");
  triplets.addTriplet("fear", "is-a", "mind-killer");

  ASSERT_EQ(1, triplets.findTriplets("Dune", "full-of").size());
  ASSERT_EQ(1, triplets.findTriplets("spice", "controls").size());
  ASSERT_EQ(1, triplets.findTriplets("fear", "is-a").size());
  ASSERT_EQ(0, triplets.findTriplets("Luke", "child-of").size());

  std::string csv = triplets.toCSV();

  soft::TripletStore triplets2;
  triplets2.fromCSV(csv);

  ASSERT_EQ(1, triplets2.findTriplets("Dune", "full-of").size()) << "Not yet implemented";
  ASSERT_EQ(1, triplets2.findTriplets("spice", "controls").size());
  ASSERT_EQ(1, triplets2.findTriplets("fear", "is-a").size());
  ASSERT_EQ(0, triplets2.findTriplets("Luke", "child-of").size());
}

TEST_F(TripletStoreTest, testReverseLookup) {
  soft::TripletStore triplets;
  triplets.addTriplet("Dune", "full-of", "sand");
  std::list<std::string> s = triplets.findTriplets("sand", "^full-of");
  ASSERT_EQ(1, s.size());
  ASSERT_EQ("Dune", s.front());
}

TEST_F(TripletStoreTest, testFindAllTriplets) {
  soft::TripletStore triplets;
  triplets.addTriplet("Dune", "full-of", "sand");
  triplets.addTriplet("Space", "has", "Dune");

  std::list<soft::TripletStore::Triplet> ts = triplets.allTriplets();
  
  EXPECT_EQ(2 * 2, ts.size());

  {
    auto t1 = std::find_if(ts.begin(), ts.end(), [](const soft::TripletStore::Triplet &t) {
      if (t.subject == "Dune" && t.predicate == "full-of" && t.object == "sand")
        return true;
      return false;
    });

    EXPECT_NE(ts.end(), t1);
    EXPECT_EQ(t1->subject, "Dune");
    EXPECT_EQ(t1->predicate, "full-of");
    EXPECT_EQ(t1->object, "sand");
  }
  
  {
    auto t1 = std::find_if(ts.begin(), ts.end(), [](const soft::TripletStore::Triplet &t) {
      if (t.subject == "sand" && t.predicate == "^full-of" && t.object == "Dune")
        return true;
      return false;
    });
    
    EXPECT_NE(ts.end(), t1);
    EXPECT_EQ(t1->subject, "sand");
    EXPECT_EQ(t1->predicate, "^full-of");
    EXPECT_EQ(t1->object, "Dune");
  }

  {
    auto t1 = std::find_if(ts.begin(), ts.end(), [](const soft::TripletStore::Triplet &t) {
      if (t.subject == "Space" && t.predicate == "has" && t.object == "Dune")
        return true;
      return false;
    });

    EXPECT_NE(ts.end(), t1);
    EXPECT_EQ(t1->subject, "Space");
    EXPECT_EQ(t1->predicate, "has");
    EXPECT_EQ(t1->object, "Dune");
  }

  {
    auto t1 = std::find_if(ts.begin(), ts.end(), [](const soft::TripletStore::Triplet &t) {
      if (t.subject == "Dune" && t.predicate == "^has" && t.object == "Space")
        return true;
      return false;
    });
    
    EXPECT_NE(ts.end(), t1);
    EXPECT_EQ(t1->subject, "Dune");
    EXPECT_EQ(t1->predicate, "^has");
    EXPECT_EQ(t1->object, "Space");
  }

}
