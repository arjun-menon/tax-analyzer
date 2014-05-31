NYC Tax Calculator
==================
This Python program calculates the total U.S. taxes owed by a single (unmarried) 
resident of New York City. It is limited in its scope and functionality, and 
was written primarily for personal use. It is also far from perfect, and the 
numbers calculated should only be considered as a guide. However, generally 
speaking, your tax should not _exceed_ the amounts calculated by this program.

Federal rates (2014) are based on http://www.forbes.com/sites/kellyphillipserb/2013/10/31/irs-announces-2014-tax-brackets-standard-deduction-amounts-and-more/ and other sources.

New York State & City tax rates (2014) are based on http://www.tax.ny.gov/pit/file/tax_tables.htm

*Note*: for more on state & local tax federal income tax deductions, see: http://taxes.about.com/od/Deductions/a/State-Income-Tax-Deduction.htm

###Usage

    usage: nyc_tax_calculator.py [-h] [-e EXEMPTIONS] [-d DEDUCTIONS] income

    Calculates total U.S. taxes owed by a single (unmarried) resident of NYC.

    positional arguments:
      income                Annual Gross Income

    optional arguments:
      -h, --help            show this help message and exit
      -e EXEMPTIONS, --exemptions EXEMPTIONS
                            Number of personal exemptions. The default is 1.
      -d DEDUCTIONS, --deductions DEDUCTIONS
                            Itemized deduction other than the standard deduction,
                            personal exemptions, and state/local/foreign tax
                            deductions.

### Sample Output
Sample output rendered by the program for a person with a gross annual 
income of $93,500, no additional itemized deducations, and claiming a 
single personal exemption:

    Annual Gross Income = $93,500.00

    New York City Income Tax:
       for the slab from $0.00 to $12,000.00 = $348.84 (at 2.91% on $12,000.00)
       for the slab from $12,000.00 to $25,000.00 = $459.42 (at 3.53% on $13,000.00)
       for the slab from $25,000.00 to $50,000.00 = $897.75 (at 3.59% on $25,000.00)
       for the slab from $50,000.00 to $93,500.00 = $1,586.88 (at 3.65% on $43,500.00)
    New York City Income Tax = $3,292.89

    New York State Income Tax:
       for the slab from $0.00 to $8,200.00 = $328.00 (at 4.00% on $8,200.00)
       for the slab from $8,200.00 to $11,300.00 = $139.50 (at 4.50% on $3,100.00)
       for the slab from $11,300.00 to $13,350.00 = $107.62 (at 5.25% on $2,050.00)
       for the slab from $13,350.00 to $20,550.00 = $424.80 (at 5.90% on $7,200.00)
       for the slab from $20,550.00 to $77,150.00 = $3,650.70 (at 6.45% on $56,600.00)
       for the slab from $77,150.00 to $93,500.00 = $1,087.28 (at 6.65% on $16,350.00)
    New York State Income Tax = $5,737.90

    Total New York State & City Taxes = $9,030.79

    Federal Tax Deductions:
        State, Local & Foreign Tax Deduction = $9,030.79
        Additional Itemized Deductions = $0.00
        Personal Exemptions = $3,950.00  (1 x $3,950.00)
    Federal Tax Deductions = $12,980.79

    Federal Taxable Income = $80,519.21

    Federal Income Tax:
       for the slab from $0.00 to $9,075.00 = $907.50 (at 10.00% on $9,075.00)
       for the slab from $9,075.00 to $36,900.00 = $4,173.75 (at 15.00% on $27,825.00)
       for the slab from $36,900.00 to $80,519.21 = $10,904.80 (at 25.00% on $43,619.21)
    Federal Income Tax = $15,986.05

    Minimum Federal Income Tax: $10,582.00 (at 26% on $40,700.00)

    Federal Insurance Contributions Act (FICA) Tax:
        Social Security's Old-Age, Survivors, and Disability Insurance (OASDI) Tax: $4,992.19 (at 6.20% flat rate)
        Medicare's Hospital Insurance (HI) Tax: $1,167.53 (at 1.45% flat rate)
    Federal Insurance Contributions Act (FICA) Tax = $6,159.72

    Total Federal Taxes = $22,145.77

    Total Federal, State & Local Tax: $31,176.56

    Income after taxation = $62,323.44
    Monthly Income = $5,193.62

### Todos

* Fix the [AMT](https://en.wikipedia.org/wiki/Alternative_minimum_tax) 
  phase-out calculation.

