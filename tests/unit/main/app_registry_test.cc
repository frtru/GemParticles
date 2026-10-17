// The component store and the registries components and systems register themselves with
#include <stdexcept>
#include <string>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include "main/app_registry.hh"
#include "main/app_world.hh"

namespace gem {
namespace particle {
namespace app {
namespace {
using testing::HasSubstr;

struct Speed { float value = 0.0f; };
struct Name  { std::string value; };

TEST(ComponentStoreTest, GetsNothingForMissingComponents) {
  ComponentStore store;
  EXPECT_EQ(store.Get<Speed>(), nullptr);
}

TEST(ComponentStoreTest, KeepsOneComponentPerType) {
  ComponentStore store;
  store.Add(Speed{ 1.0f });
  store.Add(Name{ "rain" });
  store.Add(Speed{ 2.0f });
  ASSERT_NE(store.Get<Speed>(), nullptr);
  EXPECT_EQ(store.Get<Speed>()->value, 2.0f);
  ASSERT_NE(store.Get<Name>(), nullptr);
  EXPECT_EQ(store.Get<Name>()->value, "rain");
}

TEST(ComponentStoreTest, GivesMutableComponents) {
  ComponentStore store;
  store.Add(Speed{ 1.0f });
  store.Get<Speed>()->value = 3.0f;
  EXPECT_EQ(store.Get<Speed>()->value, 3.0f);
}

TEST(AppRegistryTest, HasTheBuiltInSystems) {
  for (const char* system : { "DebugOverlay", "NextProjectKey", "ScriptedRun" }) {
    EXPECT_EQ(AppSystems().count(system), 1u) << system;
  }
}

TEST(AppRegistryTest, RejectsUnknownComponentTypesListingTheKnownOnes) {
  ComponentStore store;
  try {
    AddComponent({ { "type", "Teleporter" } }, store);
    FAIL() << "AddComponent accepted an unknown type";
  }
  catch (const std::runtime_error& error) {
    EXPECT_THAT(error.what(), HasSubstr("Teleporter"));
    EXPECT_THAT(error.what(), HasSubstr("DebugOverlay"));
    EXPECT_THAT(error.what(), HasSubstr("NextProjectKey"));
    EXPECT_THAT(error.what(), HasSubstr("ScriptedRun"));
  }
}

TEST(AppRegistryTest, RejectsEntriesWithoutType) {
  ComponentStore store;
  EXPECT_ANY_THROW(AddComponent({ { "key", "N" } }, store));
}

TEST(AppRegistryTest, BuildsRegisteredComponentsFromTheirEntry) {
  ASSERT_TRUE(RegisterComponentType("TestName", [](const nlohmann::json& a_Entry, ComponentStore& a_rStore) {
    a_rStore.Add(Name{ a_Entry.at("value").get<std::string>() });
  }));
  ComponentStore store;
  AddComponent({ { "type", "TestName" }, { "value", "snow" } }, store);
  ASSERT_NE(store.Get<Name>(), nullptr);
  EXPECT_EQ(store.Get<Name>()->value, "snow");
}

TEST(AppRegistryTest, KeepsTheFirstRegistrationOfAName) {
  EXPECT_FALSE(RegisterComponentType("DebugOverlay", [](const nlohmann::json&, ComponentStore&) {}));
  EXPECT_FALSE(RegisterAppSystem("DebugOverlay", AppSystem{}));
}
} /* namespace */
} /* namespace app */
} /* namespace particle */
} /* namespace gem */
