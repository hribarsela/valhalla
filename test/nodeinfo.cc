#include "test.h"

#include "baldr/nodeinfo.h"
#include "midgard/util.h"

using namespace std;
using namespace valhalla::baldr;
using namespace valhalla::midgard;

// Expected size is 32 bytes.
constexpr size_t kNodeInfoExpectedSize = 32;

namespace {

void test_sizeof() {
  if (sizeof(NodeInfo) != kNodeInfoExpectedSize)
    throw std::runtime_error("NodeInfo size should be " + std::to_string(kNodeInfoExpectedSize) +
                             " bytes but is " + std::to_string(sizeof(NodeInfo)));
}
void test_ll() {
  PointLL base_ll(-70.0f, 40.0f);
  NodeInfo n;
  if (n.latlng(base_ll).first != -70.0f || n.latlng(base_ll).second != 40.0f)
    throw std::runtime_error("NodeInfo ll should be -70, 40");

  NodeInfo t;
  PointLL nodell(-69.5f, 40.25f);
  t.set_latlng(base_ll, nodell);
  if (t.latlng(base_ll).first != nodell.lng() || t.latlng(base_ll).second != nodell.lat())
    throw std::runtime_error("NodeInfo ll should be -69.5, 40.25");

  // Test lon just outside tile bounds
  PointLL nodell1(-70.000005f, 40.25f);
  t.set_latlng(nodell1, base_ll);
  if (t.latlng(base_ll).first != base_ll.lng() || t.latlng(base_ll).second != nodell.lat())
    throw std::runtime_error("NodeInfo ll should be -70.0, 40.25");

  // Test lat just outside tile bounds
  PointLL nodell1(-69.5f, 39.999995f);
  t.set_latlng(nodell1, base_ll);
  if (t.latlng(base_ll).first != base_ll.lng() || t.latlng(base_ll).second != nodell.lat())
    throw std::runtime_error("NodeInfo ll should be -69.5, 40.0");
}

void TestWriteRead() {
  // Test building NodeInfo and reading back values
  NodeInfo nodeinfo;

  // Headings are reduced to 8 bits
  nodeinfo.set_heading(0, 266);
  nodeinfo.set_heading(1, 90);
  nodeinfo.set_heading(2, 32);
  nodeinfo.set_heading(3, 180);
  nodeinfo.set_heading(4, 185);
  nodeinfo.set_heading(5, 270);
  nodeinfo.set_heading(6, 145);
  nodeinfo.set_heading(7, 0);
  if (nodeinfo.heading(0) != 266) {
    throw runtime_error("NodeInfo heading for localidx 0 test failed " +
                        std::to_string(nodeinfo.heading(0)));
  }
  if (nodeinfo.heading(1) != 90) {
    throw runtime_error("NodeInfo heading for localidx 1 test failed " +
                        std::to_string(nodeinfo.heading(1)));
  }
  if (nodeinfo.heading(2) != 32) {
    throw runtime_error("NodeInfo heading for localidx 2 test failed " +
                        std::to_string(nodeinfo.heading(2)));
  }
  if (nodeinfo.heading(3) != 180) {
    throw runtime_error("NodeInfo heading for localidx 3 test failed " +
                        std::to_string(nodeinfo.heading(3)));
  }
  if (nodeinfo.heading(4) != 184) {
    throw runtime_error("NodeInfo heading for localidx 4 test failed " +
                        std::to_string(nodeinfo.heading(4)));
  }
  if (nodeinfo.heading(5) != 270) {
    throw runtime_error("NodeInfo heading for localidx 5 test failed " +
                        std::to_string(nodeinfo.heading(5)));
  }
  if (nodeinfo.heading(6) != 145) {
    throw runtime_error("NodeInfo heading for localidx 6 test failed " +
                        std::to_string(nodeinfo.heading(6)));
  }
  if (nodeinfo.heading(7) != 0) {
    throw runtime_error("NodeInfo heading for localidx 7 test failed " +
                        std::to_string(nodeinfo.heading(7)));
  }

  nodeinfo.set_name_consistency(0, 4, true);
  nodeinfo.set_name_consistency(3, 1, false);
  nodeinfo.set_name_consistency(2, 7, true);
  nodeinfo.set_name_consistency(6, 6, true);
  if (nodeinfo.name_consistency(0, 4) != true) {
    throw runtime_error("NodeInfo name_consistency for 0,4 test failed");
  }
  if (nodeinfo.name_consistency(4, 0) != true) {
    throw runtime_error("NodeInfo name_consistency for 4,0 test failed");
  }
  if (nodeinfo.name_consistency(1, 3) != false) {
    throw runtime_error("NodeInfo name_consistency for 1,3 test failed");
  }
  if (nodeinfo.name_consistency(7, 2) != true) {
    throw runtime_error("NodeInfo name_consistency for 7,2 test failed");
  }
  if (nodeinfo.name_consistency(6, 6) != true) {
    throw runtime_error("NodeInfo name_consistency for 6,6 test failed");
  }

  nodeinfo.set_local_driveability(3, Traversability::kBoth);
  nodeinfo.set_local_driveability(5, Traversability::kNone);
  nodeinfo.set_local_driveability(7, Traversability::kForward);
  nodeinfo.set_local_driveability(1, Traversability::kBackward);
  if (nodeinfo.local_driveability(3) != Traversability::kBoth) {
    throw runtime_error("NodeInfo local_driveability 3 test failed");
  }
  if (nodeinfo.local_driveability(5) != Traversability::kNone) {
    throw runtime_error("NodeInfo local_driveability 5 test failed");
  }
  if (nodeinfo.local_driveability(7) != Traversability::kForward) {
    throw runtime_error("NodeInfo local_driveability 7 test failed");
  }
  if (nodeinfo.local_driveability(1) != Traversability::kBackward) {
    throw runtime_error("NodeInfo local_driveability 1 test failed");
  }
}

} // namespace

int main(void) {
  test::suite suite("location");

  suite.test(TEST_CASE(test_sizeof));
  suite.test(TEST_CASE(test_ll));

  // Write to file and read into NodeInfo
  suite.test(TEST_CASE(TestWriteRead));

  // TODO: many more

  return suite.tear_down();
}
