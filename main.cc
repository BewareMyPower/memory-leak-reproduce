/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
#include <chrono>
#include <pulsar/Client.h>
#include <signal.h>
#include <thread>

using namespace pulsar;

int main() {
  static volatile bool running = true;
  struct sigaction handler;
  handler.sa_flags = 0;
  handler.sa_handler = [](int signal) { running = false; };
  sigaction(SIGINT, &handler, nullptr);

  Client client("pulsar://host.docker.internal:6650");
  int i = 0;
  while (running) {
    Producer producer;
    auto result = client.createProducer("my-topic", producer);
    if (result != ResultOk) {
      break;
    }
    producer.send(
        MessageBuilder().setContent("msg-" + std::to_string(i)).build());
    producer.close();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
  client.close();
}
