Calculate Taxes in NYC
======================
This tool gives you a detailed slab-by-slab breakdown of the taxes owed by taxes owed by an unmarried individual with no dependants living and working in New York City.

![Snapshot of web-based interface](https://raw.githubusercontent.com/arjun-menon/nyc-taxes/master/res/snapshot-web.png)

How to Use
----------
### Online: https://arjun-menon.github.io/nyc-taxes/

Building
--------
This uses ReasonReact. To build:
```sh
yarn install
yarn start
# in another tab
yarn run webpack
```

After you see the webpack compilation succeed (the `npm run webpack` step), open up `src/index.html` (**no server needed!**). Then modify whichever `.re` file in `src` and refresh the page to see the changes.

To build a production-optimized version:
```sh
yarn run build
yarn run webpack:production
```

This will replace the development artifact `build/Index.js` for an optimized version.

To enable dead code elimination, change `bsconfig.json`'s `package-specs` `module` from `"commonjs"` to `"es6"`. Then re-run the above 2 commands. This will allow Webpack to remove unused code.


Caveats
-------
The purpose of this tool is to estimate tax liabilities and verify the numbers produced by tools like TaxACT and TurboTax. As such, it does not cover less-common features of tax law, including:

#### Alternative Minimum Tax

If you have a high income and take a large number of tax deductions, you may be subject to the [Alternative Minimum Tax](https://en.wikipedia.org/wiki/Alternative_minimum_tax).

#### Personal Exemptions Phase-out

Personal exemptions are reduced with higher incomes, and eventually [phased out](https://en.wikipedia.org/wiki/Personal_exemption_(United_States)#Phase-out).

#### New York State Itemized Deductions Restrictions

New York State [restricts itemized deductions](http://barclaydamon.com/alerts/New-Restrictions-on-Itemized-Deductions-for-New-York-Income-Tax-Purposes-05-28-2009) for high-income earners.

#### Tax Credits
New York State and New York City both offer a variety of credtis, see:

 * [New York State Credits](http://www.tax.ny.gov/pit/credits/income_tax_credits.htm)
 * [New York City Credits](http://www.tax.ny.gov/pit/credits/new_york_city_credits.htm)

For example, most single NYC residents qulify for the [school tax credit](http://www.tax.ny.gov/pit/credits/new_york_city_credits.htm#nyc_school) ([form NYC-210](http://www.tax.ny.gov/pdf/current_forms/it/nyc210_fill_in.pdf), [instructions](http://www.tax.ny.gov/pdf/current_forms/it/nyc210i.pdf)). Lower incomes make one eligible for a wide variety of credits such as the [NYS Household Credit](http://www.tax.ny.gov/pit/credits/household_credit.htm), the [NYS earned income credit](http://www.tax.ny.gov/pit/credits/earned_income_credit.htm), the federal [EITC](http://www.irs.gov/Individuals/EITC,-Earned-Income-Tax-Credit,-Questions-and-Answers), the federal [Saver's Credit](http://www.irs.gov/Retirement-Plans/Plan-Participant,-Employee/Retirement-Topics-Retirement-Savings-Contributions-Credit-%28Saver%E2%80%99s-Credit%29), etc.

Sources
-------
* Federal tax rates (2014) are based on: http://www.forbes.com/sites/kellyphillipserb/2013/10/31/irs-announces-2014-tax-brackets-standard-deduction-amounts-and-more/ and other sources.
* New York State & City tax rates (2014) are based on: http://www.tax.ny.gov/pit/file/tax_tables.htm
* For more on state & local tax federal income tax deductions, see: http://taxes.about.com/od/Deductions/a/State-Income-Tax-Deduction.htm
* A Guide to New York Personal Income Tax: http://taxes.about.com/od/statetaxes/a/New-York-income-taxes.htm
    * Standard Deductions: http://www.tax.ny.gov/pit/file/standard_deductions.htm
    * Instructions for Form IT-201 (New York State Resident Income Tax Return): http://www.tax.ny.gov/pdf/2013/inc/it201i_2013.pdf
    * Instructions for Form IT-272 (New York State Residents' Claim for College Tuition Credit or Itemized Deduction): http://www.tax.ny.gov/pdf/current_forms/it/it272i.pdf
        * The non-refundable tax credit is limited to 4% of $10,000, i.e. $400.
        * The maximum itemized deduction for college is $10,000.
    * On NY itemized deductions: http://www.taxslayer.com/support/829/new-york-itemized-deductions
* A Guide to New York City Income Tax: http://taxes.about.com/od/statetaxes/a/New-York-City-Income-Tax.htm
    * New York City Credits: http://www.tax.ny.gov/pit/credits/new_york_city_credits.htm
    * Instructions for Form NYC-210 (Claim for NYC School Tax Credit): http://www.tax.ny.gov/pdf/current_forms/it/nyc210i.pdf
