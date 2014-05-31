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

Usage
-----
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

