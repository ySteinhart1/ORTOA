#include <chrono>
#include <fstream>
#include <numeric>
#include <sodium.h>
#include <sstream>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include "../constants/constants.h"
#include "../crypto/encryption_engine.h"
#include "../gen-cpp/RPC.h"
#include "../host/redis.h"
#include "client_utils.h"
#include "spdlog/spdlog.h"

using namespace std::chrono;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

class ClientHandler {
  private:
    ClientConfig config;
    std::vector<double> latencies;

  public:
    ClientHandler(int argc, char *argv[]) { parseArgs(argc, argv, config); }

    void start() { (config.init_db) ? initDB() : runThreaded(); }

    void initDB() {
        // Number of operations to perform corresponds to maximum key (if seed data is not used)
        config.num_operations = config.max_key;

        redisCli rd;
        auto pipeline = rd.pipe();

        while (moreOperationsExist(config)) {
            Operation op = getInitKV(config);
            pipeline.set(op.key, op.value);
        }
        pipeline.exec();
    }

    void runThreaded() {
        std::vector<std::thread> threads;
        for (int i = 0; i < config.num_clients; i++) {
            threads.push_back(std::thread(&ClientHandler::run, this));
        }

        // Wait for all threads to finish
        for (std::thread &thread : threads) {
            thread.join();
        }

        getAveLatency();
    }

    void run() {
        auto socket = std::make_shared<TSocket>(HOST_IP, HOST_PORT);
        auto transport = std::make_shared<TBufferedTransport>(socket);
        auto protocol = std::make_shared<TBinaryProtocol>(transport);
        RPCClient client(protocol);

        transport->open();

        std::string val;
        // If seed data exists, run the client with data
        while (moreOperationsExist(config)) {
            Operation op = getOperation(config);
            auto start = high_resolution_clock::now();
            client.access(val, op);
            auto end = high_resolution_clock::now();
            latencies.push_back(
                duration_cast<microseconds>(end - start).count());
        }

        transport->close();
    }

    void getAveLatency() {
        double aveLatency = std::accumulate(latencies.begin(), latencies.end(), 0.0) /
                         latencies.size();
        spdlog::info("[Client]: Data access complete, average latency: {0} microseconds", aveLatency);
    }
};

int main(int argc, char *argv[]) {
    try {
        ClientHandler client(argc, argv);

        auto start = high_resolution_clock::now();
        client.start();
        auto end = high_resolution_clock::now();

        spdlog::info("[main]: Entire program finished in {0} microseconds", duration_cast<microseconds>(end - start).count());
    } catch (std::runtime_error err) {
        spdlog::error("Client | {0}", err.what());
    } catch (TException &err) {
        spdlog::error("Client | {0}", err.what());
    }
}
