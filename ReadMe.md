Tax Analyzer
============
This tool gives you a detailed slab-by-slab breakdown of the taxes owed by an unmarried individual living in New York
City. This tool is written in [ReasonML](https://reasonml.github.io/), and
uses [ReasonReact](https://reasonml.github.io/reason-react/) and [Recharts](http://recharts.org/en-US/).

How to Use
----------

### Click here: https://arjun-menon.com/tax-analyzer/

Building
--------
To set up and install dependencies, run:

```sh
npm run init
npm install
```

During development, run:

```sh
npm run watch
# in another tab:
npm run serve
```

Both are long running commands, that will monitor the filesystem for changes, and automatically rebuild.

To generate a production-optimized JS artifact, run:

```sh
npm run bundle
```

The base ReasonML template for this was [this](https://github.com/melange-re/melange-opam-template).

### Old Screenshot

You can see how the slabs look below:
![Snapshot of web-based interface](https://raw.githubusercontent.com/arjun-menon/tax-analyzer/master/res/snapshot-web.png)

Caveats
-------

The purpose of this web app is to just get a rough analysis of the taxes you owe. It doesn't cover things like:

* Alternative Minimum Tax: if you have a high income and take a large number of tax deductions, you may be subject to
  the [Alternative Minimum Tax](https://en.wikipedia.org/wiki/Alternative_minimum_tax).

* Personal Exemptions Phase-out: personal exemptions are reduced with higher incomes, and
  eventually [phased out](https://en.wikipedia.org/wiki/Personal_exemption_(United_States)#Phase-out). This is no longer
  relevant after 2018, since the tax reform passed in December 2017 eliminated federal personal exemptions.

* New York
  State [restrictions on itemized deductions](http://barclaydamon.com/alerts/New-Restrictions-on-Itemized-Deductions-for-New-York-Income-Tax-Purposes-05-28-2009)
  for high-income earners.

* Tax Credits, federal and state.
    * New York State and New York City both offer a variety of credtis, see:
        * [New York State Credits](http://www.tax.ny.gov/pit/credits/income_tax_credits.htm)
        * [New York City Credits](http://www.tax.ny.gov/pit/credits/new_york_city_credits.htm)

      For example, most single NYC residents qulify for
      the [school tax credit](http://www.tax.ny.gov/pit/credits/new_york_city_credits.htm#nyc_school) ([form NYC-210](http://www.tax.ny.gov/pdf/current_forms/it/nyc210_fill_in.pdf), [instructions](http://www.tax.ny.gov/pdf/current_forms/it/nyc210i.pdf)).
      Lower incomes make one eligible for a wide variety of credits such as
      the [NYS Household Credit](http://www.tax.ny.gov/pit/credits/household_credit.htm),
      the [NYS earned income credit](http://www.tax.ny.gov/pit/credits/earned_income_credit.htm), the
      federal [EITC](http://www.irs.gov/Individuals/EITC,-Earned-Income-Tax-Credit,-Questions-and-Answers), the
      federal [Saver's Credit](http://www.irs.gov/Retirement-Plans/Plan-Participant,-Employee/Retirement-Topics-Retirement-Savings-Contributions-Credit-%28Saver%E2%80%99s-Credit%29),
      etc.
