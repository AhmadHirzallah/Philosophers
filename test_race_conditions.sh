#!/bin/bash

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

FAILED_TESTS=0
TOTAL_TESTS=0

run_test() {
    local test_name="$1"
    local args="$2"
    local timeout_val="${3:-10}"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    echo -e "${YELLOW}=== Test $TOTAL_TESTS: $test_name ===${NC}"
    echo "Command: ./philo $args (timeout: ${timeout_val}s)"
    
    if timeout ${timeout_val}s ./philo $args 2>&1 | grep -q "ThreadSanitizer\|data race\|SUMMARY:"; then
        echo -e "${RED}❌ FAILED: ThreadSanitizer detected data race!${NC}"
        FAILED_TESTS=$((FAILED_TESTS + 1))
    else
        echo -e "${GREEN}✅ PASSED: No data races detected${NC}"
    fi
    echo ""
}

echo -e "${YELLOW}🧪 ThreadSanitizer Race Condition Test Suite${NC}"
echo "Building with ThreadSanitizer..."
make re > /dev/null 2>&1

if [ ! -f "./philo" ]; then
    echo -e "${RED}❌ Build failed! Make sure you're in the correct directory.${NC}"
    exit 1
fi

echo "Starting tests..."
echo ""

# Test 1: Quick death scenario
run_test "Quick Death Scenario" "4 310 200 100" 5

# Test 2: High contention with many philosophers
run_test "High Contention" "10 500 200 200" 8

# Test 3: Very tight timing
run_test "Very Tight Timing" "2 60 30 30" 3

# Test 4: Single philosopher edge case
run_test "Single Philosopher" "1 200 100 100" 3

# Test 5: Meal counting scenario
run_test "Meal Counting" "4 800 200 200 5" 10

# Test 6: Rapid execution
run_test "Rapid Execution" "6 150 50 50" 5

# Test 7: Long-running stress
run_test "Long-running Stress" "8 600 150 150" 12

# Test 8: Maximum philosophers
run_test "Maximum Philosophers" "200 800 200 200" 15

# Test 9: Minimal timing
run_test "Minimal Timing" "3 50 10 10" 3

# Test 10: Uneven timing
run_test "Uneven Timing" "5 1000 100 50" 8

echo ""
echo -e "${YELLOW}📊 Test Summary:${NC}"
echo "Total Tests: $TOTAL_TESTS"
echo "Passed: $((TOTAL_TESTS - FAILED_TESTS))"

if [ $FAILED_TESTS -eq 0 ]; then
    echo -e "${GREEN}✅ All tests passed! No race conditions detected.${NC}"
    exit 0
else
    echo -e "${RED}❌ Failed Tests: $FAILED_TESTS${NC}"
    echo -e "${RED}ThreadSanitizer detected data races. Please fix the issues.${NC}"
    exit 1
fi
