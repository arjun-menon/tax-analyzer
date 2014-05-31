#!/usr/bin/python

import sys, locale

# Federal rates based on et al: http://www.forbes.com/sites/kellyphillipserb/2013/10/31/irs-announces-2014-tax-brackets-standard-deduction-amounts-and-more/
# NYS and NYC rates based on: http://www.tax.ny.gov/pit/file/tax_tables.htm

standard_deduction_2014_single_person = 6200

personal_exemption_2014 = 3950

social_security_tax_rate =  6.2
medicare_tax_rate = 1.45

federal_income_tax_schedule_2014_single = [
#   (Limit, Rate)
    (9075       , 10),
    (36900      , 15),
    (89350      , 25),
    (186350     , 28),
    (405100     , 33),
    (406750     , 35),
    (sys.maxint , 39.6)
]

alternative_minimum_tax_low_rate = 26
alternative_minimum_tax_high_rate = 28
alternative_minimum_tax_exemption_2014 = 52800
alternative_minimum_tax_exemption_phaseout_threshold_2014 = 115400
alternative_minimum_tax_high_rate_threshold_2014 = 179500

new_york_state_tax_schedule_2014_single = [
#   (Limit, Rate)
    (8200       , 4.00),
    (11300      , 4.50),
    (13350      , 5.25),
    (20550      , 5.90),
    (77150      , 6.45),
    (205850     , 6.65),
    (1029250    , 6.85),
    (sys.maxint , 8.82)
]

new_york_city_tax_schedule_2014_single = [
#   (Limit, Rate)
    (12000      , 2.907),
    (25000      , 3.534),
    (50000      , 3.591),
    (500000     , 3.648),
    (sys.maxint , 3.876)
]

def p(val, percentage):
    return (float(val) * percentage)/100

locale.setlocale(locale.LC_ALL, '')
def ns(n): # number to string
    return locale.format("%.2f", n, grouping=True)

def calc_slab_tax(income, slab_schedule):
    result = 0.0

    prev_limit = 0
    for (limit, rate) in slab_schedule:
        if income <= limit:
            slab_amt = income - prev_limit
        else: # income > limit
            slab_amt = limit - prev_limit

        tax_amt = p(slab_amt, rate)
        print("   for the slab from $%s to $%s = $%s (at %1.2f%% on $%s)" % 
            (ns(prev_limit), ns(limit) if income > limit else ns(income), 
                ns(tax_amt), rate, ns(slab_amt)))
        result += tax_amt

        if income <= limit:
            break

        prev_limit = limit

    return result

def calc_alternative_minimum_tax(full_income):
    exemption = 0.0
    if full_income < alternative_minimum_tax_exemption_phaseout_threshold_2014:
        exemption = alternative_minimum_tax_exemption_2014
    taxable_income = full_income - exemption
    if taxable_income < 0:
        taxable_income = 0

    if full_income <= alternative_minimum_tax_high_rate_threshold_2014:
        rate = alternative_minimum_tax_low_rate
    else:
        rate = alternative_minimum_tax_high_rate

    amt = p(taxable_income, rate)

    print("\nMinimum Federal Income Tax: $%s (at %d%% on $%s)" % 
        (ns(amt), rate, ns(taxable_income)))
    return amt

def calc_taxes(income, itemized_deductions=0, exemptions = 1):
    print("Annual Gross Income = $%s" % ns(income))

    print("\nNew York City Income Tax:")
    nyc_income_tax = calc_slab_tax(income, new_york_city_tax_schedule_2014_single)
    print("New York City Income Tax = $%s" % ns(nyc_income_tax))

    print("\nNew York State Income Tax:")
    nys_income_tax = calc_slab_tax(income, new_york_state_tax_schedule_2014_single)
    print("New York State Income Tax = $%s" % ns(nys_income_tax))

    total_state_and_local_income_tax = nys_income_tax + nyc_income_tax
    print("\nTotal New York State & City Taxes = $%s" % ns(total_state_and_local_income_tax))

    # ------------- Federal Taxes -------------

    print("\nFederal Tax Deductions:")

    federal_deduction = itemized_deductions + total_state_and_local_income_tax

    if federal_deduction < standard_deduction_2014_single_person:
        federal_deduction = standard_deduction_2014_single_person
        print("    Standard Deduction = $%s" % ns(federal_deduction))
    else:
        print("    State, Local & Foreign Tax Deduction = $%s" % ns(total_state_and_local_income_tax))
        print("    Additional Itemized Deductions = $%s" % ns(itemized_deductions))

    personal_exemption = personal_exemption_2014 * exemptions
    if income > 254200:
        personal_exemption = 0
    else:
        print("    Personal Exemptions = $%s  (%d x $%s)" % 
            (ns(personal_exemption), exemptions, ns(personal_exemption_2014)))
    federal_deduction += personal_exemption

    print("Federal Tax Deductions = $%s" % ns(federal_deduction))

    federal_taxable_income = income - federal_deduction
    if federal_taxable_income < 0:
        federal_taxable_income = 0
    print("\nFederal Taxable Income = $%s" % ns(federal_taxable_income))

    print("\nFederal Income Tax:")
    federal_income_tax = calc_slab_tax(
        federal_taxable_income, federal_income_tax_schedule_2014_single)
    print("Federal Income Tax = $%s" % ns(federal_income_tax))

    alternative_minimum_tax = calc_alternative_minimum_tax(income)

    actual_federal_income_tax = max(federal_income_tax, alternative_minimum_tax)

    print("\nFederal Insurance Contributions Act (FICA) Tax:")
    social_security_tax = p(federal_taxable_income, social_security_tax_rate)
    print("    Social Security's Old-Age, Survivors, and Disability Insurance \
(OASDI) Tax: $%s (at %1.2f%% flat rate)" % 
        (ns(social_security_tax), social_security_tax_rate))
    medicare_tax = p(federal_taxable_income, medicare_tax_rate)
    print("    Medicare's Hospital Insurance (HI) Tax: $%s (at %1.2f%% flat rate)" % 
        (ns(medicare_tax), medicare_tax_rate))
    fica_tax = social_security_tax + medicare_tax
    print("Federal Insurance Contributions Act (FICA) Tax = $%s" % ns(fica_tax))

    total_federal_tax = actual_federal_income_tax + fica_tax
    print("\nTotal Federal Taxes = $%s" % ns(total_federal_tax))

    total_tax = total_federal_tax + total_state_and_local_income_tax
    print("\nTotal Federal, State & Local Tax: $%s" % ns(total_tax))

    income_after_tax = income - total_tax
    print("\nIncome after taxation = $%s" % ns(income_after_tax))
    monthly_income_after_tax = income_after_tax / 12.0
    print("Monthly Income = $%s" % ns(monthly_income_after_tax))

def main():
    if len(sys.argv) != 2:
        print("Please specify in exactly one argument, your annual gross income.")
    annual_gross_income = int(sys.argv[1])

    calc_taxes(annual_gross_income)

if __name__ == "__main__":
    main()
