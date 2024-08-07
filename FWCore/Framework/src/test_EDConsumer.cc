#include "FWCore/Framework/include/EDConsumer.h"

class TestConsumer : public edm::EDConsumer {
public:
  explicit TestConsumer() : the_int_{consumes<int>("anInt")} {}

private:
  edm::EDGetTokenT<int> the_int_;
};

int main() {
  TestConsumer consumer;

  return 0;
}
