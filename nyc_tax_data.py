# Tax rates and other related data for calculateing the 
# total U.S. taxes owed by an unmarried resident of NYC

import sys, locale

###########
# FEDERAL #
###########

# Federal rates based on et al: http://www.forbes.com/sites/kellyphillipserb/2013/10/31/irs-announces-2014-tax-brackets-standard-deduction-amounts-and-more/
# NYS and NYC rates based on: http://www.tax.ny.gov/pit/file/tax_tables.htm
#     and http://taxes.about.com/od/statetaxes/a/New-York-income-taxes.htm

standard_deduction_2014_single_person = 6200

federal_personal_exemption_2014 = 3950
federal_personal_exemption_phaseout_agi_beginning_single = 166800
federal_personal_exemption_phaseout_agi_maximum_single = 289300

social_security_tax_rate =  6.2
social_security_taxable_income_limit = 117000  # as of 2014
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

#########
# STATE #
#########

new_york_state_standard_deduction_single = 7700 # as of 2013
new_york_state_personal_exemption_dependents = 1000
new_york_state_federal_itemized_deductions_agi_limit = 100000

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

#########
# LOCAL #
#########

new_york_city_tax_schedule_2014_single = [
#   (Limit, Rate)
    (12000      , 2.907),
    (25000      , 3.534),
    (50000      , 3.591),
    (500000     , 3.648),
    (sys.maxint , 3.876)
]

nyc_school_tax_credit_single = 62.50 # for full-year residents not claimed as a dependent

###########################
# Some Auxiliary Functions
###########################

def p(val, percentage):
    return (float(val) * percentage)/100

locale.setlocale(locale.LC_ALL, '')
def ns(n): # number to string
    return locale.format("%.2f", n, grouping=True)
