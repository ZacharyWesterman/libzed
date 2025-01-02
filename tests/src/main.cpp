// Do not add tests to this file.
// This file is intentionally left mostly blank.
// Catch2 will automatically generate the entry point.

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <iostream>

class resultsReporter : public Catch::StreamingReporterBase<resultsReporter> {
public:
	using StreamingReporterBase::StreamingReporterBase;

	static std::string getDescription() {
		return "Reporter for getting just the results of the test run";
	}

	void testRunEnded(Catch::TestRunStats const &testRunStats) override {
		std::cout << testRunStats.totals.assertions.passed << " " << testRunStats.totals.assertions.failed << std::endl;
	}

	void assertionStarting(Catch::AssertionInfo const &info) override {}

	bool assertionEnded(Catch::AssertionStats const &assertionStats) override {
		return true;
	}
};

CATCH_REGISTER_REPORTER("results", resultsReporter)