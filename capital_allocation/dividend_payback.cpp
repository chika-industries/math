/**
 * @file dividend_payback.cpp
 * @brief Calculates the payback period for a dividend-paying stock investment
 *
 * This program computes how many years it takes to recover an initial stock
 * investment through dividend payments, assuming a constant annual dividend
 * growth rate. The calculation accounts for compounding dividend growth but
 * does not consider dividend reinvestment or stock price appreciation.
 *
 * @author AI-generated
 * @date 2025-09-30
 */

#include <iostream>
#include <iomanip>

 /**
  * @brief Main function that calculates dividend payback period
  *
  * Simulates year-by-year dividend payments with constant growth rate
  * until cumulative dividends equal or exceed the initial investment.
  *
  * @return 0 on successful execution
  */
int main() {
    // Investment parameters - configured for Pfizer stock analysis
    const double initialInvestment = 23.8;      // Initial investment amount (1 share of Pfizer)
    const double initialDividendYield = 0.0724; // Starting dividend yield (7.24%)
    const double dividendGrowthRate = 0.02;     // Expected annual dividend growth rate (2%)

    // Calculate the first year's dividend payment
    // Formula: Annual Dividend = Investment × Yield
    double annualDividend = initialInvestment * initialDividendYield;

    // Accumulator for total dividends received across all years
    double totalDividendsReceived = 0.0;

    // Counter for number of years elapsed
    int years = 0;

    // Configure output formatting for currency display
    std::cout << std::fixed << std::setprecision(2);

    // Display input parameters
    std::cout << "Initial Investment: $" << initialInvestment << std::endl;
    std::cout << "Initial Dividend Yield: " << (initialDividendYield * 100) << "%" << std::endl;
    std::cout << "Dividend Growth Rate: " << (dividendGrowthRate * 100) << "%" << std::endl;
    std::cout << "\n";

    // Display table header for year-by-year breakdown
    std::cout << "Year\tDividend\tCumulative" << std::endl;
    std::cout << "====\t========\t==========" << std::endl;

    // Main calculation loop: iterate until cumulative dividends >= initial investment
    // Each iteration represents one year of dividend payments
    while (totalDividendsReceived < initialInvestment) {
        years++;

        // Add current year's dividend to cumulative total
        totalDividendsReceived += annualDividend;

        // Display current year's results
        std::cout << years << "\t$" << annualDividend
            << "\t\t$" << totalDividendsReceived << std::endl;

        // Apply growth rate for next year's dividend
        // Formula: Next Dividend = Current Dividend × (1 + Growth Rate)
        annualDividend *= (1 + dividendGrowthRate);
    }

    // Display final results summary
    std::cout << "\n";
    std::cout << "It takes " << years << " years to recover the initial investment." << std::endl;
    std::cout << "Total dividends received: $" << totalDividendsReceived << std::endl;

    return 0;
}
