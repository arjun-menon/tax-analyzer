
On Living in NJ and commuting to NYC for work (2014)
---------------------------------------------
at Income = $85,000

and $23,100 in federal itemized deductions ($5,100 contribution to 401k + $18,000 in deductible charitable donations)

State & City Taxes in NY (from http://www.arjungmenon.com/nyc-tax-calculator/?income=85000&deductions=23100&exemptions=1) are:

    NY State Tax = $3667

    NY City Tax = $2077

NJ State Tax

Tax Rates: http://www.tax-rates.org/new_jersey/income-tax

    Note: NJ offers no standard deduction + You can't deduct charitable donations, your 410k contribution and other federal itemized deductions from your NJ taxable income.

Calculating NJ State Tax (with Python):

    p = lambda amt, pc: (amt*pc)/100.0
    nj_state_tax = ( p(20, 1.40) + p(15, 1.75) + p(5, 3.5) + p(35, 5.53) + p(10, 6.37) )*1000 = 3290

NJ State Tax = $3290

Additional amount owed to NJ: NY State Tax - NJ State Tax = 3667 - 3290 = $377

Money saved by moving to NJ = $2077 (NY City Tax) - $377 (additional amount owed to NJ) = $1700 per year = $142 per month

If you continue to retain a monthly NYC subway pass (which costs $112), the additional transit cost is:

    (a) PATH (monthly pass) = $80

    (b) NJ Transit (monthly pass) = betw. $116 and $139 ($116 for Secaucus-NYPenn; $139 for Newark-NYPenn)

Effective Savings per month = $142 (money saved by moving to NJ) - $80 (additional transit costs) = $62

By living in NJ and commuting to NYC:

If you keep your NYC subway monthly pass, you save about $62 per month.  <------------- (1)

If you do not keep your NYC subway monthly pass, you save $172 per month.  <------------- (2)

If you do not keep your NYC subway monthly pass, and if you do not make $18,000 in deductible donations per year ($1,500 per month), you pay $1556 less in NY taxes per year ($130 per month), and save $302 per month.  <------------- (3)
