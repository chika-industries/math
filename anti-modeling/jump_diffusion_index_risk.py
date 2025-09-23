# No upward jumps (COVID recovery day 11 % etc. yet)

import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from scipy.stats import norm, poisson

def simulate_merton_jump_diffusion(S0=100, mu=0.08, sigma=0.15, lamb=0.1, 
                                   jump_mean=-0.4, jump_std=0.1, T=30, dt=1/252, num_simulations=5):
    """
    Simulate Merton Jump-Diffusion process
    - S0: initial price
    - mu: drift (civilizational growth)
    - sigma: volatility  
    - lamb: jump intensity (jumps per year)
    - jump_mean: average jump size (log return)
    - jump_std: std dev of jump sizes
    - T: time in years
    - dt: time step (daily)
    - num_simulations: number of paths to simulate
    """
    
    # Time array
    N = int(T/dt)
    t = np.arange(0, T, dt)
    
    # Results storage
    paths = np.zeros((num_simulations, N))
    paths[:, 0] = np.log(S0)
    
    for i in range(num_simulations):
        # Brownian motion component
        dW = np.random.normal(0, np.sqrt(dt), N-1)
        
        # Jump component - Poisson process
        jump_arrivals = np.random.poisson(lamb*dt, N-1)
        jump_sizes = np.random.normal(jump_mean, jump_std, N-1) * jump_arrivals
        
        # Combine components using our derived equation
        for j in range(1, N):
            diffusion = (mu - 0.5*sigma**2)*dt + sigma*dW[j-1]
            paths[i, j] = paths[i, j-1] + diffusion + jump_sizes[j-1]
    
    # Convert log prices back to actual prices
    price_paths = np.exp(paths)
    
    return t, price_paths

# Parameters (using our "realistic" estimates)
params = {
    'S0': 100000,    # Initial investment of $100,000
    'mu': 0.09,      # 8% mu (productivity / civilizational growth)
    'sigma': 0.14,   # 14% volatility
    'lamb': 0.08,    # 0.08 jumps/year = one crash every 12.5 years
    'jump_mean': np.log(0.6),  # Average 40% drop
    'jump_std': 0.05,          # Some variation in crash severity
    'T': 30,         # 30-year investment horizon
    'num_simulations': 100
}

# Run simulation
t, paths = simulate_merton_jump_diffusion(**params)

# Plot results
plt.figure(figsize=(12, 6))
for i in range(params['num_simulations']):
    plt.plot(t, paths[i, :], alpha=0.7, linewidth=1)

plt.axhline(y=params['S0'], color='red', linestyle='--', label='Initial Investment')
plt.title('Merton Jump-Diffusion Simulation: 30-Year Investment Horizons')
plt.xlabel('Years')
plt.ylabel('Portfolio Value ($)')
plt.yscale('log')  # Log scale to see compounding clearly
plt.legend()
plt.grid(True, alpha=0.3)
plt.show()

# Calculate statistics
final_values = paths[:, -1]
print(f"Initial investment: ${params['S0']:,.2f}")
print(f"Average final value after {params['T']} years: ${np.mean(final_values):,.2f}")
print(f"Median final value: ${np.median(final_values):,.2f}")
print(f"Best case: ${np.max(final_values):,.2f}")
print(f"Worst case: ${np.min(final_values):,.2f}")
print(f"Probability of losing money: {(final_values < params['S0']).mean():.1%}")
