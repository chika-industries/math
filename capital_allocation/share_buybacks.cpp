#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>

// Utility function to format currency with abbreviations
std::string formatCurrency(double value) {
    std::string result;

    if (value >= 1e9) {
        result = "$" + std::to_string(value / 1e9);
        result = result.substr(0, result.find('.') + 3) + "B";
    }
    else if (value >= 1e6) {
        result = "$" + std::to_string(value / 1e6);
        result = result.substr(0, result.find('.') + 3) + "M";
    }
    else if (value >= 1e3) {
        result = "$" + std::to_string(value / 1e3);
        result = result.substr(0, result.find('.') + 3) + "K";
    }
    else {
        result = "$" + std::to_string(static_cast<int>(value));
    }

    return result;
}

// Utility function to format share count
std::string formatShares(double shares) {
    if (shares >= 1e6) {
        std::string result = std::to_string(shares / 1e6);
        return result.substr(0, result.find('.') + 3) + "M shares";
    }
    else if (shares >= 1e3) {
        std::string result = std::to_string(shares / 1e3);
        return result.substr(0, result.find('.') + 3) + "K shares";
    }
    else {
        return std::to_string(static_cast<int>(shares)) + " shares";
    }
}

class Company {
private:
    double earnings;               ///< Current annual earnings
    double shares_outstanding;     ///< Number of shares outstanding
    double book_value;             ///< Total book value of the company
    double earnings_growth_rate;   ///< Annual earnings growth rate (default 10%)
    double share_buyback_rate;     ///< Annual share buyback rate (default 5%)
    double dividend_payout_ratio;  ///< Dividend payout ratio (default 30%)

public:
    // Constructor
    Company(double initial_earnings,
        double initial_shares,
        double initial_book_value,
        double growth_rate = 0.10,
        double buyback_rate = 0.05,
        double payout_ratio = 0.30)
        : earnings(initial_earnings), shares_outstanding(initial_shares), book_value(initial_book_value),
        earnings_growth_rate(growth_rate), share_buyback_rate(buyback_rate),
        dividend_payout_ratio(payout_ratio) {}

    // Simulate one year of operations
    void simulateYear(int year) {
        std::cout << "\n-----------------------------------------------" << std::endl;
        std::cout << "                   YEAR " << std::setw(2) << year << "                    " << std::endl;
        std::cout << "-----------------------------------------------" << std::endl;

        std::cout << "   Starting Position:" << std::endl;
        std::cout << "   Book Value: " << formatCurrency(book_value) << std::endl;
        std::cout << "   Earnings:   " << formatCurrency(earnings) << std::endl;
        std::cout << "   Shares:     " << formatShares(shares_outstanding) << std::endl;

        // Step 1: Grow earnings by specified rate
        earnings *= (1 + earnings_growth_rate);
        std::cout << "\n  Earnings Growth (" << (earnings_growth_rate * 100) << "%): "
            << formatCurrency(earnings) << std::endl;

        // Step 2: Calculate dividends
        double total_dividends = earnings * dividend_payout_ratio;
        double dividend_per_share = total_dividends / shares_outstanding;   // NEW
        std::cout << "  Dividends Paid (" << (dividend_payout_ratio * 100) << "%): "
            << formatCurrency(total_dividends)
            << " (" << formatCurrency(dividend_per_share) << " per share)" << std::endl;

        // Step 3: Calculate retained earnings
        double retained_earnings = earnings * (1 - dividend_payout_ratio);
        std::cout << "  Retained Earnings: " << formatCurrency(retained_earnings) << std::endl;

        // Step 4: Calculate share buyback amount
        double shares_to_buyback = shares_outstanding * share_buyback_rate;
        double share_price = book_value / shares_outstanding; // Simplified: assume trading at book value
        double buyback_cost = shares_to_buyback * share_price;
        std::cout << "  Share Buyback (" << (share_buyback_rate * 100) << "%): "
            << formatShares(shares_to_buyback) << " costing " << formatCurrency(buyback_cost) << std::endl;

        // Step 5: Update shares outstanding
        shares_outstanding -= shares_to_buyback;

        // Step 6: Update book value
        book_value = book_value + retained_earnings - total_dividends - buyback_cost;

        std::cout << "\n  End of Year Results:" << std::endl;
        std::cout << "   Book Value:         " << formatCurrency(book_value) << std::endl;
        std::cout << "   Shares:             " << formatShares(shares_outstanding) << std::endl;
        std::cout << "   Book Value/Share:   " << formatCurrency(book_value / shares_outstanding) << std::endl;
        std::cout << "   Earnings/Share:     " << formatCurrency(earnings / shares_outstanding) << std::endl; // NEW
        std::cout << "   Dividend/Share:     " << formatCurrency(dividend_per_share) << std::endl;             // NEW
    }
    // Inside Company class
    void plotDividends(int years) {
        std::cout << "\n╔══════════════════════════════════════════════════╗" << std::endl;
        std::cout << "║        📊 DIVIDEND PER SHARE PROJECTION         ║" << std::endl;
        std::cout << "╚══════════════════════════════════════════════════╝" << std::endl;

        double local_earnings = earnings;
        double local_book_value = book_value;
        double local_shares = shares_outstanding;

        for (int year = 1; year <= years; ++year) {
            // grow earnings
            local_earnings *= (1 + earnings_growth_rate);

            // dividends
            double total_dividends = local_earnings * dividend_payout_ratio;
            double dividend_per_share = total_dividends / local_shares;

            // update retained earnings + buyback
            double retained_earnings = local_earnings * (1 - dividend_payout_ratio);
            double shares_to_buyback = local_shares * share_buyback_rate;
            double share_price = local_book_value / local_shares;
            double buyback_cost = shares_to_buyback * share_price;
            local_shares -= shares_to_buyback;
            local_book_value = local_book_value + retained_earnings - total_dividends - buyback_cost;

            // scaling factor to avoid too many dots (tune if needed)
            int dots = static_cast<int>(dividend_per_share);
            if (dots > 60) dots = 60; // cap so it doesn't explode

            std::cout << "Year " << std::setw(2) << year
                << " | " << std::setw(6) << std::fixed << std::setprecision(2)
                << dividend_per_share << " $/share "
                << " : " << std::string(dots, '.') << "●" << std::endl;
        }
    }

    // Project book value for multiple years
    void projectGrowth(int years) {
        std::cout << "\n╔══════════════════════════════════════════════════╗" << std::endl;
        std::cout << "║          📈 FINANCIAL PROJECTION MODEL          ║" << std::endl;
        std::cout << "╚══════════════════════════════════════════════════╝" << std::endl;

        std::cout << "\n⚙️  Model Parameters:" << std::endl;
        std::cout << "   • Earnings Growth:   " << (earnings_growth_rate * 100) << "% annually" << std::endl;
        std::cout << "   • Share Buybacks:    " << (share_buyback_rate * 100) << "% annually" << std::endl;
        std::cout << "   • Dividend Payout:   " << (dividend_payout_ratio * 100) << "% of earnings" << std::endl;

        double initial_book_value = book_value;

        for (int year = 1; year <= years; year++) {
            simulateYear(year);
        }

        std::cout << "\n╔══════════════════════════════════════════════════╗" << std::endl;
        std::cout << "║              🎯 " << years << "-YEAR SUMMARY                ║" << std::endl;
        std::cout << "╚══════════════════════════════════════════════════╝" << std::endl;

        std::cout << "Final Book Value:        " << formatCurrency(book_value) << std::endl;
        std::cout << "Final Annual Earnings:   " << formatCurrency(earnings) << std::endl;
        std::cout << "Final Shares:            " << formatShares(shares_outstanding) << std::endl;
        std::cout << "Final Book Value/Share:  " << formatCurrency(getBookValuePerShare()) << std::endl;
        std::cout << "Final Earnings/Share:    " << formatCurrency(earnings / shares_outstanding) << std::endl;
        std::cout << "Final Dividend/Share:    "
            << formatCurrency((earnings * dividend_payout_ratio) / shares_outstanding) << std::endl;

        double cagr = calculateBookValueCAGR(initial_book_value, years) * 100;
        std::cout << "Book Value CAGR:         " << std::fixed << std::setprecision(1) << cagr << "%" << std::endl;


    }

    // Getter methods
    double getBookValue() const { return book_value; }
    double getEarnings() const { return earnings; }
    double getSharesOutstanding() const { return shares_outstanding; }
    double getBookValuePerShare() const { return book_value / shares_outstanding; }

    // Setter methods
    void setEarningsGrowthRate(double rate) { earnings_growth_rate = rate; }
    void setShareBuybackRate(double rate) { share_buyback_rate = rate; }
    void setDividendPayoutRatio(double ratio) { dividend_payout_ratio = ratio; }

    // Calculate compound annual growth rate
    double calculateBookValueCAGR(double initial_book_value, int years) const {
        return std::pow(book_value / initial_book_value, 1.0 / years) - 1.0;
    }

    // Display current metrics
    void displayCurrentMetrics() const {
        std::cout << "\n┌─────────────────────────────────────────────┐" << std::endl;
        std::cout << "│           📋 COMPANY OVERVIEW              │" << std::endl;
        std::cout << "└─────────────────────────────────────────────┘" << std::endl;

        std::cout << "💼 Financial Position:" << std::endl;
        std::cout << "   Book Value:           " << formatCurrency(book_value) << std::endl;
        std::cout << "   Annual Earnings:      " << formatCurrency(earnings) << std::endl;
        std::cout << "   Shares Outstanding:   " << formatShares(shares_outstanding) << std::endl;
        std::cout << "   Book Value/Share:     " << formatCurrency(getBookValuePerShare()) << std::endl;
        std::cout << "   Earnings/Share:       " << formatCurrency(earnings / shares_outstanding) << std::endl;
        std::cout << "   Dividend/Share:       " << formatCurrency((earnings * dividend_payout_ratio) / shares_outstanding) << std::endl;

        std::cout << "\n⚙️  Operating Parameters:" << std::endl;
        std::cout << "   Earnings Growth:      " << (earnings_growth_rate * 100) << "%" << std::endl;
        std::cout << "   Share Buyback Rate:   " << (share_buyback_rate * 100) << "%" << std::endl;
        std::cout << "   Dividend Payout:      " << (dividend_payout_ratio * 100) << "%" << std::endl;
    }

    // Reset company to initial state
    void reset(double initial_earnings, double initial_shares, double initial_book_value) {
        earnings = initial_earnings;
        shares_outstanding = initial_shares;
        book_value = initial_book_value;
    }
};

// Function that can be called from your buyback_math.cpp
int buyback_go() {
    // Initialize company with starting values
    double initial_earnings = 1000000;    // $1M initial earnings
    double initial_shares = 100000;       // 100K shares outstanding
    double initial_book_value = 5000000;  // $5M initial book value

    // Some examples
    //Company company(initial_earnings, initial_shares, initial_book_value, 0.10, 0.05, 0.30); // Example with extreme buyback
    Company company(initial_earnings, initial_shares, initial_book_value, 0.10, 0.02, 0.30);   // Example with few opportunities of buybacks
    //Company company(initial_earnings, initial_shares, initial_book_value, 0.10, 0.0, 0.30);  // Example if shares are overvalued and no buyback chance
    
    std::cout << "----------------------------------------------------" << std::endl;
    std::cout << "                CHIKA INDUSTRIES MODEL              " << std::endl;
    std::cout << "----------------------------------------------------" << std::endl;

    company.displayCurrentMetrics();

    // Project growth for 5 years with default parameters
    company.projectGrowth(5);

    // NEW: visualize dividends per share
    company.reset(initial_earnings, initial_shares, initial_book_value);
    company.plotDividends(20);

    return 0;
}

int main()
{
    buyback_go();
    return 0;
}
