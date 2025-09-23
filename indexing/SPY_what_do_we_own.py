import requests
import pandas as pd
from bs4 import BeautifulSoup

def get_sp500_holdings():
    url = "https://www.slickcharts.com/sp500"
    headers = {"User-Agent": "Mozilla/5.0"}
    response = requests.get(url, headers=headers)
    response.raise_for_status()
    
    soup = BeautifulSoup(response.text, "html.parser")
    table = soup.find("table", {"class": "table"})
    df = pd.read_html(str(table))[0]
    
    # Keep only relevant columns and rename them
    df = df[["Company", "Symbol", "Weight"]]
    df["Weight"] = df["Weight"].str.rstrip('%').astype(float)
    return df

def calculate_investment_distribution(investment_amount):
    holdings_df = get_sp500_holdings()
    holdings_df["Investment (JPY)"] = (holdings_df["Weight"] / 100) * investment_amount
    return holdings_df

if __name__ == "__main__":
    investment_amount = 300000  # Your investment in JPY
    holdings = calculate_investment_distribution(investment_amount)
    pd.set_option("display.max_rows", None)  # Show all rows
    print(holdings)
