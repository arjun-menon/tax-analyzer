
On Living in NJ and commuting to NYC for work (2014)
---------------------------------------------

### Input

  * Income = $85,000

  * Itemized Deductions = $23,100 ($5,100 contribution to 401k + $18,000 in deductible charitable donations)

### State & City Taxes in NY

Source: http://www.arjungmenon.com/nyc-tax-calculator/?income=85000&deductions=23100&exemptions=1

  * NY State Tax = $3667

  * **NY City Tax = $2077**

You have to continue paying NY state tax regardless of where you reside.

### NJ State Tax

Tax Rates: http://www.tax-rates.org/new_jersey/income-tax

*Note*: NJ offers no standard deduction + You can't deduct charitable donations, your 410k contribution and other federal itemized deductions from your NJ taxable income.

Calculating NJ State Tax (with Python):

```python
    p = lambda amt, pc: (amt*pc)/100.0
    nj_state_tax = ( p(20, 1.40) + p(15, 1.75) + p(5, 3.5) + p(35, 5.53) + p(10, 6.37) )*1000 = 3290
```

**NJ State Tax = $3290**

You can take a credit for the taxes you paid to NY on your NJ state tax return. However, if your NY tax is lower than your NJ tax, you will have to pay the difference to NJ.

Here, since the NJ state tax is lower, you end up *paying the higher NY state tax* (of $3667).

### Additional Transit Cost

If you continue to retain a monthly NYC subway pass (which costs $112), the additional transit cost is:

  * PATH (monthly pass) = $80

  * NJ Transit (monthly pass) = betw. $116 and $139 ($116 for Secaucus-NYPenn; $139 for Newark-NYPenn)

So a minimum additional transit cost of $80, in order to commute to NYC from NJ.

### Your Savings

Tax reduction by moving to NJ = $2077 (NY City Tax) = $173 per month

Effective Savings = $173 (money saved by moving to NJ) - $80 (additional transit cost) = **$93 per month**
