/*-------------------------------------------------------------------*
 *  Calculate total U.S. taxes owed by an unmarried resident of NYC  *
 *-------------------------------------------------------------------*/

/***************.
| Federal Taxes |
\***************/

standard_deduction_2014_single_person = 6200

federal_personal_exemption_2014 = 3950
federal_personal_exemption_phaseout_agi_beginning_single = 166800
federal_personal_exemption_phaseout_agi_maximum_single = 289300

social_security_tax_rate =  6.2
social_security_taxable_income_limit = 117000  // as of 2014
medicare_tax_rate = 1.45

federal_income_tax_schedule_2014_single = [
//  (Limit, Rate)
    [9075       , 10],
    [36900      , 15],
    [89350      , 25],
    [186350     , 28],
    [405100     , 33],
    [406750     , 35],
    [Number.MAX_VALUE , 39.6]
]

alternative_minimum_tax_low_rate = 26
alternative_minimum_tax_high_rate = 28
alternative_minimum_tax_exemption_2014 = 52800
alternative_minimum_tax_exemption_phaseout_threshold_2014 = 115400
alternative_minimum_tax_high_rate_threshold_2014 = 179500

/*************.
| State Taxes |
\*************/

new_york_state_standard_deduction_single = 7700 // as of 2013
new_york_state_personal_exemption_dependents = 1000
new_york_state_federal_itemized_deductions_agi_limit = 100000

new_york_state_tax_schedule_2014_single = [
//  (Limit, Rate)
    [8200       , 4.00],
    [11300      , 4.50],
    [13350      , 5.25],
    [20550      , 5.90],
    [77150      , 6.45],
    [205850     , 6.65],
    [1029250    , 6.85],
    [Number.MAX_VALUE , 8.82]
]

/************.
| City Taxes |
\************/

new_york_city_tax_schedule_2014_single = [
//  (Limit, Rate)
    [12000      , 2.907],
    [25000      , 3.534],
    [50000      , 3.591],
    [500000     , 3.648],
    [Number.MAX_VALUE , 3.876]
]

nyc_school_tax_credit_single = 62.50 // for full-year residents not claimed as a dependent

/*******************.
| Utility Functions |
\*******************/

function p(val, percentage) {
    return (val * percentage)/100.0
}

function numberWithCommas(x) { // from http://stackoverflow.com/a/2901298
    return x.toString().replace(/\B(?=(\d{3})+(?!\d))/g, ",");
}

function ns(number) { // stringify a number represeting money:
    return "$" + numberWithCommas(number.toFixed(2))
}

/*********************.
| Slab Tax Calculator |
\*********************/

function calc_slab_tax(income, slab_schedule, w) {
    result = 0.0

    prev_limit = 0
    for (i in slab_schedule) {
        limit = slab_schedule[i][0]
        rate = slab_schedule[i][1]

        if (income <= limit)
            slab_amt = income - prev_limit
        else // income > limit
            slab_amt = limit - prev_limit

        tax_amt = p(slab_amt, rate)

        slab_limit = income > limit ? ns(limit) : ns(income)
        w('b', "Slab from " + ns(prev_limit) + " to " + slab_limit + 
            " = " + ns(tax_amt) + " (at " + rate + "% on " + ns(slab_amt) + ")")

        result += tax_amt

        if (income <= limit)
            break

        prev_limit = limit
    }

    return result
}

/****************.
| AMT Calculator |
\****************/

function calc_alternative_minimum_tax(full_income, w) {
    exemption = 0.0

    if (full_income < alternative_minimum_tax_exemption_phaseout_threshold_2014)
        exemption = alternative_minimum_tax_exemption_2014

    taxable_income = full_income - exemption

    if (taxable_income < 0)
        taxable_income = 0

    if (full_income <= alternative_minimum_tax_high_rate_threshold_2014)
        rate = alternative_minimum_tax_low_rate
    else
        rate = alternative_minimum_tax_high_rate

    amt = p(taxable_income, rate)

    w('a', "\nMinimum Federal Income Tax", 
        ns(amt) + " (at " + rate + "% on " + ns(taxable_income) + ")")

    return amt
}

/********************.
| The Tax Calculator |
\********************/

function calc_taxes(income, itemized_deductions, exemptions, w) {
    // default itemized_deductions = 0
    // default exemptions = 1

    w('a', "Adjusted Gross Income", ns(income))

    // ------------- New York State & City -------------

    w('l_start', "\nNew York State Tax Deductions")

    nys_deduction = 0.0

    if (income <= new_york_state_federal_itemized_deductions_agi_limit)
        nys_deduction = itemized_deductions

    if (nys_deduction <= new_york_state_standard_deduction_single) {
        w('b', "New York State Standard Deduction = " + 
            ns(new_york_state_standard_deduction_single))

        nys_deduction = new_york_state_standard_deduction_single
    }
    else
        w('b', "Itemized Deductions = " + ns(itemized_deductions))

    // NYS Personal Exemptions for Dependents
    no_of_dependents = Math.max(exemptions - 1, 0)

    nys_personal_exemptions = no_of_dependents * new_york_state_personal_exemption_dependents

    if (no_of_dependents > 0)
        w('b', "Personal exemptions for dependents = " + 
            ns(nys_personal_exemptions) + "  (" + no_of_dependents + 
            " x " + ns(new_york_state_personal_exemption_dependents) +")")

    nys_deduction += nys_personal_exemptions

    w('l_end', "New York State Tax Deductions", ns(nys_deduction))

    nys_taxable_income = Math.max(income - nys_deduction, 0)

    w('a', "\nNew York State Taxable Income", ns(nys_taxable_income))

    w('l_start', "\nNew York State Income Tax")

    nys_income_tax = calc_slab_tax(nys_taxable_income, 
        new_york_state_tax_schedule_2014_single, w)

    w('l_end', "New York State Income Tax", ns(nys_income_tax))

    w('l_start', "\nNew York City Income Tax")

    nyc_income_tax = calc_slab_tax(nys_taxable_income, 
        new_york_city_tax_schedule_2014_single, w)

    w('b', "minus the New York City School Tax Credit of " + 
        ns(nyc_school_tax_credit_single))

    nyc_income_tax -= nyc_school_tax_credit_single

    w('l_end', "New York City Income Tax", ns(nyc_income_tax))

    total_state_and_local_income_tax = nys_income_tax + nyc_income_tax

    w('a', "\nTotal New York State & City Taxes", ns(total_state_and_local_income_tax))

    // ------------- Federal -------------

    w('l_start', "\nFederal Tax Deductions")

    federal_deduction = itemized_deductions + total_state_and_local_income_tax

    if (federal_deduction < standard_deduction_2014_single_person) {
        federal_deduction = standard_deduction_2014_single_person
        w('b', "Federal Standard Deduction = " + ns(federal_deduction))
    }
    else {
        w('b', "State and Local Taxes Deduction = " + ns(total_state_and_local_income_tax))
        w('b', "Additional Itemized Deductions = " + ns(itemized_deductions))
    }

    federal_personal_exemptions = federal_personal_exemption_2014 * exemptions

    if (income > federal_personal_exemption_phaseout_agi_maximum_single)
        federal_personal_exemptions = 0
    else
        w('b', "Personal Exemptions = " + ns(federal_personal_exemptions) + 
            "  (" + exemptions + " x " + ns(federal_personal_exemption_2014) + ")")

    federal_deduction += federal_personal_exemptions

    w('l_end', "Federal Tax Deductions", ns(federal_deduction))

    federal_taxable_income = Math.max(income - federal_deduction, 0)

    w('a', "\nFederal Taxable Income", ns(federal_taxable_income))

    w('l_start', "\nFederal Income Tax")

    federal_income_tax = calc_slab_tax(federal_taxable_income, 
        federal_income_tax_schedule_2014_single, w)

    w('l_end', "Federal Income Tax", ns(federal_income_tax))

    alternative_minimum_tax = calc_alternative_minimum_tax(income, w)

    if (alternative_minimum_tax > federal_income_tax)
        w('t', "Your alternative minimum tax (AMT) exceeds your regular federal income tax.")

    actual_federal_income_tax = Math.max(federal_income_tax, alternative_minimum_tax)

    w('l_start', "\nFederal Insurance Contributions Act (FICA) Tax")

    social_security_taxable_income = Math.min(income, social_security_taxable_income_limit)
    social_security_tax = p(social_security_taxable_income, social_security_tax_rate)

    w('b', "Social Security's Old-Age, Survivors, and Disability Insurance (OASDI) Tax: " + 
        ns(social_security_tax) + " (at " + social_security_tax_rate + "% flat on " + ns(income) + ")")

    medicare_tax = p(income, medicare_tax_rate)

    w('b', "Medicare's Hospital Insurance (HI) Tax: " + ns(medicare_tax) + 
        " (at " + medicare_tax_rate + "% flat on " + ns(income) + ")")

    fica_tax = social_security_tax + medicare_tax

    w('l_end', "Federal Insurance Contributions Act (FICA) Tax", ns(fica_tax))

    total_federal_tax = actual_federal_income_tax + fica_tax

    w('a', "\nTotal Federal Taxes", ns(total_federal_tax))

    // ----------------------- Total -----------------------

    total_tax = total_federal_tax + total_state_and_local_income_tax

    income_after_tax = income - total_tax

    monthly_income_after_tax = income_after_tax / 12.0

    w('a', "\nTotal Federal, State & Local Taxes", ns(total_tax))

    w('x', "<hr/>")

    w('a', "\nIncome after Taxation", ns(income_after_tax))

    w('a', "Effective Tax Rate", (total_tax*100.0/income).toFixed(2) + "%")

    w('a', "Monthly Income", ns(monthly_income_after_tax))
}

if( !( typeof exports == 'undefined' ) )
    exports.calc_taxes = calc_taxes
