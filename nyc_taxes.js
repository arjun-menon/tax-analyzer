
/*
    Tax rates and other related data for calculateing the 
    total U.S. taxes owed by an unmarried resident of NYC
*/

/***************.
| Federal Taxes |
\***************/

// Federal rates based on et al: http://www.forbes.com/sites/kellyphillipserb/2013/10/31/irs-announces-2014-tax-brackets-standard-deduction-amounts-and-more/
// NYS and NYC rates based on: http://www.tax.ny.gov/pit/file/tax_tables.htm
//     and http://taxes.about.com/od/statetaxes/a/New-York-income-taxes.htm

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
    return (val * percentage)/100.0 //(val * percentage)/100.0
}

function numberWithCommas(x) { // from http://stackoverflow.com/a/2901298
    return x.toString().replace(/\B(?=(\d{3})+(?!\d))/g, ",");
}

function ns(number) {
    return numberWithCommas(number.toFixed(2))
}

function calc_slab_tax(income, slab_schedule) {
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
        console.log("   for the slab from $%s to $%s = $%s (at %d%% on $%s)", 
            ns(prev_limit), 
            slab_limit, 
            ns(tax_amt), 
            rate, 
            ns(slab_amt))

        result += tax_amt

        if (income <= limit)
            break

        prev_limit = limit
    }

    return result
}

function calc_alternative_minimum_tax(full_income) {
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

    console.log("\nMinimum Federal Income Tax: $%s (at %d%% on $%s)", 
        ns(amt), rate, ns(taxable_income))

    return amt
}

//calc_taxes(income, itemized_deductions=0, exemptions = 1)
function calc_taxes(income, itemized_deductions, exemptions) {
    console.log("Adjusted Gross Income = $%s", ns(income))

    // ------------- New York State & City -------------

    console.log("\nNew York State Tax Deductions:")

    nys_deduction = 0.0

    if (income <= new_york_state_federal_itemized_deductions_agi_limit)
        nys_deduction = itemized_deductions

    if (nys_deduction <= new_york_state_standard_deduction_single) {
        console.log("    New York State Standard Deduction = $%s", 
            ns(new_york_state_standard_deduction_single))
        nys_deduction = new_york_state_standard_deduction_single
    }
    else
        console.log("    Itemized Deductions = $%s", ns(itemized_deductions))

    // NYS Personal Exemptions for Dependents
    no_of_dependents = Math.max(exemptions - 1, 0)

    nys_personal_exemptions = no_of_dependents * new_york_state_personal_exemption_dependents

    if (no_of_dependents > 0)
        console.log("    Personal exemptions for dependents = $%s  (%d x $%s)", 
            ns(nys_personal_exemptions), no_of_dependents, 
            ns(new_york_state_personal_exemption_dependents))

    nys_deduction += nys_personal_exemptions

    console.log("New York State Tax Deductions = $%s", ns(nys_deduction))

    nys_taxable_income = Math.max(income - nys_deduction, 0)

    console.log("\nNew York State Taxable Income = $%s", ns(nys_taxable_income))

    console.log("\nNew York State Income Tax:")

    nys_income_tax = calc_slab_tax(nys_taxable_income, new_york_state_tax_schedule_2014_single)

    console.log("New York State Income Tax = $%s", ns(nys_income_tax))

    console.log("\nNew York City Income Tax:")
    nyc_income_tax = calc_slab_tax(nys_taxable_income, new_york_city_tax_schedule_2014_single)

    console.log("   minus the New York City School Tax Credit of $%s", 
        ns(nyc_school_tax_credit_single))

    nyc_income_tax -= nyc_school_tax_credit_single

    console.log("New York City Income Tax = $%s", ns(nyc_income_tax))

    total_state_and_local_income_tax = nys_income_tax + nyc_income_tax

    console.log("\nTotal New York State & City Taxes = $%s", 
        ns(total_state_and_local_income_tax))

    // ------------- Federal -------------

    console.log("\nFederal Tax Deductions:")

    federal_deduction = itemized_deductions + total_state_and_local_income_tax

    if (federal_deduction < standard_deduction_2014_single_person) {
        federal_deduction = standard_deduction_2014_single_person
        console.log("    Federal Standard Deduction = $%s", ns(federal_deduction))
    }
    else {
        console.log("    State, Local & Foreign Tax Deduction = $%s", ns(total_state_and_local_income_tax))
        console.log("    Additional Itemized Deductions = $%s", ns(itemized_deductions))
    }

    federal_personal_exemptions = federal_personal_exemption_2014 * exemptions

    if (income > federal_personal_exemption_phaseout_agi_maximum_single)
        federal_personal_exemptions = 0
    else
        console.log("    Personal Exemptions = $%s  (%d x $%s)", 
            ns(federal_personal_exemptions), exemptions, 
            ns(federal_personal_exemption_2014))

    federal_deduction += federal_personal_exemptions

    console.log("Federal Tax Deductions = $%s", ns(federal_deduction))

    federal_taxable_income = Math.max(income - federal_deduction, 0)

    console.log("\nFederal Taxable Income = $%s", ns(federal_taxable_income))

    console.log("\nFederal Income Tax:")

    federal_income_tax = calc_slab_tax(federal_taxable_income, federal_income_tax_schedule_2014_single)

    console.log("Federal Income Tax = $%s", ns(federal_income_tax))

    alternative_minimum_tax = calc_alternative_minimum_tax(income)

    actual_federal_income_tax = Math.max(federal_income_tax, alternative_minimum_tax)

    console.log("\nFederal Insurance Contributions Act (FICA) Tax:")

    social_security_taxable_income = Math.min(income, social_security_taxable_income_limit)
    social_security_tax = p(social_security_taxable_income, social_security_tax_rate)

    console.log("    Social Security's Old-Age, Survivors, and Disability Insurance \
(OASDI) Tax: $%s (at %d%% flat on $%s)", 
        ns(social_security_tax), social_security_tax_rate, ns(income))

    medicare_tax = p(income, medicare_tax_rate)

    console.log("    Medicare's Hospital Insurance (HI) Tax: $%s (at %d%% flat on $%s)", 
        ns(medicare_tax), medicare_tax_rate, ns(income))

    fica_tax = social_security_tax + medicare_tax

    console.log("Federal Insurance Contributions Act (FICA) Tax = $%s", ns(fica_tax))

    total_federal_tax = actual_federal_income_tax + fica_tax

    console.log("\nTotal Federal Taxes = $%s", ns(total_federal_tax))

    // ----------------------- Total -----------------------

    total_tax = total_federal_tax + total_state_and_local_income_tax

    income_after_tax = income - total_tax

    monthly_income_after_tax = income_after_tax / 12.0

    console.log("\nTotal Federal, State & Local Tax: $%s", ns(total_tax))

    console.log("\nIncome after taxation = $%s", ns(income_after_tax))

    console.log("Effective Tax Rate = %d%%", (total_tax*100.0/income).toFixed(2))

    console.log("Monthly Income = $%s", ns(monthly_income_after_tax))
}

exports.calc_taxes = calc_taxes
