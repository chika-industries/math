"""
Shows effects of the fight between short-term noise and long-term drift on stock price movements.
Y-axis can be chosen to be log (to see "linearity" in drift) or normal (to see exponential increase).

Source:
https://quantpy.com.au/stochastic-calculus/simulating-geometric-brownian-motion-gbm-in-python/
"""

#logarithmic = False
logarithmic = True

import numpy as np
import matplotlib.pyplot as plt

# Parameters
mu = 0.1    # drift coefficent
n = 3000    # number of steps
T = 30      # time in years
M = 1       # number of sims
#S0 = 100    # initial stock price
S0 = 26773  # emaxis mitsubishi ufj SPYTF
sigma = 0.2 # volatility

# DCA parameters
dca_delta = 30                  # Time interval for purchases in days
#investment_per_period = 10     # Amount invested at each time point
investment_per_period = 20000   # ideco - Amount invested at each time point

# Calc each time step
# Simulation using numpy arrays
dt = T/n
St = np.exp(
    (mu - sigma ** 2 / 2) * dt
    + sigma * np.random.normal(0, np.sqrt(dt), size=(M,n)).T
)

# Include array of 1's
# Multiply through by S0 and return the cumulative product of elements along 
# a given simulation path (axis=0). 
St = np.vstack([np.ones(M), St])
St = S0 * St.cumprod(axis=0)

# Define time interval correctly 
time = np.linspace(0,T,n+1)

# Require numpy array that is the same shape as St
tt = np.full(shape=(M,n+1), fill_value=time).T
plt.plot(tt, St, label='GBM')

if logarithmic is True:
    plt.yscale("log")  # Set y-axis to log scale

plt.xlabel("Years $(t)$")
plt.ylabel("Stock Price $(S_t)$")
plt.title(
    "Realizations of Geometric Brownian Motion\n $dS_t = \mu S_t dt + \sigma S_t dW_t$\n $S_0 = {0}, \mu = {1}, \sigma = {2}$".format(S0, mu, sigma)
)

# Calculate purchased points using dollar-cost averaging
shares_owned = 0
shares_owned_array = []
price_paid_array = []
for i in range(0, n + 1, dca_delta):
    shares_purchased = investment_per_period / St[i]
    shares_owned += shares_purchased
    shares_owned_array.append(shares_owned)
    price_paid_array.append(investment_per_period)
    print("Purchasing: ", shares_purchased, " shares of stock at DCA step = ", i)
    print("Price: ", investment_per_period)
    print("Total shared owns now:", shares_owned)
    print()

# Calculate the value of the investment over time
#investment_value = shares_owned_array * St

# Report on DCA
price_mean = np.mean(price_paid_array)
price_sum = np.sum(price_paid_array)
final_investment_value = St[-1] * shares_owned

print("Total price paid  : ", price_sum)
print("Average price paid: ", price_mean)
print("Total shares owned: ", shares_owned)
print("Marked value of owned shares: ", final_investment_value)

plt.axhline(y=price_sum, color='red', linestyle='--', label='total price paid')
plt.axhline(y=price_mean, color='blue', linestyle='--', label='average price paid')
plt.axhline(y=final_investment_value, color='green', linestyle='--', label='market value of owned shares at last timepoint')

#print(investment_value)
# S(t) only shows the movement of Mr. Market over time.
# When were purchases actually made?
#print(St)

#number_of_market_points = len(St)
#print(number_of_market_points)

# Purchased every 60th point
purchased_indices = np.arange(0, n + 1, dca_delta)


# Purchased every quarter (if 30 years, 4*30 years. Every 120th point is a purchase)
#purchased_points = St[::dca_delta]
#print("Purchased at:", purchased_points, " $ (time: ", tt[::dca_delta], ")")


#

#print("Average purchase price: ", mean)

# Highlighting purchased points
for i in purchased_indices:
  plt.axvspan(time[i], time[i], color='orange', alpha=0.5)


#plt.fill_between(tt, St, mean, color='gray', alpha=0.5, label='Filled Area')
plt.legend()
#plt.show()
plt.savefig("dca.png", dpi=300)
