## 1. Portfolio Future Value (Stock Positions)
**Sequence:**
- `[2nd]` `[CLR TVM]`
- `10` `[N]` (10 years)
- `7` `[I/Y]` (7% growth)
- `[current amount]` `[+/-]` `[PV]` (e.g., 250000)
- `0` `[PMT]`
- `[CPT]` `[FV]`

---

## 2. Real Estate (Land Appreciation Only)
**Sequence:**
- `[2nd]` `[CLR TVM]`
- `10` `[N]`
- `0.5` `[I/Y]` (0.5% land appreciation - Japan typical)
- `[land value only]` `[+/-]` `[PV]` (e.g., 10.000.000 JPY for land only)
- `0` `[PMT]`
- `[CPT]` `[FV]`
- **Then add back** the building value (unchanged): Land FV + Building current value

---

## 3. Annuity Growth
**Sequence:**
- `[2nd]` `[CLR TVM]`
- `10` `[N]`
- `2.5` `[I/Y]` (2.5% typical European pension growth)
- `35000` `[+/-]` `[PV]` (35,000 EUR)
- `3200` `[+/-]` `[PMT]`
- `[CPT]` `[FV]`

---

## 4. Japanese iDeCo (Monthly Contributions)
**Sequence:**
- `[2nd]` `[CLR TVM]`
- `120` `[N]` (10 years × 12 months)
- `7` `[I/Y]` (annual rate)
- `[÷]` `12` `[=]` (converts to monthly rate - calculator does this automatically)
- `[current ideco balance]` `[+/-]` `[PV]` (e.g., 300.000 JPY if you have existing balance)
- `20000` `[+/-]` `[PMT]` (20,000 JPY monthly contribution)
- `[CPT]` `[FV]`

**Note:** Make sure calculator is in END mode for payments at end of period

---

## 5. Opportunity Cost of Current Assets
**What-If Scenario:**

**Current situation (cash earning ~0%):**
- `[2nd]` `[CLR TVM]`
- `10` `[N]`
- `0.1` `[I/Y]` (0.1% typical savings rate)
- `[cash amount]` `[+/-]` `[PV]`
- `0` `[PMT]`
- `[CPT]` `[FV]` → **Result A**

**If invested at market rate:**
- `[2nd]` `[CLR TVM]`
- `10` `[N]`
- `7` `[I/Y]` (market growth rate)
- `[same cash amount]` `[+/-]` `[PV]`
- `0` `[PMT]`
- `[CPT]` `[FV]` → **Result B**

**Opportunity cost = Result B - Result A**

---

## Summary Table Template:

| Component | Current Value | FV (10 years) | Currency |
|-----------|--------------|---------------|----------|
| Stock Portfolio | | | USD |
| Real Estate (land) | | | JPY |
| Real Estate (building) | | | JPY |
|  Annuity |  | | EUR |
| iDeCo | | | JPY |
| Current Assets | | | JPY |
| **Opportunity Cost** | | | JPY |

Convert everything to USD at current rates for total net worth comparison!
