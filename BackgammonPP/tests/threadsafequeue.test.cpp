#include <catch2/catch_test_macros.hpp>

#include "utility/threadsafequeue.h"

TEST_CASE("Thead safe queue class"){
    SECTION("Should be empty on initialization"){
        // Arrange
        ThreadSafeQueue<int> q;

        // Act

        // Assert
        REQUIRE(q.empty());

    }
    SECTION("Should be empty after an equal number of push and pop operations"){
        // Arrange
        ThreadSafeQueue<int> q;

        // Act
        for(int i = 0; i < 5; i++){
            q.push(i);
        }
        for(int i = 0; i < 5; i++){
            q.pop();
        }
        // Assert
        REQUIRE(q.empty());
    }
    SECTION("Should return an empty optional on pop() if the queue is empty"){
        // Arrange
        ThreadSafeQueue<int> q;
        // Act
        auto x = q.pop();
        // Assert
        REQUIRE(!x.has_value());
    }
    SECTION("Should be FIFO"){
        // Arrange
        std::vector<int> ns = {1, 2, 3};
        ThreadSafeQueue<int> q;
        for(auto n: ns){
            q.push(n);
        }
        bool isFIFO = true;

        // Act
        for(int i = 0; !q.empty() && i < ns.size(); i++){
            auto next = q.pop();
            if(!next.has_value() || next.value() != ns[i]){
                isFIFO = false;
            }
        }

        // Assert
        REQUIRE(isFIFO);
    }
}
