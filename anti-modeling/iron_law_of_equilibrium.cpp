#include <iostream>
#include <iomanip>

// https://www.hussmanfunds.com/comment/observations/ob250416/

int main() {
    const double TOTAL_SHARES = 1000.0;
    const double FUNDAMENTAL_VALUE = 100.0;

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\n=== Simple Market Equilibrium Simulation ===\n";
    std::cout << "Total shares that must be held: " << TOTAL_SHARES << "\n";
    std::cout << "Fundamental value per share: $" << FUNDAMENTAL_VALUE << "\n\n";

    // SCENARIO 1: Normal equilibrium
    std::cout << "SCENARIO 1: Normal Market\n";
    std::cout << "----------------------------\n";
    double tf_wants = 500.0;  // Trend followers want 500 shares
    double vi_wants = 500.0;  // Value investors want 500 shares
    double price = 100.0;     // Price at fundamental value

    std::cout << "Price: $" << price << "\n";
    std::cout << "Trend followers want: " << tf_wants << " shares\n";
    std::cout << "Value investors want: " << vi_wants << " shares\n";
    std::cout << "Total demand: " << (tf_wants + vi_wants) << " shares\n";
    std::cout << "Market clears: " << (tf_wants + vi_wants == TOTAL_SHARES ? "YES" : "NO") << "\n\n";

    // SCENARIO 2: Price rises, trend followers get excited
    std::cout << "SCENARIO 2: Market Rally (Trend Followers 'All In')\n";
    std::cout << "---------------------------------------------------\n";
    price = 150.0;
    tf_wants = 900.0;  // Trend followers excited, want 900 shares!
    vi_wants = 100.0;  // Value investors think it's expensive, only want 100

    std::cout << "Price: $" << price << " (overvalued)\n";
    std::cout << "Trend followers want: " << tf_wants << " shares (90% of market!)\n";
    std::cout << "Value investors want: " << vi_wants << " shares (only 10%)\n";
    std::cout << "Total demand: " << (tf_wants + vi_wants) << " shares\n";
    std::cout << "Market clears: " << (tf_wants + vi_wants == TOTAL_SHARES ? "YES" : "NO") << "\n";
    std::cout << "\n** Trend followers are 'ALL IN' **\n";
    std::cout << "** Value investors are underweight (offsetting position) **\n\n";

    // SCENARIO 3: THE SHOCK - Trend followers suddenly want out
    std::cout << "SCENARIO 3: THE SHOCK - Trend Followers Want Out!\n";
    std::cout << "---------------------------------------------------\n";
    std::cout << "Trend followers suddenly want only 400 shares (down from 900)\n\n";

    tf_wants = 400.0;  // SHOCK: Trend followers want out!

    // At current price, let's see what happens
    std::cout << "At current price ($150):\n";
    vi_wants = 100.0;  // Value investors still think it's expensive
    std::cout << "  Trend followers want: " << tf_wants << " shares\n";
    std::cout << "  Value investors want: " << vi_wants << " shares\n";
    std::cout << "  Total demand: " << (tf_wants + vi_wants) << " shares\n";
    std::cout << "  SHORTAGE: " << (TOTAL_SHARES - (tf_wants + vi_wants)) << " shares need a buyer!\n\n";

    // Price must fall to make value investors willing to buy more
    std::cout << "Price drops to $120:\n";
    price = 120.0;
    vi_wants = 300.0;  // Value investors a bit more interested
    std::cout << "  Value investors want: " << vi_wants << " shares\n";
    std::cout << "  Total demand: " << (tf_wants + vi_wants) << " shares\n";
    std::cout << "  SHORTAGE: " << (TOTAL_SHARES - (tf_wants + vi_wants)) << " shares still need a buyer!\n\n";

    std::cout << "Price drops to $90:\n";
    price = 90.0;
    vi_wants = 600.0;  // Now value investors see a bargain!
    std::cout << "  Value investors want: " << vi_wants << " shares\n";
    std::cout << "  Total demand: " << (tf_wants + vi_wants) << " shares\n";
    std::cout << "  Market clears: " << (tf_wants + vi_wants == TOTAL_SHARES ? "YES!" : "NO") << "\n\n";

    std::cout << "=== THE IRON LAW ===\n";
    std::cout << "Price fell from $150 to $90 (40% crash!)\n";
    std::cout << "This was NECESSARY to induce value investors to absorb\n";
    std::cout << "the shares that trend followers no longer wanted.\n";
    std::cout << "\nThe shares don't disappear - someone must hold them!\n";

    return 0;
}
