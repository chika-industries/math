"""
Monte Carlo Simulation for Portfolio Antifragility Analysis

This module implements a Monte Carlo simulation using Geometric Brownian Motion (GBM)
to analyze portfolio resilience and antifragility under various market scenarios.
The implementation follows Nassim Taleb's "barbell strategy" concept, where a portion
of capital is kept as cash reserves to exploit market downturns.

The simulation generates alternative financial histories to assess portfolio exposure
to random events and determine whether the investment strategy exhibits fragile or
antifragile characteristics.

Mathematical Foundation:
    dS_t = μ * S_t * dt + σ * S_t * dW_t
    Where:
        S_t = Stock price at time t
        μ   = Expected return (drift)
        σ   = Volatility
        dW_t = Wiener process (Brownian motion)

References:
    - Taleb, N. N. (2012). Antifragile: Things That Gain from Disorder
    - Geometric Brownian Motion: https://quantpy.com.au/stochastic-calculus/simulating-geometric-brownian-motion-gbm-in-python/
    - Munger's $100k principle: The importance of reaching critical mass in wealth accumulation

Author: [Your Name]
Date: [Current Date]
"""

import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns

# Configuration
logarithmic_plot = False  # Toggle for logarithmic y-axis scaling

# === SIMULATION PARAMETERS ===
n = 252        # Number of time steps (trading days per year)
T = 10         # Time horizon in years (focus on medium-term volatility effects)
M = 20000      # Number of Monte Carlo simulations (alternative histories)

# === PORTFOLIO CONFIGURATION ===
S0 = 140000    # Initial portfolio value (USD)
working_capital = 14000  # Cash reserves for barbell strategy (USD)

# Calculate barbell allocation as percentage of total assets
barbell_percentage = working_capital / S0

# === MARKET REGIME PARAMETERS ===
# Test various market conditions to assess portfolio robustness
# Current configuration: Normal growth with medium volatility

# Regime 1: Strong growth, high volatility (50% drawdown possible)
# sigma = 0.17, mu = 0.15  # High vol: 385 → 190 pullback potential

# Regime 2: Normal growth, medium volatility (current setting)
sigma = 0.2   # Volatility: potential 385 → 315 pullback
mu = 0.1      # Expected annual return (10%)

# Regime 3: Bull market, low volatility
# sigma = 0.1, mu = 0.1   # Low vol: 385 → 333 pullback potential

# Test scenarios (for validation)
# sigma = 0     # Zero volatility (compound interest approximation)
# sigma = 0.05  # Minimal volatility


def simulate_gbm(S0, mu, sigma, T, n, M):
    """
    Simulate Geometric Brownian Motion paths using vectorized NumPy operations.
    
    Args:
        S0 (float): Initial asset price
        mu (float): Expected return (drift parameter)
        sigma (float): Volatility parameter  
        T (float): Time horizon
        n (int): Number of time steps
        M (int): Number of simulation paths
        
    Returns:
        tuple: (time_array, price_paths) where price_paths is (n+1, M) array
    """
    dt = T / n  # Time step size
    
    # Generate random increments for all paths simultaneously
    random_increments = np.random.normal(0, np.sqrt(dt), size=(M, n))
    
    # Calculate price ratios using GBM formula
    price_ratios = np.exp(
        (mu - sigma**2 / 2) * dt + sigma * random_increments.T
    )
    
    # Add initial condition and compute cumulative product
    price_ratios = np.vstack([np.ones(M), price_ratios])
    price_paths = S0 * price_ratios.cumprod(axis=0)
    
    # Create time array
    time_array = np.linspace(0, T, n + 1)
    
    return time_array, price_paths


def analyze_portfolio_resilience(price_paths, working_capital, critical_threshold=100000):
    """
    Analyze portfolio resilience under extreme scenarios.
    
    The critical threshold represents Charlie Munger's principle that the first
    $100,000 is the most difficult to accumulate and serves as a foundation
    for future wealth building.
    
    Args:
        price_paths (ndarray): Simulated price paths
        working_capital (float): Available cash reserves
        critical_threshold (float): Minimum portfolio value to maintain
        
    Returns:
        dict: Analysis results including worst-case scenarios and statistics
    """
    # Identify worst-case scenario across all simulations
    min_portfolio_value = np.min(price_paths)
    total_assets_worst_case = min_portfolio_value + working_capital
    
    # Calculate final portfolio statistics
    final_values = price_paths[-1]
    final_values_with_barbell = final_values + working_capital
    
    results = {
        'min_portfolio_value': min_portfolio_value,
        'total_assets_worst_case': total_assets_worst_case,
        'survives_threshold': total_assets_worst_case > critical_threshold,
        'additional_barbell_needed': max(0, critical_threshold - min_portfolio_value),
        'final_mean': np.mean(final_values),
        'final_std': np.std(final_values),
        'final_mean_with_barbell': np.mean(final_values_with_barbell),
        'final_std_with_barbell': np.std(final_values_with_barbell)
    }
    
    return results


def create_visualization(time_array, price_paths, logarithmic=False):
    """Create and save visualization of Monte Carlo simulation results."""
    # Prepare time matrix for plotting
    time_matrix = np.full(shape=(M, n + 1), fill_value=time_array).T
    
    plt.figure(figsize=(12, 8))
    plt.plot(time_matrix, price_paths, alpha=0.1, linewidth=0.5)
    
    if logarithmic:
        plt.yscale("log")
    
    plt.xlabel("Time (years)")
    plt.ylabel("Portfolio Value (USD)")
    plt.title(
        f"Monte Carlo Simulation: Geometric Brownian Motion\n"
        f"dS_t = μS_t dt + σS_t dW_t\n"
        f"S₀ = ${S0:,}, μ = {mu:.1%}, σ = {sigma:.1%}, Paths = {M:,}"
    )
    plt.grid(True, alpha=0.3)
    
    filename = "monte_carlo_gbm_log.png" if logarithmic else "monte_carlo_gbm.png"
    plt.savefig(filename, dpi=300, bbox_inches='tight')
    plt.show()


def create_distribution_plot(final_values):
    """Create kernel density estimation plot of final portfolio values."""
    plt.figure(figsize=(10, 6))
    sns.kdeplot(final_values, bw_adjust=0.5, fill=True, alpha=0.7)
    
    plt.xlabel('Final Portfolio Value (USD)')
    plt.ylabel('Probability Density')
    plt.title('Distribution of Final Portfolio Values\n(Kernel Density Estimation)')
    plt.grid(True, alpha=0.3)
    
    # Add statistical annotations
    mean_val = np.mean(final_values)
    plt.axvline(mean_val, color='red', linestyle='--', alpha=0.8, 
                label=f'Mean: ${mean_val:,.0f}')
    plt.legend()
    
    plt.tight_layout()
    plt.show()


def print_analysis_report(results, mu, sigma, S0, barbell_percentage, M):
    """Print comprehensive analysis report."""
    print("=" * 60)
    print("PORTFOLIO ANTIFRAGILITY ANALYSIS REPORT")
    print("=" * 60)
    print(f"Market Conditions:")
    print(f"  Expected Return (μ) ............................ {mu:.1%}")
    print(f"  Volatility (σ) ................................. {sigma:.1%}")
    print(f"  Initial Portfolio Value ........................ ${S0:,}")
    print(f"  Barbell Allocation ............................. {barbell_percentage:.1%}")
    print()
    
    print(f"Monte Carlo Analysis ({M:,} simulations):")
    print(f"  Worst-case Portfolio Value ..................... ${results['min_portfolio_value']:,.0f}")
    print(f"  Worst-case Total Assets (incl. barbell) ........ ${results['total_assets_worst_case']:,.0f}")
    print()
    
    print("Resilience Assessment (Munger's $100k Principle):")
    if results['survives_threshold']:
        print("  ✓ Portfolio survives extreme scenarios")
        print("    Even worst-case maintains critical mass for wealth building")
    else:
        print("  ✗ Portfolio vulnerable to extreme scenarios")
        print(f"    Additional barbell needed: ${results['additional_barbell_needed']:,.0f}")
    print()
    
    print("Final Portfolio Statistics:")
    print(f"  Mean Final Value ............................... ${results['final_mean']:,.0f}")
    print(f"  Standard Deviation ............................. ${results['final_std']:,.0f}")
    print(f"  Mean Final Value (with barbell) ............... ${results['final_mean_with_barbell']:,.0f}")
    print(f"  Standard Deviation (with barbell) ............. ${results['final_std_with_barbell']:,.0f}")
    print("=" * 60)


# === MAIN EXECUTION ===
if __name__ == "__main__":
    # Run Monte Carlo simulation
    print("Running Monte Carlo simulation...")
    time_array, price_paths = simulate_gbm(S0, mu, sigma, T, n, M)
    
    # Analyze results
    results = analyze_portfolio_resilience(price_paths, working_capital)
    
    # Generate visualizations
    create_visualization(time_array, price_paths, logarithmic_plot)
    create_distribution_plot(price_paths[-1])
    
    # Print comprehensive report
    print_analysis_report(results, mu, sigma, S0, barbell_percentage, M)
    
    # Optional: Display raw final values for detailed analysis
    print("\nFinal Portfolio Values (sample):")
    print(price_paths[-1][:10])  # Show first 10 values as sample
