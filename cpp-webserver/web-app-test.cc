#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "db.h"
#include "mux.h"
#include "web-app.h"

using ::testing::_;
using ::testing::Return;
using ::testing::Throw;
using namespace web::core;
using namespace web::application;

class MockDB : public IDb
{
public:
  MockDB() {}
  virtual ~MockDB() {}
  // 1 arg
  MOCK_METHOD1(runQuery, string(string));
};

class MockMux : public IMux
{
public:
  MockMux() {}
  virtual ~MockMux() {}
  // 1 arg
  MOCK_METHOD1(handleRequest, string(string));
};

// The fixture for testing class WebApp.
class WebAppTest : public ::testing::Test
{
protected:
  unique_ptr<IAppRoot> webapp;
  MockDB *db;
  MockMux *mux;

  WebAppTest() : db{new MockDB()}, mux{new MockMux}
  {
    webapp = make_unique<WebApp>(db, mux);
  }

  ~WebAppTest() override
  {
  }

  void SetUp()
  {
  }

  void TearDown()
  {
  }
};

TEST_F(WebAppTest, HappyCase)
{
    EXPECT_CALL(*mux, handleRequest(_)).WillOnce(Return(""));
    ASSERT_TRUE(webapp->run());
}

TEST_F(WebAppTest, ThrowEx)
{
    EXPECT_CALL(*mux, handleRequest(_)).WillOnce(Throw(std::runtime_error("some error")));
    EXPECT_THROW(webapp->run(), std::runtime_error);
}
