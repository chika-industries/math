"""
Fat Tails Analysis in S&P 500 Daily Returns

This script analyzes the distribution of daily percentage returns in the S&P 500 index
to demonstrate the presence of "fat tails" - a key characteristic that differentiates
real financial data from normal (Gaussian) distributions.

The analysis compares the empirical distribution with a fitted normal distribution
and uses the ratio of standard deviation to Mean Absolute Deviation (MAD) as a
fat-tail indicator, following Nassim Taleb's methodology.

Key Metrics:
- Normal Distribution: σ/MAD ≈ 1.25 (√(π/2))
- Fat-tail Distribution: σ/MAD > 1.25 (typically 1.4-2.0+ for financial data)

References:
- Taleb, N. N. "Statistical Consequences of Fat Tails"
- MAD methodology: https://www.youtube.com/watch?v=iKJy2YpYPe8&t=6s

Author: AI-generated
Date: 2024/9
"""

import yfinance as yf
import matplotlib.pyplot as plt
import numpy as np
from scipy.stats import norm
import pandas as pd


def download_sp500_data_debug():
    """Download S&P 500 data with detailed debugging information."""
    print("Downloading S&P 500 data...")
    
    # Download with explicit parameters
    data = yf.download('^GSPC', period="50y", auto_adjust=True, progress=True)
    
    print(f"Data shape: {data.shape}")
    print(f"Columns: {data.columns}")
    print(f"Column types: {type(data.columns)}")
    
    # Check if we have MultiIndex
    if isinstance(data.columns, pd.MultiIndex):
        print("MultiIndex detected - flattening columns...")
        data.columns = data.columns.get_level_values(0)
        print(f"Flattened columns: {data.columns}")
    
    print(f"First few rows:")
    print(data.head())
    print(f"Last few rows:")
    print(data.tail())
    
    return data


def fix_column_access(data):
    """Fix column access issues with yfinance data."""
    
    # Print available columns for debugging
    print(f"Available columns: {list(data.columns)}")
    
    # Try to find the price column - common names
    price_column_candidates = [
        'Close', 'Adj Close', 'close', 'adj_close', 'adj close',
        'Close Price', 'Adjusted Close', 'price'
    ]
    
    price_column = None
    for candidate in price_column_candidates:
        if candidate in data.columns:
            price_column = candidate
            print(f"Found price column: '{price_column}'")
            break
    
    # If still no column found, use the first numeric column
    if price_column is None:
        numeric_columns = data.select_dtypes(include=[np.number]).columns
        if len(numeric_columns) > 0:
            price_column = numeric_columns[0]  # Use first numeric column
            print(f"Using first numeric column as price: '{price_column}'")
        else:
            raise ValueError("No suitable price column found in the data")
    
    # Calculate daily returns
    print(f"Calculating returns using column: '{price_column}'")
    data['Daily_Return'] = data[price_column].pct_change() * 100
    
    # Remove NaN values
    data_clean = data.dropna()
    print(f"Data after cleaning: {len(data_clean)} rows")
    
    # Basic statistics check
    returns = data_clean['Daily_Return']
    print(f"Returns statistics:")
    print(f"  Min: {returns.min():.4f}%")
    print(f"  Max: {returns.max():.4f}%")
    print(f"  Mean: {returns.mean():.4f}%")
    print(f"  Std: {returns.std():.4f}%")
    print(f"  Count: {len(returns)}")
    
    return data_clean


def create_simple_analysis(data):
    """Create a simple but effective analysis of the returns."""
    returns = data['Daily_Return']
    
    # Calculate basic statistics
    mean_return = returns.mean()
    std_return = returns.std()
    mad = np.mean(np.abs(returns - mean_return))
    fat_tail_ratio = std_return / mad
    normal_ratio = np.sqrt(np.pi / 2)
    
    print("\n" + "="*60)
    print("S&P 500 FAT TAIL ANALYSIS")
    print("="*60)
    print(f"Period: {data.index[0].date()} to {data.index[-1].date()}")
    print(f"Trading days analyzed: {len(returns):,}")
    print()
    print("DISTRIBUTION STATISTICS:")
    print(f"  Mean daily return: {mean_return:.4f}%")
    print(f"  Standard deviation: {std_return:.4f}%")
    print(f"  Mean Absolute Deviation: {mad:.4f}%")
    print()
    print("FAT TAIL ANALYSIS:")
    print(f"  σ/MAD ratio (empirical): {fat_tail_ratio:.4f}")
    print(f"  σ/MAD ratio (normal): {normal_ratio:.4f}")
    print(f"  Excess fat tail factor: {fat_tail_ratio - normal_ratio:.4f}")
    print()
    
    if fat_tail_ratio > 1.4:
        print("🚨 SIGNIFICANT FAT TAILS DETECTED")
        print("   Financial returns show much higher tail risk than normal distribution")
    elif fat_tail_ratio > 1.3:
        print("⚠️  MODERATE FAT TAILS DETECTED")
        print("   Returns show elevated tail risk vs normal distribution")
    else:
        print("✅ Distribution approximately normal")
    
    print("="*60)
    
    # Create visualization
    fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2, figsize=(15, 10))
    
    # Plot 1: Histogram with normal overlay
    ax1.hist(returns, bins=100, alpha=0.7, color='lightblue', 
             density=True, edgecolor='black', linewidth=0.5)
    
    # Fit normal distribution
    x_range = np.linspace(returns.min(), returns.max(), 200)
    normal_fit = norm.pdf(x_range, mean_return, std_return)
    ax1.plot(x_range, normal_fit, 'r-', linewidth=2, label='Normal Fit')
    ax1.axvline(0, color='black', linestyle='--', alpha=0.8)
    ax1.set_title('S&P 500 Daily Returns Distribution')
    ax1.set_xlabel('Daily Return (%)')
    ax1.set_ylabel('Density')
    ax1.legend()
    ax1.grid(True, alpha=0.3)
    
    # Plot 2: Time series
    ax2.plot(data.index, returns, alpha=0.7, linewidth=0.5, color='orange')
    ax2.set_title('S&P 500 Daily Returns Over Time')
    ax2.set_xlabel('Date')
    ax2.set_ylabel('Daily Return (%)')
    ax2.axhline(0, color='red', linestyle='--', alpha=0.8)
    ax2.grid(True, alpha=0.3)
    
    # Plot 3: Zoomed histogram (focus on tails)
    # Focus on returns between -10% and +10% to see the main distribution
    main_returns = returns[(returns > -10) & (returns < 10)]
    ax3.hist(main_returns, bins=80, alpha=0.7, color='lightgreen', 
             density=True, edgecolor='black', linewidth=0.5)
    x_main = np.linspace(main_returns.min(), main_returns.max(), 200)
    normal_main = norm.pdf(x_main, mean_return, std_return)
    ax3.plot(x_main, normal_main, 'r-', linewidth=2, label='Normal Fit')
    ax3.set_title('Distribution (Zoomed: -10% to +10%)')
    ax3.set_xlabel('Daily Return (%)')
    ax3.set_ylabel('Density')
    ax3.legend()
    ax3.grid(True, alpha=0.3)
    
    # Plot 4: Fat tail comparison
    categories = ['Normal\nDistribution', 'S&P 500\nActual']
    values = [normal_ratio, fat_tail_ratio]
    bars = ax4.bar(categories, values, color=['lightblue', 'coral'], alpha=0.8)
    ax4.axhline(normal_ratio, color='blue', linestyle='--', alpha=0.8)
    ax4.set_title('Fat Tail Indicator: σ/MAD Ratio')
    ax4.set_ylabel('σ/MAD Ratio')
    ax4.grid(True, alpha=0.3)
    
    # Add value labels on bars
    for bar, value in zip(bars, values):
        ax4.text(bar.get_x() + bar.get_width()/2, bar.get_height() + 0.01,
                f'{value:.3f}', ha='center', va='bottom', fontweight='bold')
    
    plt.tight_layout()
    plt.savefig('sp500_fat_tail_analysis_fixed.png', dpi=300, bbox_inches='tight')
    plt.show()
    
    return {
        'mean': mean_return,
        'std': std_return, 
        'mad': mad,
        'fat_tail_ratio': fat_tail_ratio,
        'normal_ratio': normal_ratio
    }


def main():
    """Main execution function with comprehensive error handling."""
    try:
        # Download data with debugging
        data = download_sp500_data_debug()
        
        # Fix column access issues
        data_clean = fix_column_access(data)
        
        # Perform analysis
        results = create_simple_analysis(data_clean)
        
        print(f"\nAnalysis completed successfully!")
        print(f"Chart saved as 'sp500_fat_tail_analysis_fixed.png'")
        
        # Show extreme returns for context
        returns = data_clean['Daily_Return']
        print(f"\nEXTREME RETURNS (for context):")
        print(f"Largest daily gains:")
        top_gains = returns.nlargest(5)
        for date, ret in top_gains.items():
            print(f"  {date.date()}: +{ret:.2f}%")
        
        print(f"Largest daily losses:")
        top_losses = returns.nsmallest(5)
        for date, ret in top_losses.items():
            print(f"  {date.date()}: {ret:.2f}%")
            
    except Exception as e:
        print(f"Error in analysis: {e}")
        import traceback
        traceback.print_exc()
        
        print("\nDebugging suggestions:")
        print("1. Check internet connection")
        print("2. Update yfinance: pip install --upgrade yfinance")
        print("3. Try running: pip install pandas numpy scipy matplotlib")


if __name__ == "__main__":
    main()
